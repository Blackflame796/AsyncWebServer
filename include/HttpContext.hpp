#ifndef HTTP_CONTEXT_HPP
#define HTTP_CONTEXT_HPP
#include <Base.hpp>
#include <HttpRequest.hpp>
#include <HttpResponse.hpp>

struct HttpContext {
  tcp::socket socket;
  HttpRequest request;
  HttpResponse response;

  HttpContext(tcp::socket sock) : socket(std::move(sock)) {}
};

using Handler = std::function<net::awaitable<void>(HttpContext &)>;

#endif // HTTP_CONTEXT_HPP