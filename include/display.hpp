/**
 * @file display.hpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 *
 * @copyright Copyright © 2020-2021
 */

#ifndef DISPLAY_HPP_
#define DISPLAY_HPP_

#include "display_config.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <thread>

class Winch;
class InputType;


class DisplaySH1106_128x64_I2C;

template <uint32_t W, uint32_t H, uint8_t BPP>
class NanoCanvas;

template<class C, class D> class NanoEngine;

template <uint8_t BPP> class NanoCanvasOps;

template <class D>
using NanoEngine1 = NanoEngine<NanoCanvas<16, 16, 1>, D>;

extern const uint8_t free_SLANT12x10[];

class GuiType {
 public:
  enum ValueGuiType : uint8_t {
    DISABLE = 0,
    SH1106_I2C = 1,
    VGA = 100,
    CAPTURE = 101
  };

  explicit operator bool() = delete;        // Prevent usage: if(value)
  constexpr GuiType(const ValueGuiType& v) : value{v} {}  // not explicit here.
  constexpr operator ValueGuiType() const { return value; }
  constexpr GuiType& operator=(ValueGuiType v) { value = v; return *this; }
  constexpr bool operator==(const ValueGuiType v) const { return value == v; }
  constexpr bool operator!=(const ValueGuiType v) const { return value != v; }

  operator std::string() const {
    std::string result("No available");

    switch (value) {
      case SH1106_I2C:  result = "SH1106_I2C"; break;
      case VGA:         result = "VGA"; break;
      case CAPTURE:     result = "CAPTURE"; break;
      case DISABLE:
      default:
          result = "DISABLE";
    }

    return result;
  }

  static GuiType valueof(const std::string& str) {
      if (str == "SH1106_I2C")  return SH1106_I2C;
      if (str == "VGA")         return VGA;
      if (str == "CAPTURE")     return CAPTURE;
      if (str == "DISABLE")     return DISABLE;

      return DISABLE;
  }

 private:
  ValueGuiType value;
  GuiType() = default;
};

class ScreenBase;
class Logger;

class Gui {
 public:
  ScreenBase* screen = nullptr;
  int16_t cursor_pos = 1;
  uint8_t view_pos = 0;

  explicit Gui(Winch*);
  virtual ~Gui();
  Winch* getWinch();
  void boot();
  void display();
  uint8_t getPos();
  void enter(InputType);
  void statusBar(NanoCanvasOps<1>*);
  void createValue(NanoCanvasOps<1>*, std::string, std::string);
  void createMenuScroll(NanoCanvasOps<1>*, std::vector<std::string>, std::string);
  void createMenuIcon(NanoCanvasOps<1>*, std::vector<std::string>);
  void extractScreen();

 private:
  Logger *logger = nullptr;
  Winch* winch = nullptr;
  
  DisplaySH1106_128x64_I2C* device = nullptr;
  NanoEngine1<DisplaySH1106_128x64_I2C>* engine = nullptr;
  NanoCanvas<128, 64, 1>* draw = nullptr;
  std::thread display_draw_Loop;

  void drawBoot();
  void draw_loop();
};

class ScreenBase {
 public:
  explicit ScreenBase(Gui*);
  virtual ~ScreenBase() = default;
  virtual uint8_t countItems() = 0;
  virtual void display(NanoCanvasOps<1>*) = 0;
  virtual void enter(InputType, uint8_t) = 0;

 protected:
  Gui* gui = nullptr;
  Winch* winch = nullptr;
  std::string title;
  bool inver = false;
  explicit ScreenBase(Gui*, std::string);
  void selector_value(NanoCanvasOps<1>*);
  void go_back();
};

class MainScreen: public ScreenBase {
 public:
  explicit MainScreen(Gui*);
  uint8_t countItems() override;
  void display(NanoCanvasOps<1>*) override;
  void enter(InputType, uint8_t) override;

 private:
  std::vector<std::string> ITEMS_IDLE {u8"1", u8"2", "3"}; // { u8"", u8"", " " };
  std::vector<std::string> ITEMS_RUNNING {u8"4", u8"5", "6"}; // { u8"", u8"", " " };
  std::vector<std::string> ITEMS_ERROR {u8"7", u8"8", "9"}; // { u8"", u8"", u8"" };
  uint8_t count = 1;
};

class MenuScreen: public ScreenBase {
 public:
  explicit MenuScreen(Gui*);
  uint8_t countItems() override;
  void display(NanoCanvasOps<1>*) override;
  void enter(InputType, uint8_t) override;

 private:
  std::vector<std::string> ITEMS_MENU = {
        ITEM_BACK,
        "Manual position",
        "Security distance",
        "Mode selector",
        "Velocity Start",
        "Velocity Stop"};
};

class ManualPositionScreen: public ScreenBase {
 public:
  explicit ManualPositionScreen(Gui*);
  uint8_t countItems() override;
  void display(NanoCanvasOps<1>*) override;
  void enter(InputType, uint8_t) override;

 private:
  int16_t cursor_pos = 0;
};

class SecurityDistanceScreen: public ScreenBase {
 public:
  explicit SecurityDistanceScreen(Gui*);
  uint8_t countItems() override;
  void display(NanoCanvasOps<1>*) override;
  void enter(InputType, uint8_t) override;

 private:
 protected:
};

class ModeSelectorScreen: public ScreenBase {
 public:
  explicit ModeSelectorScreen(Gui*);
  uint8_t countItems() override;
  void display(NanoCanvasOps<1>*) override;
  void enter(InputType, uint8_t) override;

 private:
  std::vector<std::string> ITEMS_MENU = {
        ITEM_BACK,
        "OneWay",
        "TwoWay"
  };
 protected:
};

class VelocityStartScreen: public ScreenBase {
 public:
  explicit VelocityStartScreen(Gui*);
  uint8_t countItems() override;
  void display(NanoCanvasOps<1>*) override;
  void enter(InputType, uint8_t) override;

 private:
 protected:
};

class VelocityStopScreen: public ScreenBase {
 public:
  explicit VelocityStopScreen(Gui*);
  uint8_t countItems() override;
  void display(NanoCanvasOps<1>*) override;
  void enter(InputType, uint8_t) override;

 private:
 protected:
};

#endif  // DISPLAY_HPP_
