/**
 * @file input.hpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020-2021
 */

#ifndef INPUT_HPP_
#define INPUT_HPP_

#include <cstdint>
#include <string>
#include <thread>
#include <termios.h>
#include <unistd.h>

class InputType {
 public:
  enum ValueInputType : uint8_t {
    UP = 1,
    RIGHT = 2,
    DOWN = 3,
    LEFT = 4,
    ENTER = InputType::DOWN,
    NONE = 255,
  };

  explicit operator bool() = delete;        // Prevent usage: if(value)
  constexpr InputType(const ValueInputType& v) : value{v} {} //not explicit here.
  constexpr operator ValueInputType() const { return value; }
  constexpr InputType& operator=(ValueInputType v) { value = v; return *this;}
  constexpr bool operator==(const ValueInputType v) const { return value == v; }
  constexpr bool operator!=(const ValueInputType v) const { return value != v; }

  operator std::string() const {
    std::string result("No available");

    switch (value) {
      case UP:    result = "UP"; break;
      case RIGHT: result = "RIGHT"; break;
      case DOWN:  result = "DOWN"; break;
      case LEFT:  result = "LEFT"; break;
      //case ENTER: result = "ENTER"; break;
      case NONE:
      default:
        result = "NONE";
    }

    return result;
  }

  InputType list();

 private:
  ValueInputType value;
  InputType() = default;
};

class Winch;
class Keyboard {
 public:
  Keyboard(Winch *);
 private:
  Winch* winch = nullptr;
  std::thread* controlLoop = nullptr;
  void runControlLoop();
  InputType get();
};

class BufferToggle
{
    private:
        struct termios t;

    public:

        /*
         * Disables buffered input
         */

        void off(void)
        {
            tcgetattr(STDIN_FILENO, &t); //get the current terminal I/O structure
            t.c_lflag &= ~ICANON; //Manipulate the flag bits to do what you want it to do
            tcsetattr(STDIN_FILENO, TCSANOW, &t); //Apply the new settings TSCANOW
        }


        /*
         * Enables buffered input
         */

        void on(void)
        {
            tcgetattr(STDIN_FILENO, &t); //get the current terminal I/O structure
            t.c_lflag |= ICANON; //Manipulate the flag bits to do what you want it to do
            tcsetattr(STDIN_FILENO, TCSANOW, &t); //Apply the new settings
        }
};

#endif  // INPUT_HPP_