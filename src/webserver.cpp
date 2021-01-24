/**
 * @file webserver.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020-2021
 */

#include "openwinch.hpp"
#include "webserver.hpp"

#include "mode.hpp"

#include <string>
#include <fstream>
#include <streambuf>
#include <mutex>

#include <httplib.h>

#define SERVER_CERT_FILE "./cert.pem"
#define SERVER_PRIVATE_KEY_FILE "./key.pem"

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
#define PORT 443
#else
#define PORT 80
#endif


WebServer::WebServer(Winch* _winch) {
  this->logger = &Logger::get();
  this->logger->debug("WEB : Initialize web server stack...");

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
  this->srv = new httplib::SSLServer(SERVER_CERT_FILE, SERVER_PRIVATE_KEY_FILE);
#else
  this->srv = new httplib::Server();
#endif

  if (!this->srv->is_valid()) {
    this->logger->error("WEB : Server has an error...");
  }

  this->srv->set_keep_alive_max_count(HTTP_KEEPALIVE_MAX);
  this->srv->set_read_timeout(5, 0);        // 5 seconds
  this->srv->set_write_timeout(5, 0);       // 5 seconds
  this->srv->set_idle_interval(0, 100000);  // 100 milliseconds
  this->srv->set_payload_max_length(HTTP_PAYLOAD_MAX);

  this->setErrorHandler();
  this->setLogger();
  this->setStaticFile();

  this->logger->debug("WEB : Initialize routing...");
  WebMain main(this, _winch);
  WebExtra extra(this, _winch);
}

WebServer::~WebServer() {
  this->srv->stop();

  delete this->srv;
}

void WebServer::setErrorHandler() {
  this->logger->debug("WEB : Initialize error handler...");

  this->srv->set_error_handler(
      [this](const httplib::Request & /*req*/, httplib::Response &res) {
    const char *fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
    char buf[BUFSIZ];
    snprintf(buf, sizeof(buf), fmt, res.status);
    res.set_content(buf, "text/html");

    this->logger->error(buf);
  });
}
void WebServer::setLogger() {
  this->logger->debug("WEB : Initialize logger...");

  this->srv->set_logger(
      [this](const httplib::Request &req, const httplib::Response &/*res*/) {
    // this->logger->debug("WEB : %s", log(req, res).c_str());
    this->logger->debug("WEB : %s %s", req.method.c_str(), req.path.c_str());
  });
}
void WebServer::setStaticFile() {
  this->logger->debug("WEB : Initialize Static content...");

  bool ret = this->srv->set_mount_point("/css", "./public/css");
  if (!ret) { throw ret; }
  
  ret = this->srv->set_mount_point("/img", "./public/img");
  if (!ret) { throw ret; }
}

httplib::Server* WebServer::getServer() {
    return this->srv;
}

void WebServer::run() {
  this->logger->info("WEB : Start listener & thread stack...");
  this->srv->listen("0.0.0.0", PORT);
}

#ifdef OW_WEB_DEBUG

// std::string WebServer::log(const httplib::Request &req, const httplib::Response &res) {
//   std::string s;
//   char buf[BUFSIZ];

//   s += "================================\n";
//   snprintf(buf, sizeof(buf), "%s %s %s",
//             req.method.c_str(),
//             req.version.c_str(),
//             req.path.c_str());
//   s += buf;

//   s += "#################################\n";
//   std::string query;
//   for (auto it = req.params.begin(); it != req.params.end(); ++it) {
//     const auto &x = *it;
//     snprintf(buf, sizeof(buf), "%c%s=%s",
//              (it == req.params.begin()) ? '?' : '&', x.first.c_str(),
//              x.second.c_str());
//     query += buf;
//   }
//   snprintf(buf, sizeof(buf), "%s\n", query.c_str());
//   s += buf;
//   s += WebServer::dump_headers(req.headers);
//   s += "--------------------------------\n";

//   snprintf(buf, sizeof(buf), "%d %s\n", res.status, res.version.c_str());
//   s += buf;
//   s += WebServer::dump_headers(res.headers);
//   s += "\n";

//   // if (!res.body.empty()) { s += res.body; }
//   // s += "\n";

//   return s;
// }

// std::string WebServer::dump_headers(const httplib::Headers &headers) {
//   std::string s("HEADER :\n***************************\n");
//   char buf[BUFSIZ];

//   for (auto it = headers.begin(); it != headers.end(); ++it) {
//     const auto &x = *it;
//     snprintf(buf, sizeof(buf), "%s: %s\n", x.first.c_str(), x.second.c_str());
//     s += buf;
//   }

//   return s;
// }

#endif  // OW_WEB_DEBUG

