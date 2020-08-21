/*
  webserver.h - OpenWinch Project

  Copyright (C) 2020  Mickael Gaillard
*/

#ifndef WEBSERVER_HPP_
#define WEBSERVER_HPP_

#include <string>

extern const std::string index_html;
extern const std::string extra_html;

namespace httplib {
class SSLServer;
class Server;
// class Headers;
class Request;
class Response;
}

class Logger;


class WebServer {
 public:
  WebServer();
  void run();
  httplib::Server* getServer();

 private:
  Logger *logger = nullptr;
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
  httplib::SSLServer* srv = nullptr;
#else
  httplib::Server* srv = nullptr;
#endif
  void setErrorHandler();
  void setLogger();
  void setStaticFile();
  static std::string log(const httplib::Request&, const httplib::Response&);
  // static std::string dump_headers(const httplib::Headers &headers);
};

class WebMain {
 public:
  explicit WebMain(WebServer*);

 private:
  WebServer* server = nullptr;
  httplib::Server* srv = nullptr;
};

class WebExtra {
 public:
  explicit WebExtra(WebServer*);

 private:
  WebServer* server = nullptr;
  httplib::Server* srv = nullptr;
};

#endif  // WEBSERVER_HPP_
