/*
  webserver.h - OpenWinch Project

  Copyright (C) 2020  Mickael Gaillard
*/

#ifndef WEBSERVER_HPP_
#define WEBSERVER_HPP_

#include <string>

namespace httplib {
  class SSLServer;
  class Server;
//  class Headers;
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
  static std::string log(const httplib::Request&, const httplib::Response&);
  //static std::string dump_headers(const httplib::Headers &headers);

};

class WebMain {
 public:
  WebMain(WebServer*);

 private:
  WebServer* server = nullptr;
  httplib::Server* srv = nullptr;
};

class WebExtra {
 public:
  WebExtra(WebServer*);

 private:
  WebServer* server = nullptr;
  httplib::Server* srv = nullptr;

};

#endif  // WEBSERVER_HPP_