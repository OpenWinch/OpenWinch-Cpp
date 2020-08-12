/*
  input.h - OpenWinch Project

  Copyright (C) 2020  Mickael Gaillard
*/

#ifndef INPUT_HPP_
#define INPUT_HPP_

#include <cstdint>

class InputType {
 public:
  enum ValueInputType : uint8_t {
    UP = 1,
    RIGHT = 2,
    DOWN = 3,
    LEFT = 4,
    ENTER = InputType::DOWN,
  };

  explicit operator bool() = delete;        // Prevent usage: if(value)
  constexpr InputType(const ValueInputType& v) : value{v} {} //not explicit here.
  constexpr operator ValueInputType() const { return value; }
  constexpr InputType& operator=(ValueInputType v) { value = v; return *this;}
  constexpr bool operator==(const ValueInputType v) const { return value == v; }
  constexpr bool operator!=(const ValueInputType v) const { return value != v; }

  InputType list();

 private:
  ValueInputType value;
  InputType() = default;

};

#endif  // INPUT_HPP_