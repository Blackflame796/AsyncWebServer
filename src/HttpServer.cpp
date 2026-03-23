#include <HttpServer.hpp>

net::awaitable<void> default_handler(HttpContext &ctx) {
  ctx.response.status(http::status::ok);
  ctx.response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
  ctx.response.set(http::field::content_type, "text/plain");
  ctx.response.keep_alive(ctx.request.keep_alive());
  ctx.response.body("Hello, your body was: " + ctx.request.body());
  ctx.response.prepare_payload();
  co_return;
}

HttpServer::HttpServer() {
  middleware = std::make_shared<Middleware>(default_handler);
  middleware->add(DefaultMiddleware::logging_middleware);
}

net::awaitable<void> HttpServer::handle_client(HttpContext ctx) {
  beast::flat_buffer buffer;
  try {
    // Set TCP_NODELAY to disable Nagle's algorithm and reduce latency
    ctx.socket.set_option(tcp::no_delay(true));

    for (;;) {
      // Clear the request/response for the next iteration
      ctx.request = HttpRequest();
      ctx.response = HttpResponse();

      co_await http::async_read(ctx.socket, buffer, ctx.request.base(),
                                net::use_awaitable);

      co_await middleware->next(ctx);

      co_await http::async_write(ctx.socket, ctx.response.base(),
                                 net::use_awaitable);

      if (!ctx.response.keep_alive()) {
        break;
      }
    }

    ctx.socket.shutdown(tcp::socket::shutdown_send);
  } catch (const boost::system::system_error &e) {
    if (e.code() != http::error::end_of_stream) {
      std::cerr << "System error in handle_client: " << e.what() << std::endl;
    }
  } catch (const std::exception &e) {
    std::cerr << "Error in handle_client: " << e.what() << std::endl;
  }
}

void HttpServer::setAddress(const std::string &address) {
  this->address = address;
}

void HttpServer::setPort(uint16_t port) { this->port = port; }

void HttpServer::setThreads(uint8_t threads) { this->threads = threads; }

void HttpServer::start() {
  try {
    net::io_context io_context{threads};
    auto executor = io_context.get_executor();

    // Spawn the acceptor coroutine
    net::co_spawn(
        executor,
        [this, executor]() -> net::awaitable<void> {
          tcp::acceptor acceptor(executor,
                                 {net::ip::make_address(address), port});
          std::cout << "Server listening on " << address << ":" << port << " ("
                    << (int)threads << " threads)..." << std::endl;

          for (;;) {
            auto socket = co_await acceptor.async_accept(net::use_awaitable);
            net::co_spawn(executor,
                          this->handle_client(HttpContext(std::move(socket))),
                          net::detached);
          }
        },
        net::detached);

    // Run the io_context in a thread pool
    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for (auto i = threads - 1; i > 0; --i) {
      v.emplace_back([&io_context] { io_context.run(); });
    }
    io_context.run();

    for (auto &t : v) {
      t.join();
    }
  } catch (const std::exception &e) {
    std::cerr << "Error in server start: " << e.what() << std::endl;
  }
}