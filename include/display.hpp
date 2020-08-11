/*
  display.h - OpenWinch Project

  Copyright (C) 2020  Mickael Gaillard
*/

#ifndef DISPLAY_HPP_
#define DISPLAY_HPP_

class GuiType {
 public:
  enum Value {
    DISABLE = 0,
    SH1106_I2C = 1,
    VGA = 100,
    CAPTURE = 101
  };
};

class Gui {

};

class ScreenBase {

};

class MainScreen: public ScreenBase {

};

#endif  // DISPLAY_HPP_