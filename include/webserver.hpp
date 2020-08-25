/**
 * @file webserver.hpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#ifndef WEBSERVER_HPP_
#define WEBSERVER_HPP_

#include <string>
#include <map> 

#define TPL_URL         "{{ request.url_root }}"
#define TPL_MODE        "{{ mode }}"
#define TPL_BAT         "{{ battery }}"
#define TPL_ENABLE      "{{ enable }}"
#define TPL_SPD_TARGET  "{{ speed_target }}"
#define TPL_SPD_UNIT    "{{ speed_unit }}"
#define TPL_VERSION     "{{ app_version }}"


extern const std::string index_html;
extern const std::string extra_html;

namespace httplib {

  namespace detail {
  struct ci;
  }

class SSLServer;
class Server;
class Request;
class Response;

using Headers = std::multimap<std::string, std::string, detail::ci>;
}

class Logger;


class WebServer {
 public:
  WebServer();
  ~WebServer();
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
  static std::string dump_headers(const httplib::Headers &headers);
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
