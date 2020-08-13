/**
 * @file mode.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#include <iostream>

// #include <stdio.h>
// #include <string.h>
// #include <stdint.h>
// #include <errno.h>
// #include <stdlib.h>
// #include <time.h>

#include "tachometer.hpp"
#include "logger.hpp"
#include "hardware.hpp"
#include "controller.hpp"

int main(int argc, char *argv[])  {
  int i = 0;
  int j = 0;

  Logger &logger = Logger::get();
  Tachometer &tacho = Tachometer::get();

  Winch *winch = new Winch();
  winch->start();


  

  // clock_t tic = clock();
  std::cout << "Start.." << std::endl;
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

  }

  return 0;
}