/*
  display.h - OpenWinch Project

  Copyright (C) 2020  Mickael Gaillard
*/

#ifndef DISPLAY_HPP_
#define DISPLAY_HPP_

#include <cstdint>

class Winch;
class InputType;

class GuiType {
 public:
  enum ValueGuiType : uint8_t {
    DISABLE = 0,
    SH1106_I2C = 1,
    VGA = 100,
    CAPTURE = 101
  };

  explicit operator bool() = delete;        // Prevent usage: if(value)
  constexpr GuiType(const ValueGuiType& v) : value{v} {} //not explicit here.
  constexpr operator ValueGuiType() const { return value; }
  constexpr GuiType& operator=(ValueGuiType v) { value = v; return *this;}
  constexpr bool operator==(const ValueGuiType v) const { return value == v; }
  constexpr bool operator!=(const ValueGuiType v) const { return value != v; }

 private:
  ValueGuiType value;
  GuiType() = default;

};

class Gui {
 public:
  Gui(Winch*);
  void boot();
  void enter(InputType);
};

class ScreenBase {

};

class MainScreen: public ScreenBase {

};

#endif  // DISPLAY_HPP_