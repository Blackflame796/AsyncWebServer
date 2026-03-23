#ifndef MIDDLEWARE_HPP
#define MIDDLEWARE_HPP
#include <Base.hpp>
#include <HttpContext.hpp>

class Middleware;
using MiddlewareFunc =
    std::function<net::awaitable<void>(HttpContext &, Middleware &)>;

class Middleware
{
public:
    Middleware(Handler handler)
        : final_handler(handler) {}
    net::awaitable<void> next(HttpContext &ctx);
    void add(MiddlewareFunc func);

private:
    std::vector<MiddlewareFunc> middlewares;
    Handler final_handler;
    size_t index = 0;
};

namespace DefaultMiddleware
{
    net::awaitable<void> logging_middleware(HttpContext &ctx, Middleware &middleware);
}

#endif