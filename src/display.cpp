/**
 * @file display.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#include "display.hpp"
#include "display_config.hpp"
#include "constantes.hpp"
#include "controller.hpp"
#include "config.hpp"
#include "hardware_config.hpp"
#include "input.hpp"

#include <string>
#include <vector>

#include "lcdgfx.h"

Gui::Gui(Winch *_winch) : winch(_winch) {
  GuiType config = GuiType::valueof(OW_GUI);

  if (GuiType::SH1106_I2C == config) {
    // from luma.core.interface.serial import i2c
    // from luma.oled.device import sh1106

    // serial_interface = i2c(port=1, address=LCD_ADDR)
    this->device = new DisplaySH1106_128x64_I2C(-1); // = sh1106(serial_interface, width=LCD_WIDTH, height=LCD_HEIGHT, rotate=0)
    this->device->begin();
    this->device->clear();
    this->device->setColor(RGB_COLOR16(255,255,0));
    //this->device->drawLine(10,30,56,96);
    //this->device->setFixedFont(free_SLANT12x10);
    this->device->setFixedFont(ssd1306xled_font6x8);
    this->device->clear();
    this->device->printFixed(1, 1, "OpenWinch", STYLE_NORMAL);
  } else if (GuiType::VGA == config) {
    // from luma.emulator.device import pygame

    this->device ; // = pygame(width=LCD_WIDTH, height=LCD_HEIGHT, rotate=0, mode='1', transform='scale2x', scale=2, frame_rate=60)
  } else if (GuiType::CAPTURE == config) {
    // from luma.emulator.device import capture

    this->device ; // = capture(width=LCD_WIDTH, height=LCD_HEIGHT, rotate=0, mode='1', transform='scale2x', scale=2, file_template="docs/images/screens/OpenWinch_{0:06}.png")
  }

  if (this->device != nullptr) {
    // this->device->show();
  }

//   this->font = ImageFont.truetype(FONT_TEXT, 8)
//   this->regulator = framerate_regulator(fps=LCD_FPS)

  this->screen = new MainScreen(this);
}

Winch* Gui::getWinch() {
  return this->winch;
}

void Gui::boot() {
  this->drawBoot();
  this->display_draw_Loop = std::thread(&Gui::draw_loop, this);
}

void Gui::display() {
  if (this->device != nullptr) {
    // with canvas(this->device) as draw:
    //   this->screen.display(draw)
  }
}

uint8_t Gui::getPos() {
  return this->cursor_pos;
}

void Gui::enter(InputType key) {
  // Directional Common
  if (InputType::RIGHT == key) {
    this->cursor_pos += 1;
  } else if (InputType::LEFT == key) {
    this->cursor_pos -= 1;
  } else if (InputType::ENTER == key) {
    this->screen->enter(this->cursor_pos);
  }

  // out bound fix
  if (this->cursor_pos < 0) {
      if (this->screen->countItems() - (uint8_t)1 > 0) {
        this->cursor_pos = this->screen->countItems() - 1;
      } else {
        this->cursor_pos = 0;
      }
  } else if (this->cursor_pos > this->screen->countItems() - 1) {
    this->cursor_pos = 0;
  }
}

void Gui::statusBar(void*) {
  // Battery
  uint8_t battery_value = this->winch->getBattery();

  char battery_symbol = '';
  if (battery_value > 87.5) {
      battery_symbol = '';
  } else if (battery_value > 62.5) {
      battery_symbol = '';
  } else if (battery_value > 37.5) {
      battery_symbol = '';
  } else if (battery_value > 12.5) {
      battery_symbol = '';
  }

  uint8_t battery_x = 2;
  // draw.text((battery_x, 0), battery_symbol, fill=COLOR_PRIM_FONT, font=ImageFont.truetype(FONT_ICON, 8))
  // draw.text((battery_x + 15, 1), "%s%%" % battery_value, fill=COLOR_PRIM_FONT, font=this->__font)

  // Wifi
  uint8_t wifi_x = 105;
  // draw.text((wifi_x, 0), "", fill=COLOR_PRIM_FONT, font=ImageFont.truetype(FONT_ICON, 8))
  // draw.text((wifi_x + 7, 1), "%s " % this->__winch.getRemote(), fill=COLOR_PRIM_FONT, font=this->__font)

}

void Gui::createValue(void*, const char* title, const char* value) {
  // draw.text((0, 0), title, fill=COLOR_PRIM_FONT, font=ImageFont.truetype(FONT_TEXT, 12))
  // draw.rectangle([0, 12, LCD_WIDTH, 12], fill="white", outline="white")
  // draw.text((2, 18), "%s" % value, fill=COLOR_PRIM_FONT, font=ImageFont.truetype(FONT_TEXT, 14))

  // y = 0.78 * LCD_HEIGHT
  // draw.rectangle([0, y, LCD_WIDTH, LCD_HEIGHT], fill="white", outline="white")
  // draw.text((0, 0.80 * LCD_HEIGHT), "exit to save...", fill="black", font=ImageFont.truetype(FONT_TEXT, 12))

}

void Gui::createMenuScroll(void*, std::vector<std::string> items, std::string selected_item = nullptr) {
  uint8_t font_size = 12;
  uint8_t draw_cursor_pos = 0;
  uint8_t draw_view_pos = 0;

  uint8_t cursor_limit_screen = (LCD_HEIGHT / font_size) - 1;
  if (this->cursor_pos > cursor_limit_screen) {
    draw_view_pos = -((this->cursor_pos - cursor_limit_screen) * font_size);
  }

  for (auto &item : items) {
    std::string text_color = COLOR_PRIM_FONT;
    uint8_t y = draw_cursor_pos * font_size;

    if (this->cursor_pos == draw_cursor_pos) {
      std::string text_color = "black";
      // draw.rectangle([0, draw_view_pos + y, LCD_WIDTH, draw_view_pos + y + font_size], fill="white", outline="white")
    }

    if (selected_item.empty() && selected_item == item) {
      // draw.text((LCD_WIDTH - font_size, draw_view_pos + y), "", fill=text_color, font=ImageFont.truetype(FONT_ICON, font_size - 2))
    }
    // draw.text((1, draw_view_pos + y), item, fill=text_color, font=ImageFont.truetype(FONT_TEXT, font_size))
    draw_cursor_pos += 1;
  }
}

void Gui::createMenuIcon(void*, std::vector<std::string> items) {
  uint8_t font_size = 12;
  uint8_t btn_width = LCD_WIDTH / 3;
  uint8_t btn_start = ((btn_width - font_size) / 2) + 1;
  uint8_t btn_height = 0.78 * LCD_HEIGHT;
  uint8_t draw_cursor_pos = 0;

  for (auto &item : items) {
      std::string bgd = "white";
      std::string fnt = "black";

      if (draw_cursor_pos == this->cursor_pos) {
        bgd = "black";
        fnt = "white";
      }

      // draw.rectangle([draw_cursor_pos * btn_width, btn_height, (draw_cursor_pos + 1) * btn_width, LCD_HEIGHT], fill=bgd, outline=fnt)
      // draw.text((btn_start + draw_cursor_pos * btn_width, 0.79 * LCD_HEIGHT), items[draw_cursor_pos], fill=fnt, font=ImageFont.truetype(FONT_ICON, font_size))
      draw_cursor_pos += 1;
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
    // with canvas(this->__device) as draw:
    //   font_size = 20
    //   name = "OpenWinch"

    //   x = (LCD_WIDTH / 2) - (len(name) / 2 * font_size / 2)
    //   xver = (LCD_WIDTH / 2) + (((len(name) / 2) - 1) * font_size / 2)
    //   y = (LCD_HEIGHT / 2) - (font_size / 2)
    //   yver = y + font_size

    //   draw.text((x, y), name, fill=COLOR_PRIM_FONT, font=ImageFont.truetype(FONT_LOGO, font_size))
    //   draw.text((xver, yver), __version__, fill=COLOR_PRIM_FONT, font=ImageFont.truetype(FONT_TEXT, 8))
  }
}

void Gui::draw_loop() {
  // auto t = std::this_thread;
  GuiType config = GuiType::valueof(OW_GUI);

  if (GuiType::DISABLE != config && GuiType::CAPTURE != config) {
    while (true) {
      //with this->regulator:
        if (this->winch->getState().isBoot()) {
          this->display();
        } else {
          this->drawBoot();
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
  return sizeof(this->ITEMS_IDLE);
}

void MainScreen::display(void*) {

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
      free(this->gui->screen);
      this->gui->screen = new MenuScreen(this->gui);
  }
  if (2 == cursor_pos) {

  }
}

//////////////////////////////////////////////////////////

MenuScreen::MenuScreen(Gui *_gui) : ScreenBase{_gui} { }

uint8_t MenuScreen::countItems() {
  return 0;
}

void MenuScreen::display(void*) {

}

void MenuScreen::enter(uint8_t cursor_pos) {

}

//////////////////////////////////////////////////////////

ManualPositionScreen::ManualPositionScreen(Gui *_gui) : ScreenBase{_gui} { }

uint8_t ManualPositionScreen::countItems() {
  return 0;
}

void ManualPositionScreen::display(void*) {

}

void ManualPositionScreen::enter(uint8_t cursor_pos) {

}

//////////////////////////////////////////////////////////

SecurityDistanceScreen::SecurityDistanceScreen(Gui *_gui) : ScreenBase{_gui} { }

uint8_t SecurityDistanceScreen::countItems() {
  return 0;
}

void SecurityDistanceScreen::display(void*) {

}

void SecurityDistanceScreen::enter(uint8_t cursor_pos) {

}

//////////////////////////////////////////////////////////

ModeSelectorScreen::ModeSelectorScreen(Gui *_gui) : ScreenBase{_gui} { }

uint8_t ModeSelectorScreen::countItems() {
  return 0;
}

void ModeSelectorScreen::display(void*) {

}

void ModeSelectorScreen::enter(uint8_t cursor_pos) {

}

//////////////////////////////////////////////////////////

VelocityStartScreen::VelocityStartScreen(Gui *_gui) : ScreenBase{_gui} { }

uint8_t VelocityStartScreen::countItems() {
  return 0;
}

void VelocityStartScreen::display(void*) {

}

void VelocityStartScreen::enter(uint8_t cursor_pos) {

}

//////////////////////////////////////////////////////////

VelocityStopScreen::VelocityStopScreen(Gui *_gui) : ScreenBase{_gui} {
  this->gui->cursor_pos = this->value;
}

uint8_t VelocityStopScreen::countItems() {
  return 255;
};

void VelocityStopScreen::display(void* draw) {
  this->gui->createValue(draw, this->TITLE.c_str(), std::to_string(this->gui->getPos()).c_str());
}

void VelocityStopScreen::enter(uint8_t cursor_pos) {
  free(this->gui->screen);
  this->gui->screen = new MenuScreen(this->gui);
}
