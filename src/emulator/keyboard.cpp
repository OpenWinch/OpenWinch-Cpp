/**
 * @file controller.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020-2021
 */

#include "openwinch.hpp"
#include "input.hpp"

#include <iostream>

Keyboard::Keyboard(Winch* _winch) : winch(_winch) {
  this->run();
}

void Keyboard::runLoop() {
  auto input = InputType::NONE;

  BufferToggle bt;
  bt.off();

  while (this->isNotAbort()) {
    input = this->get();
    this->winch->enterGui(input);
  }

  bt.on();
}

InputType Keyboard::get() {
  InputType result = InputType::NONE;
  auto c = std::getchar();

  if (c == 27) {
    c = std::getchar();

    if (c == '[') {
      c = std::getchar();

      if (c == 'A') { //'\x1b[A') {
        result = InputType::UP;
      } else
      
      if (c == 'B') {
        result = InputType::DOWN;
      } else
      
      if (c == 'C') {
        result = InputType::RIGHT;
      } else
      
      if (c == 'D') {
        result = InputType::LEFT;
      }
    }
  }

  // if (result == nullptr) {
     //std::cout << "Not an arrow key!" << std::endl;
  // }

  return result;
}