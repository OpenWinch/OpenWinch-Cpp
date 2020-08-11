/*
  hardware.h - OpenWinch Project

  Copyright (C) 2020  Mickael Gaillard
*/

#ifndef HARDWARE_HPP_
#define HARDWARE_HPP_

class SpeedMode {
 public:
  enum Value {
    LOW = 0,
    MEDIUM = 1,
    HIGH = 2
  };
};

class Board {

};

class Emulator: public Board {

};


#endif  // HARDWARE_HPP_