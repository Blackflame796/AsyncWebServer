#include <Middleware.hpp>

net::awaitable<void> Middleware::next(HttpContext &ctx)
{
    if (index < middlewares.size())
    {
        auto &middleware = middlewares[index++];
        co_await middleware(ctx, *this);
    }
    else
    {
        co_await final_handler(ctx);
    }
}

void Middleware::add(MiddlewareFunc func) { middlewares.push_back(func); }

net::awaitable<void> DefaultMiddleware::logging_middleware(HttpContext &ctx,
                                            Middleware &middleware)
    {
        std::cout << "[LOG] Request: " << ctx.request.method_string() << " "
                  << ctx.request.target() << std::endl;
        auto start = std::chrono::steady_clock::now();

        co_await middleware.next(ctx);

        auto end = std::chrono::steady_clock::now();
        auto diff =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "[LOG] Response: " << ctx.response.result_int()
                  << " (Time: " << diff.count() << "ms)" << std::endl;
    }