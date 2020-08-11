/*
  input.h - OpenWinch Project

  Copyright (C) 2020  Mickael Gaillard
*/

#ifndef INPUT_HPP_
#define INPUT_HPP_

class InputType {
 public:
  enum Value {
    UP = 1,
    RIGHT = 2,
    DOWN = 3,
    LEFT = 4,
    ENTER = InputType::DOWN,
  };
};

#endif  // INPUT_HPP_