#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <Base.hpp>

class HttpRequest {
public:
  HttpRequest() = default;
  HttpRequest(http::request<http::string_body> req) : req_(std::move(req)) {}

  http::verb method() const { return req_.method(); }
  std::string_view method_string() const {
    auto s = req_.method_string();
    return std::string_view{s.data(), s.size()};
  }
  std::string_view target() const {
    auto t = req_.target();
    return std::string_view{t.data(), t.size()};
  }
  const std::string &body() const { return req_.body(); }
  std::string &body() { return req_.body(); }

  bool keep_alive() const { return req_.keep_alive(); }

  const http::request<http::string_body> &base() const { return req_; }
  http::request<http::string_body> &base() { return req_; }

private:
  http::request<http::string_body> req_;
};

#endif