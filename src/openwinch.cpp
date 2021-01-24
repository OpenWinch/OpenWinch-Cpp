/**
 * @file openwinch.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 *
 * @copyright Copyright © 2020-2021
 */

#include "openwinch.hpp"
#include "webserver.hpp"

#include <iostream>
#include <unistd.h>

#define DEBUG

#ifdef DEBUG
#include <thread>
#include "tachometer.hpp"
#include "logger.hpp"

void debug() {
  Logger *logger = &Logger::get();
  Tachometer *tacho = &Tachometer::get();

  while (1) {
    tacho->hall_debug(tacho->get_hall_sensorU(), "U");
    tacho->hall_debug(tacho->get_hall_sensorW(), "W");
    tacho->hall_debug(tacho->get_hall_sensorV(), "V");
    logger->debug("RPM %d", tacho->get_rpm());

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}
#endif

void is_root() {
  uid_t uid = getuid(), euid = geteuid();
  if (uid > 0 || uid != euid) {
    std::cout << "You are not root!" << std::endl;
    exit(-1);
  }
}


int main(int /*argc*/, char *argv[])  {
  std::string argv_str(argv[0]);
  std::string base = argv_str.substr(0, argv_str.find_last_of("/"));
  std::cout << base << std::endl;

  is_root();

  // Start Winch engine.
  Winch *winch = &WinchControl::get();
  winch->boot();

  // Debug
#ifdef DEBUG
  // Logger *logger = &Logger::get();
  // Tachometer *tacho = &Tachometer::get();

  std::thread t(debug);
#endif

  // Start Web stack.
  WebServer srv(winch);
  srv.run();


//   while (1) {

// #ifdef DEBUG
//     tacho->hall_debug(tacho->get_hall_sensorU(), "U");
//     tacho->hall_debug(tacho->get_hall_sensorW(), "W");
//     tacho->hall_debug(tacho->get_hall_sensorV(), "V");
//     logger->debug("%d", tacho->get_rpm());
// #endif

    // ++j;

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
  //   std::this_thread::sleep_for(std::chrono::seconds(1));
  // }

  return 0;
}
