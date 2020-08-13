/**
 * @file hardwarepi.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#include "hardwarepi.hpp"
#include <wiringPi.h>

Raspberrypi::Raspberrypi(Winch *winch) : Board(winch) {

}