void findAndReplaceAll(std::string & data, std::string toSearch, std::string replaceStr)
{
    // Get the first occurrence
    size_t pos = data.find(toSearch);
    // Repeat till end is reached
    while( pos != std::string::npos)
    {
        // Replace this occurrence of Sub String
        data.replace(pos, toSearch.size(), replaceStr);
        // Get the next occurrence from the current position
        pos =data.find(toSearch, pos + replaceStr.size());
    }
}

WebMain::WebMain(WebServer* _server, Winch* _winch) : srv(_server->getServer()), winch(_winch) {
  auto winch = this->winch;

  this->srv->Get(HTTP_ROUTE_ROOT,
    [this](const httplib::Request &/*req*/, httplib::Response &res) {
      res.set_redirect(HTTP_ROUTE_INDEX);
  });

  this->srv->Get(HTTP_ROUTE_INDEX, 
    [winch](const httplib::Request &req, httplib::Response &res) {
      std::string content(index_html);
      const auto host = req.headers.find("HOST");

      auto enable = "white";
      if (winch->getState().isRun()) {
        enable = "lime";
      } else if (winch->getState() == State::ERROR) {
        enable = "red";
      } else if (winch->getState() == State::UNKNOWN or winch->getState() == State::INIT) {
        enable = "orange";
      }

      findAndReplaceAll(content, TPL_URL,  "http://" + host->second + HTTP_ROUTE_INDEX);
      findAndReplaceAll(content, TPL_MODE, std::string(winch->getMode()).c_str());
      findAndReplaceAll(content, TPL_BAT, std::to_string(winch->getBattery()));
      findAndReplaceAll(content, TPL_ENABLE, enable);
      findAndReplaceAll(content, TPL_SPD_TARGET, std::to_string(winch->getSpeedTarget()));
      findAndReplaceAll(content, TPL_SPD_UNIT, SPEED_UNIT);
      findAndReplaceAll(content, TPL_VERSION, VERSION);

      res.set_content(content.c_str(), content.size(), "text/html");
  });

  this->srv->Get(HTTP_ROUTE_START,
    [winch](const httplib::Request &/*req*/, httplib::Response &res) {
      winch->start();
      res.set_redirect(HTTP_ROUTE_INDEX);
  });

  this->srv->Get(HTTP_ROUTE_STOP,
    [winch](const httplib::Request &/*req*/, httplib::Response &res) {
      winch->stop();
      res.set_redirect(HTTP_ROUTE_INDEX);
  });

  this->srv->Get(HTTP_ROUTE_UP,
    [winch](const httplib::Request &/*req*/, httplib::Response &res) {
      winch->speedUp();
      res.set_redirect(HTTP_ROUTE_INDEX);
  });

  this->srv->Get(HTTP_ROUTE_DOWN,
    [winch](const httplib::Request &/*req*/, httplib::Response &res) {
      winch->speedDown();
      res.set_redirect(HTTP_ROUTE_INDEX);
  });

  this->srv->Get(HTTP_ROUTE_HALT,
    [winch](const httplib::Request &/*req*/, httplib::Response &res) {
      winch->emergency();
      res.set_redirect(HTTP_ROUTE_INDEX);
  });
}

WebExtra::WebExtra(WebServer* _server, Winch* _winch) : srv(_server->getServer()), winch(_winch) {
  auto winch = this->winch;

  this->srv->Get(HTTP_ROUTE_EXTRA,
    [winch](const httplib::Request &req, httplib::Response &res) {
      std::string content(extra_html);
      const auto host = req.headers.find("HOST");

      findAndReplaceAll(content, TPL_URL, "http://" + host->second + HTTP_ROUTE_EXTRA);
      findAndReplaceAll(content, TPL_MODE, std::string(winch->getMode()).c_str());
      findAndReplaceAll(content, TPL_BAT, std::to_string(winch->getBattery()));
      findAndReplaceAll(content, TPL_ENABLE, "white");
      findAndReplaceAll(content, TPL_SPD_TARGET, std::to_string(winch->getSpeedTarget()));
      findAndReplaceAll(content, TPL_SPD_UNIT, SPEED_UNIT);
      findAndReplaceAll(content, TPL_VERSION, VERSION);

      res.set_content(content.c_str(), content.size(), "text/html");
  });

  this->srv->Get(HTTP_ROUTE_RESET,
    [winch](const httplib::Request &/*req*/, httplib::Response &res) {
      winch->initialize();
      res.set_redirect(HTTP_ROUTE_EXTRA);
  });

  this->srv->Get(HTTP_ROUTE_LEFT,
    [this](const httplib::Request &/*req*/, httplib::Response &res) {
      res.set_redirect(HTTP_ROUTE_EXTRA);
  });

  this->srv->Get(HTTP_ROUTE_RIGHT,
    [this](const httplib::Request &/*req*/, httplib::Response &res) {
      res.set_redirect(HTTP_ROUTE_EXTRA);
  });
}
