/**
 * @file display.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#include "openwinch.hpp"
#include "display.hpp"

#include "input.hpp"

#include <string>
#include <vector>
#include <cinttypes>

#include "lcdgfx.h"

extern const uint8_t free_SLANT24x17[];
extern const uint8_t free_fontawesome_webfont22x14[];

#define FONT_COLOR_WHITE() draw->setColor(RGB_COLOR16(255, 255, 255))
#define FONT_COLOR_BLACK() draw->setColor(RGB_COLOR16(0, 0, 0))

#define FONT_TYPE_LOGO() draw->setFreeFont(free_SLANT24x17, nullptr)
#define FONT_TYPE_SPEED() draw->setFixedFont(comic_sans_font24x32_123)
#define FONT_TYPE_ICON() draw->setFreeFont(free_fontawesome_webfont22x14, nullptr)
#define FONT_TYPE_DEFAULT() draw->setFixedFont(ssd1306xled_font6x8)

Gui::Gui(Winch *_winch) : winch(_winch) {
  this->logger = &Logger::get();
  this->logger->debug("GUI : Initialize LCD stack...");

  GuiType config = GuiType::valueof(OW_GUI);

  if (GuiType::SH1106_I2C == config) {
    // from luma.core.interface.serial import i2c
    // from luma.oled.device import sh1106

    // serial_interface = i2c(port=1, address=LCD_ADDR)
    this->logger->debug("GUI : Initialize SH1106...");
    ssd1306_enableUtf8Mode2();
    const SPlatformI2cConfig &config = {(-1), (60), (-1), (-1), (0)};
    this->device = new DisplaySH1106_128x64_I2C(-1, config);

    this->logger->debug("GUI : Initialize Engine... (not use)");
    this->engine = new NanoEngine1<DisplaySH1106_128x64_I2C>(*this->device);

    this->logger->debug("GUI : Initialize Canvas...");
    this->draw = new NanoCanvas<128, 64, 1>();
  } else if (GuiType::VGA == config) {
    // from luma.emulator.device import pygame

    //his->device ; // = pygame(width=LCD_WIDTH, height=LCD_HEIGHT, rotate=0, mode='1', transform='scale2x', scale=2, frame_rate=60)
  } else if (GuiType::CAPTURE == config) {
    // from luma.emulator.device import capture

    //this->device ; // = capture(width=LCD_WIDTH, height=LCD_HEIGHT, rotate=0, mode='1', transform='scale2x', scale=2, file_template="docs/images/screens/OpenWinch_{0:06}.png")
  }

  if (this->device != nullptr) {
    this->device->begin();
    this->device->clear();
    this->device->fill(0x00);

    //this->engine->begin();
    //this->engine->setFrameRate(30);
    //this->engine->drawCallback( drawAll );

    //this->draw = this->engine->getCanvas();
  }

//   this->font = ImageFont.truetype(FONT_TEXT, 8)
//   this->regulator = framerate_regulator(fps=LCD_FPS)

  this->screen = new MainScreen(this);
}

Gui::~Gui() {
  if (this->device != nullptr) {
    this->draw->clear();
    this->device->clear();
    
    delete this->draw;
    delete this->engine;
    delete this->device;
  }
}

Winch* Gui::getWinch() {
  return this->winch;
}

void Gui::boot() {
  this->drawBoot();
  this->logger->debug("GUI : Start Thread...");
  this->display_draw_Loop = std::thread(&Gui::draw_loop, this);
}

void Gui::display() {
  if (this->device != nullptr) {
    this->draw->clear();
    this->screen->display(this->draw);
    this->device->drawCanvas(0, 0, *this->draw);
  }
}

uint8_t Gui::getPos() {
  return this->cursor_pos;
}

void Gui::enter(InputType key) {
  // Directional Common
  if (InputType::RIGHT == key) {
    this->logger->debug("GUI : Right");
    this->cursor_pos += 1;
  } else if (InputType::LEFT == key) {
    this->logger->debug("GUI : Left");
    this->cursor_pos -= 1;
  } else if (InputType::ENTER == key) {
    this->logger->debug("GUI : Enter");
    this->screen->enter(this->cursor_pos);
  }

  // out bound fix
  if (this->cursor_pos < 0) {
    this->logger->debug("GUI : Loop from min !");
      if (this->screen->countItems() - (uint8_t)1 > 0) {
        this->cursor_pos = this->screen->countItems() - 1;
      } else {
        this->cursor_pos = 0;
      }
  } else if (this->cursor_pos > this->screen->countItems() - 1) {
    this->logger->debug("GUI : Loop from max !");
    this->cursor_pos = 0;
  }

  this->logger->debug("GUI : Cursor postion : %d", this->cursor_pos);
}

void Gui::statusBar(NanoCanvasOps<1>* draw) {
  // Battery
  uint8_t battery_value = this->winch->getBattery();

  std::string battery_symbol = ""; // U+F244 - 62020
  if (battery_value > 87.5) {
      battery_symbol = "";         // U+F240 - 62016
  } else if (battery_value > 62.5) {
      battery_symbol = "";         // U+F241 - 62017
  } else if (battery_value > 37.5) {
      battery_symbol = "";         // U+F242 - 62018
  } else if (battery_value > 12.5) {
      battery_symbol = "";         // U+F243 - 62019
  }

  uint8_t battery_x = 2;
  FONT_TYPE_ICON();
  FONT_COLOR_WHITE();
  draw->printFixed(battery_x, 0, battery_symbol.c_str(), STYLE_NORMAL);

  char buffer1[256];
  snprintf(buffer1, 255, "%d", battery_value);
  FONT_COLOR_WHITE();
  FONT_TYPE_DEFAULT();
  draw->printFixed(battery_x + 15, 1, buffer1, STYLE_NORMAL);
  // this->logger->debug("GUI : Display battery : %s", buffer1);

  // Wifi
  uint8_t wifi_x = 105;
  FONT_COLOR_WHITE();
  FONT_TYPE_ICON();
  draw->printFixed(wifi_x, 0, "", STYLE_NORMAL);  // U+F09E - 61598

  char buffer2[256];
  FONT_COLOR_WHITE();
  FONT_TYPE_DEFAULT();
  snprintf(buffer2, 255, "%d", this->winch->getRemote());
  draw->printFixed(wifi_x + 7, 1, buffer2, STYLE_NORMAL);
  // this->logger->debug("GUI : Display wifi : %s", buffer2);
}

void Gui::createValue(NanoCanvasOps<1>* draw,
                      const char* title,
                      const char* value) {

  FONT_COLOR_WHITE();
  draw->printFixed(0, 0, title, STYLE_NORMAL);
    // draw.text((0, 0), title, fill=COLOR_PRIM_FONT, font=ImageFont.truetype(FONT_TEXT, 12))

  FONT_COLOR_WHITE();
  draw->fillRect(0, 12, LCD_WIDTH, 12);
  draw->drawRect(0, 12, LCD_WIDTH, 12);

  FONT_COLOR_WHITE();
  draw->printFixed(2, 18, value, STYLE_NORMAL);
    // draw.text((2, 18), "%s" % value, fill=COLOR_PRIM_FONT, font=ImageFont.truetype(FONT_TEXT, 14))

  auto y = 0.78 * LCD_HEIGHT;
  FONT_COLOR_WHITE();
  draw->fillRect(0, y, LCD_WIDTH, LCD_HEIGHT);
  draw->drawRect(0, y, LCD_WIDTH, LCD_HEIGHT);

  FONT_COLOR_BLACK();
  draw->printFixed(0, 0.80 * LCD_HEIGHT, "exit to save...", STYLE_NORMAL);
    // draw.text((0, 0.80 * LCD_HEIGHT), "exit to save...", fill="black", font=ImageFont.truetype(FONT_TEXT, 12))

}

void Gui::createMenuScroll(NanoCanvasOps<1>* draw,
                           std::vector<std::string> items,
                           std::string selected_item = nullptr) {

  uint8_t font_size = 12;
  uint8_t draw_cursor_pos = 0;
  uint8_t draw_view_pos = 0;

  uint8_t cursor_limit_screen = (LCD_HEIGHT / font_size) - 1;
  if (this->cursor_pos > cursor_limit_screen) {
    draw_view_pos = -((this->cursor_pos - cursor_limit_screen) * font_size);
  }

  for (auto &item : items) {
    uint8_t y = draw_cursor_pos * font_size;

    if (this->cursor_pos == draw_cursor_pos) {
      FONT_COLOR_WHITE();
      draw->fillRect(0, draw_view_pos + y, LCD_WIDTH, draw_view_pos + y + font_size);
      draw->drawRect(0, draw_view_pos + y, LCD_WIDTH, draw_view_pos + y + font_size);

      FONT_COLOR_BLACK();
    } else {
      FONT_COLOR_WHITE();
    }

    if (selected_item.empty() && selected_item == item) {
      FONT_TYPE_ICON();
      draw->printFixed(LCD_WIDTH - font_size, draw_view_pos + y, "", STYLE_NORMAL);  // U+F00C - 61452
        // draw.text((LCD_WIDTH - font_size, draw_view_pos + y), "", fill=text_color, font=ImageFont.truetype(FONT_ICON, font_size - 2))
    }

    FONT_TYPE_DEFAULT();
    draw->printFixed(1, draw_view_pos + y, item.c_str(), STYLE_NORMAL);
      // draw.text((1, draw_view_pos + y), item, fill=text_color, font=ImageFont.truetype(FONT_TEXT, font_size))
    draw_cursor_pos += 1;
  }
}

void Gui::createMenuIcon(NanoCanvasOps<1>* draw, const char* items) {
  uint8_t font_size = 12;
  uint8_t btn_width = LCD_WIDTH / 3;
  uint8_t btn_start = ((btn_width - font_size) / 2) + 1;
  uint8_t btn_height = 0.78 * LCD_HEIGHT;
  uint8_t draw_cursor_pos = 0;

  for (int i=0; i < sizeof(items); i++) {
      std::string bgd = "white";
      std::string fnt = "black";

      if (i == this->cursor_pos) {
        bgd = "black";
        fnt = "white";
      }

      FONT_COLOR_WHITE();
      draw->fillRect(draw_cursor_pos * btn_width, btn_height, (draw_cursor_pos + 1) * btn_width, LCD_HEIGHT);
      FONT_COLOR_BLACK();
      draw->drawRect(draw_cursor_pos * btn_width, btn_height, (draw_cursor_pos + 1) * btn_width, LCD_HEIGHT);
      // draw.rectangle([draw_cursor_pos * btn_width, btn_height, (draw_cursor_pos + 1) * btn_width, LCD_HEIGHT], fill=bgd, outline=fnt)

      std::string sym(1, items[i]);
      FONT_TYPE_ICON();
      draw->printFixed(btn_start + draw_cursor_pos * btn_width, 0.79 * LCD_HEIGHT, sym.c_str(), STYLE_NORMAL);
        // draw.text((btn_start + draw_cursor_pos * btn_width, 0.79 * LCD_HEIGHT), items[draw_cursor_pos], fill=fnt, font=ImageFont.truetype(FONT_ICON, font_size))
  }
}

void Gui::extractScreen() {
  // Capture mode for DOC
  this->display();

  // Stop Screen 003
  this->enter(InputType::ENTER);
  this->display();

  // Play Screen
  this->enter(InputType::ENTER);

  // Menu Screen 004 & 005
  this->enter(InputType::RIGHT);
  this->display();
  this->enter(InputType::ENTER);
  this->display();

  // Manual postition 006 & 007
  this->enter(InputType::RIGHT);
  this->display();
  this->enter(InputType::ENTER);
  this->display();
  this->enter(InputType::ENTER);

  // Security distance 008 & 009
  this->enter(InputType::RIGHT);
  this->enter(InputType::RIGHT);
  this->display();
  this->enter(InputType::ENTER);
  this->display();
  this->enter(InputType::ENTER);

  // Mode Selector 010 & 011
  this->enter(InputType::RIGHT);
  this->enter(InputType::RIGHT);
  this->enter(InputType::RIGHT);
  this->display();
  this->enter(InputType::ENTER);
  this->display();
  this->enter(InputType::RIGHT);
  this->display();
  this->enter(InputType::ENTER);

  // Mode Velocity Start 012 & 013
  this->enter(InputType::RIGHT);
  this->enter(InputType::RIGHT);
  this->enter(InputType::RIGHT);
  this->enter(InputType::RIGHT);
  this->display();
  this->enter(InputType::ENTER);
  this->display();
  this->enter(InputType::ENTER);

  // Mode Velocity Stop 014 & 015
  this->enter(InputType::RIGHT);
  this->enter(InputType::RIGHT);
  this->enter(InputType::RIGHT);
  this->enter(InputType::RIGHT);
  this->enter(InputType::RIGHT);
  this->display();
  this->enter(InputType::ENTER);
  this->display();
  this->enter(InputType::ENTER);
}

void Gui::drawBoot() {
  if (this->device != nullptr) {
    auto draw = this->draw;
    draw->clear();

    uint8_t font_size = 24;
    std::string name = "OpenWinch";

    auto x = (LCD_WIDTH / 2) - (name.size() / 2 * font_size / 2);
    auto xver = (LCD_WIDTH / 2) + (((name.size() / 2) - 2) * font_size / 2);
    auto y = (LCD_HEIGHT / 2) - (font_size / 2);
    auto yver = y + font_size;

    FONT_TYPE_LOGO();
    FONT_COLOR_WHITE();
    draw->printFixed(x, y, name.c_str(), STYLE_NORMAL);

    FONT_TYPE_DEFAULT();
    FONT_COLOR_WHITE();
    draw->printFixed(xver, yver, VERSION, STYLE_NORMAL);

    this->device->drawCanvas(0, 0, *draw);
  }
}

void Gui::draw_loop() {
  // auto t = std::this_thread;
  GuiType config = GuiType::valueof(OW_GUI);

  if (GuiType::DISABLE != config && GuiType::CAPTURE != config) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/LCD_FPS));
          // with this->regulator:
        if (this->winch->getState().isBoot()) {
          this->display();
        } else {
          this->drawBoot();
          std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
  } else {
    this->extractScreen();
  }
}


ScreenBase::ScreenBase(Gui *_gui) : gui(_gui) {
  this->winch = this->gui->getWinch();
  this->gui->cursor_pos = 0;
  this->gui->view_pos = 0;
}

//////////////////////////////////////////////////////////

MainScreen::MainScreen(Gui *_gui) : ScreenBase{_gui} { }

uint8_t MainScreen::countItems() {
  return 3;
}

void MainScreen::display(NanoCanvasOps<1>* draw) {
  this->count += 2;
  this->inver = true;

  // Status bar
  this->gui->statusBar(draw);

  // Speed
  auto speed_x = 54;
  char buffer2[256];
  sprintf(buffer2, "%d", this->winch->getSpeedTarget());
  FONT_TYPE_SPEED();
  draw->printFixed(speed_x, 14, buffer2, STYLE_NORMAL); //FONT_SIZE_2X
    // draw.text((speed_x, 14), "%s" % self._winch.getSpeedTarget(), fill="white", font=ImageFont.truetype(FONT_TEXT, 35))
  FONT_TYPE_DEFAULT();
  FONT_COLOR_WHITE();
  draw->printFixed(speed_x + 40, 28, SPEED_UNIT, STYLE_NORMAL);
    // draw.text((speed_x + 40, 28), SPEED_UNIT, fill="white", font=ImageFont.truetype(FONT_TEXT, 15))  # Very good

  // Distance
  auto marg = 4;
  auto percent = 0; //TODO 1 / WINCH_DISTANCE * this->winch->getDistance();
  FONT_COLOR_WHITE();
  draw->fillRect(0 + marg, 11, ((LCD_WIDTH - marg) * percent), 14);
  draw->drawRect(0 + marg, 11, ((LCD_WIDTH - marg) * percent), 14);

  if (this->winch->getState().isStop()) {
      this->gui->createMenuIcon(draw, this->ITEMS_IDLE);
  } else if (this->winch->getState().isRun()) {
      this->gui->createMenuIcon(draw, this->ITEMS_RUNNING);
      //this->animateDistance(device);
  } else {
      this->gui->createMenuIcon(draw, this->ITEMS_ERROR);
  }
}

void MainScreen::enter(uint8_t cursor_pos) {
  if (0 == cursor_pos) {
    if (this->winch->getState().isStop()) {
      this->winch->start();
    } else {
      this->winch->stop();
    }
  }
  if (1 == cursor_pos) {
      ScreenBase* old = this->gui->screen;
      this->gui->screen = new MenuScreen(this->gui);
      delete old;
  }
  if (2 == cursor_pos) {
    this->winch->initialize();
  }
}

//////////////////////////////////////////////////////////

MenuScreen::MenuScreen(Gui *_gui) : ScreenBase{_gui} { }

uint8_t MenuScreen::countItems() {
  return this->ITEMS_MENU.size();
}

void MenuScreen::display(NanoCanvasOps<1>* draw) {
  this->gui->createMenuScroll(draw, this->ITEMS_MENU);
}

void MenuScreen::enter(uint8_t cursor_pos) {
  ScreenBase* old = this->gui->screen;

  switch (cursor_pos) {
    case 0: this->gui->screen = new MainScreen(this->gui); break;
    case 1: this->gui->screen = new ManualPositionScreen(this->gui); break;
    case 2: this->gui->screen = new SecurityDistanceScreen(this->gui); break;
    case 3: this->gui->screen = new ModeSelectorScreen(this->gui); break;
    case 4: this->gui->screen = new VelocityStartScreen(this->gui); break;
    case 5: this->gui->screen = new VelocityStopScreen(this->gui); break;
  }
  delete old;
}

//////////////////////////////////////////////////////////

ManualPositionScreen::ManualPositionScreen(Gui *_gui) : ScreenBase{_gui} { }

uint8_t ManualPositionScreen::countItems() {
  return 0;
}

void ManualPositionScreen::display(NanoCanvasOps<1>* draw) {

}

void ManualPositionScreen::enter(uint8_t cursor_pos) {

}

//////////////////////////////////////////////////////////

SecurityDistanceScreen::SecurityDistanceScreen(Gui *_gui) : ScreenBase{_gui} { }

uint8_t SecurityDistanceScreen::countItems() {
  return 0;
}

void SecurityDistanceScreen::display(NanoCanvasOps<1>* draw) {

}

void SecurityDistanceScreen::enter(uint8_t cursor_pos) {

}

//////////////////////////////////////////////////////////

ModeSelectorScreen::ModeSelectorScreen(Gui *_gui) : ScreenBase{_gui} { }

uint8_t ModeSelectorScreen::countItems() {
  return 0;
}

void ModeSelectorScreen::display(NanoCanvasOps<1>* draw) {

}

void ModeSelectorScreen::enter(uint8_t cursor_pos) {

}

//////////////////////////////////////////////////////////

VelocityStartScreen::VelocityStartScreen(Gui *_gui) : ScreenBase{_gui} { }

uint8_t VelocityStartScreen::countItems() {
  return 0;
}

void VelocityStartScreen::display(NanoCanvasOps<1>* draw) {

}

void VelocityStartScreen::enter(uint8_t cursor_pos) {

}

//////////////////////////////////////////////////////////

VelocityStopScreen::VelocityStopScreen(Gui *_gui) : ScreenBase{_gui} {
  this->gui->cursor_pos = this->value;
}

uint8_t VelocityStopScreen::countItems() {
  return 255;
}

void VelocityStopScreen::display(NanoCanvasOps<1>* draw) {
  this->gui->createValue(draw, this->TITLE.c_str(), std::to_string(this->gui->getPos()).c_str());
}

void VelocityStopScreen::enter(uint8_t cursor_pos) {
  free(this->gui->screen);
  this->gui->screen = new MenuScreen(this->gui);
}
