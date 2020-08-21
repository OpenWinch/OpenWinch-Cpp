/**
 * @file mode.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#include <iostream>

#include "controller.hpp"
#include "webserver.hpp"

#ifdef DEBUG
#include "tachometer.hpp"
#include "logger.hpp"
#endif

int main(int argc, char *argv[])  {
  int j = 0;

  // Start Winch engine.
  Winch *winch = &Winch::get();

  // Debug
#ifdef DEBUG
  Logger *logger = &Logger::get();
  Tachometer *Tachometer = &Tachometer::get();
#endif

  // Start Web stack.
  WebServer srv;
  srv.run();


  // clock_t tic = clock();
  std::cout << "Start.." << std::endl;
  // httplib::Server svr;

  // if (!svr.is_valid()) {
  //   printf("server has an error...\n");
  //   return -1;
  // }

  // svr.Get("/", [=](const httplib::Request & /*req*/, httplib::Response &res) {
  //   res.set_redirect("/hi");
  // });

  // svr.Get("/hi", [](const httplib::Request & /*req*/, httplib::Response &res) {
  //   res.set_content("Hello World!\n", "text/plain");
  // });

  // svr.Get("/slow", [](const httplib::Request & /*req*/, httplib::Response &res) {
  //   std::this_thread::sleep_for(std::chrono::seconds(2));
  //   res.set_content("Slow...\n", "text/plain");
  // });

  // // svr.Get("/dump", [](const httplib::Request &req, httplib::Response &res) {
  // //   res.set_content(dump_headers(req.headers), "text/plain");
  // // });

  // svr.Get("/stop",
  //         [&](const httplib::Request & /*req*/, httplib::Response & /*res*/) { svr.stop(); });

  // // svr.set_error_handler([](const httplib::Request & /*req*/, httplib::Response &res) {
  // //   const char *fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
  // //   char buf[BUFSIZ];
  // //   snprintf(buf, sizeof(buf), fmt, res.status);
  // //   res.set_content(buf, "text/html");
  // // });

  // // svr.set_logger([](const httplib::Request &req, const httplib::Response &res) {
  // //   printf("%s", log(req, res).c_str());
  // // });

  // svr.listen("0.0.0.0", 80);

  while (1) {
    ++j;

    // clock_t toc = clock();
    // double delta = (int)(toc - tic);
    //printf("d=%f", delta);
    // if ( (delta / CLOCKS_PER_SEC) >= 1.0 ) {
    //   tic = clock();
      // tacho_hallSensor_t hallU = tacho_get_hall_sensorU();
      // printf("CPU: %dkHz  GPIO : %dHz \n", j/1000, i);
      // printf("U: %d  W: %d  V: %d \n",
      //           tacho_get_rpm(hallU.pulseTime),
      //           tacho_get_rpm(tacho_get_hall_sensorW().pulseTime),
      //           tacho_get_rpm(tacho_get_hall_sensorV().pulseTime));
      // tacho_hall_debug(hallU);
    //   i = 0;
    //   j = 0;
    // }
    std::this_thread::sleep_for(std::chrono::seconds(5));
  }

  return 0;
}
