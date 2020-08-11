/*
  input.h - OpenWinch Project

  Copyright (C) 2020  Mickael Gaillard
*/

#ifndef INPUT_HPP_
#define INPUT_HPP_

class InputType {
 public:
  enum ValueInputType {
    UP = 1,
    RIGHT = 2,
    DOWN = 3,
    LEFT = 4,
    ENTER = InputType::DOWN,
  };

  InputType() = default;
  explicit constexpr InputType(ValueInputType aValue) : value(aValue) { }

  constexpr bool operator==(InputType a) const { return value == a.value; }
  constexpr bool operator!=(InputType a) const { return value != a.value; }

  ModeType list();

 private:
  ValueInputType value;

};

#endif  // INPUT_HPP_