#include <HttpServer.hpp>

int main()
{
  HttpServer server;
  server.setAddress("0.0.0.0");
  server.setPort(8080);
  server.setThreads(4);
  server.start();
  return 0;
}