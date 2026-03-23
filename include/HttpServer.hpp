#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP
#include <Base.hpp>
#include <HttpContext.hpp>
#include <Middleware.hpp>

class HttpServer {
public:
    HttpServer();
    net::awaitable<void> handle_client(HttpContext ctx);
    void setAddress(const std::string &address);
    void setPort(uint16_t port);
    void setThreads(uint8_t threads);
    void start();
private:
    std::string address = "0.0.0.0";
    uint16_t port = 8080;
    uint8_t threads = 1;
    std::shared_ptr<Middleware> middleware;
};

#endif