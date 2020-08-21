#include "webserver.hpp"
#include "logger.hpp"
#include "controller.hpp"

//#include <string>
#include <httplib.h>

#define SERVER_CERT_FILE "./cert.pem"
#define SERVER_PRIVATE_KEY_FILE "./key.pem"

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
#define PORT 443
#else
#define PORT 80
#endif

WebServer::WebServer() {
  this->logger = &Logger::get();
  this->logger->info("WEB : Initialize web server stack...");

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
  this->srv = new httplib::SSLServer(SERVER_CERT_FILE, SERVER_PRIVATE_KEY_FILE);
#else
  this->srv = new httplib::Server();
#endif

  if (!this->srv->is_valid()) {
    this->logger->error("WEB : Server has an error...");
  }

  this->srv->set_error_handler([](const httplib::Request & /*req*/, httplib::Response &res) {
    const char *fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
    char buf[BUFSIZ];
    snprintf(buf, sizeof(buf), fmt, res.status);
    res.set_content(buf, "text/html");

    Logger::get().error(buf);
  });

  this->srv->set_logger([](const httplib::Request &req, const httplib::Response &res) {
    Logger::get().debug("WEB : %s", log(req, res).c_str());
  });

  this->logger->debug("WEB : Initialize routing...");
  WebMain main(this);
  WebExtra extra(this);
}

httplib::Server* WebServer::getServer() {
    return this->srv;
}

void WebServer::run() {
  this->srv->listen("0.0.0.0", PORT);   
}

std::string WebServer::log(const httplib::Request &req, const httplib::Response &res) {
  std::string s;
  char buf[BUFSIZ];

  s += "================================\n";

  snprintf(buf, sizeof(buf), "%s %s %s", req.method.c_str(),
           req.version.c_str(), req.path.c_str());
  s += buf;

  std::string query;
  for (auto it = req.params.begin(); it != req.params.end(); ++it) {
    const auto &x = *it;
    snprintf(buf, sizeof(buf), "%c%s=%s",
             (it == req.params.begin()) ? '?' : '&', x.first.c_str(),
             x.second.c_str());
    query += buf;
  }
  snprintf(buf, sizeof(buf), "%s\n", query.c_str());
  s += buf;

  //s += dump_headers(req.headers);

  s += "--------------------------------\n";

  snprintf(buf, sizeof(buf), "%d %s\n", res.status, res.version.c_str());
  s += buf;
  //s += dump_headers(res.headers);
  s += "\n";

  if (!res.body.empty()) { s += res.body; }

  s += "\n";

  return s;
}

// std::string WebServer::dump_headers(const Headers &headers) {
//   std::string s;
//   char buf[BUFSIZ];
//
//   for (auto it = headers.begin(); it != headers.end(); ++it) {
//     const auto &x = *it;
//     snprintf(buf, sizeof(buf), "%s: %s\n", x.first.c_str(), x.second.c_str());
//     s += buf;
//   }
//
//   return s;
// }

WebMain::WebMain(WebServer* _server) : server(_server) {
  this->srv = this->server->getServer();

  this->srv->Get("/start", [](const httplib::Request &req, httplib::Response &res) {
    Winch::get().start();
    res.set_content("Start...", "text/plain");
  });

  this->srv->Get("/stop", [](const httplib::Request &req, httplib::Response &res) {
    Winch::get().stop();
    res.set_content("Stop...", "text/plain");
  });

  this->srv->Get("/up", [](const httplib::Request &req, httplib::Response &res) {
    Winch::get().speedUp();
    res.set_content("Up...", "text/plain");
  });

  this->srv->Get("/down", [](const httplib::Request &req, httplib::Response &res) {
    Winch::get().speedDown();
    res.set_content("Down...", "text/plain");
  });

  this->srv->Get("/halt", [](const httplib::Request &req, httplib::Response &res) {
    Winch::get().emergency();
    res.set_content("Halt...", "text/plain");
  });

}

WebExtra::WebExtra(WebServer* _server) : server(_server) {
  this->srv = this->server->getServer();

  this->srv->Get("/extra", [](const httplib::Request &req, httplib::Response &res) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    res.set_content("Extra...\n", "text/plain");
  });

  this->srv->Get("/reset", [](const httplib::Request &req, httplib::Response &res) {
    Winch::get().initialize();
    res.set_content("Reset...\n", "text/plain");
  });

  this->srv->Get("/left", [](const httplib::Request &req, httplib::Response &res) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    res.set_content("Left...\n", "text/plain");
  });

  this->srv->Get("/right", [](const httplib::Request &req, httplib::Response &res) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    res.set_content("Right...\n", "text/plain");
  });

}
