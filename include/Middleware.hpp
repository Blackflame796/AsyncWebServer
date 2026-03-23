#ifndef MIDDLEWARE_HPP
#define MIDDLEWARE_HPP
#include <Base.hpp>
#include <HttpContext.hpp>

class Middleware;
using MiddlewareFunc =
    std::function<net::awaitable<void>(HttpContext &, Middleware &, size_t)>;

class Middleware
{
public:
    Middleware(Handler handler)
        : final_handler(handler) {}
    net::awaitable<void> next(HttpContext &ctx, size_t index = 0);
    void add(MiddlewareFunc func);

private:
    std::vector<MiddlewareFunc> middlewares;
    Handler final_handler;
};

namespace DefaultMiddleware
{
    net::awaitable<void> logging_middleware(HttpContext &ctx, Middleware &middleware, size_t index);
}

#endif