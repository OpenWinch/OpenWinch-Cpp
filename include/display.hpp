/**
 * @file display.hpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
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
    switch (value) {
      case DISABLE:     return "DISABLE";
      case SH1106_I2C:  return "SH1106_I2C";
      case VGA:         return "VGA";
      case CAPTURE:     return "CAPTURE";
    }
  }

  static GuiType valueof(const std::string& str) {
      if (str == "DISABLE")         return DISABLE;
      else if (str == "SH1106_I2C") return SH1106_I2C;
      else if (str == "VGA")        return VGA;
      else if (str == "CAPTURE")    return CAPTURE;
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
  uint8_t cursor_pos = 1;
  uint8_t view_pos = 0;

  explicit Gui(Winch*);
  Winch* getWinch();
  void boot();
  void exit();
  void display();
  uint8_t getPos();
  void enter(InputType);
  void statusBar(NanoCanvasOps<1>*);
  void createValue(NanoCanvasOps<1>*, const char*, const char*);
  void createMenuScroll(NanoCanvasOps<1>*, std::vector<std::string>, std::string);
  void createMenuIcon(NanoCanvasOps<1>*, const char* items);
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
  virtual uint8_t countItems() = 0;
  virtual void display(NanoCanvasOps<1>*) = 0;
  virtual void enter(uint8_t) = 0;

 protected:
  Gui *gui = nullptr;
  Winch *winch = nullptr;
  bool inver = false;
};

class MainScreen: public ScreenBase {
 public:
  explicit MainScreen(Gui*);
  uint8_t countItems() override;
  void display(NanoCanvasOps<1>*) override;
  void enter(uint8_t) override;

 private:
  const char *ITEMS_IDLE = " ";
  const char *ITEMS_RUNNING = " ";
  const char *ITEMS_ERROR = "";
  uint8_t count = 1;
};

class MenuScreen: public ScreenBase {
 public:
  explicit MenuScreen(Gui*);
  uint8_t countItems() override;
  void display(NanoCanvasOps<1>*) override;
  void enter(uint8_t) override;

 private:
  std::vector<std::string> ITEMS_MENU = {
        "Back",  // ITEM_BACK,
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
  void enter(uint8_t) override;

 private:
};

class SecurityDistanceScreen: public ScreenBase {
 public:
  explicit SecurityDistanceScreen(Gui*);
  uint8_t countItems() override;
  void display(NanoCanvasOps<1>*) override;
  void enter(uint8_t) override;

 private:
};

class ModeSelectorScreen: public ScreenBase {
 public:
  explicit ModeSelectorScreen(Gui*);
  uint8_t countItems() override;
  void display(NanoCanvasOps<1>*) override;
  void enter(uint8_t) override;

 private:
};

class VelocityStartScreen: public ScreenBase {
 public:
  explicit VelocityStartScreen(Gui*);
  uint8_t countItems() override;
  void display(NanoCanvasOps<1>*) override;
  void enter(uint8_t) override;

 private:
};

class VelocityStopScreen: public ScreenBase {
 public:
  explicit VelocityStopScreen(Gui*);
  uint8_t countItems() override;
  void display(NanoCanvasOps<1>*) override;
  void enter(uint8_t) override;

 private:
  std::string TITLE = "Velocity Stop";
  uint8_t value = 10;
};

#endif  // DISPLAY_HPP_
