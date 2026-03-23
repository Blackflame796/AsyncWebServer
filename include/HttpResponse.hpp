#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <Base.hpp>

class HttpResponse {
public:
  HttpResponse() { res_.version(11); }
  HttpResponse(http::response<http::string_body> res) : res_(std::move(res)) {}

  void status(http::status s) { res_.result(s); }
  void body(std::string b) { res_.body() = std::move(b); }
  const std::string &body() const { return res_.body(); }

  void set(http::field f, std::string_view v) { res_.set(f, v); }

  void prepare_payload() { res_.prepare_payload(); }
  void keep_alive(bool k) { res_.keep_alive(k); }
  bool keep_alive() const { return res_.keep_alive(); }

  int result_int() const { return static_cast<int>(res_.result()); }

  const http::response<http::string_body> &base() const { return res_; }
  http::response<http::string_body> &base() { return res_; }

private:
  http::response<http::string_body> res_;
};

#endif
