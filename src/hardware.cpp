#include "hardware.hpp"

#include "controller.hpp"
#include "logger.hpp"

Board::Board(Winch *_winch) : winch{_winch}  {
  this->logger = &Logger::get();
  this->logger->debug("IO : Initialize board...");
}

void Board::initialize() {
  this->rotation_from_init = 0;
}

void Board::setReverse(bool enable) {
  this->reverse = enable;
  this->logger->debug("IO : Change Reverse mode to %s", this->isReverse());
}

bool Board::isReverse() {
  return this->reverse;
}

void Board::setSpeedMode(SpeedMode speed_mode) {
  this->speed_mode = speed_mode;
  this->logger->debug("IO : Change Speed mode to %s", this->getSpeedMode());
}

SpeedMode Board::getSpeedMode() {
  return this->speed_mode;
}

int Board::getBattery() {
  return 100;
}

int Board::getRotationFromBegin() {
  return this->rotation_from_init;
}

int Board::getRotationFromEnd() {
  return 60;
}



Emulator::Emulator(Winch *_winch) : Board{_winch} { }

void Emulator::initialize() {
  Board::initialize();
  this->init = true;
  this->logger->info("IO : Emulator Initialized !");
}

void Emulator::emergency() {
  this->init = false;
  this->logger->info("IO : Emulator Emergency mode !");
}

void Emulator::setThrottleValue(int value) {
  if (this->value > 0) {
    this->rotation_from_init -= 1;
  } else if (this->init) {
    this->rotation_from_init += 1;
  }

  if (this->value != value) {
    this->value = value;
    this->logger->debug("IO : Throttle to %d", this->value);
  }
}

int Emulator::getThrottleValue() {
  this->logger->debug("IO : Throttle to %d", this->value);
  return this->value;
}

void Emulator::setReverse(bool enable) {
  Board::setReverse(enable);
}

void Emulator::setSpeedMode(SpeedMode mode) {
  Board::setSpeedMode(mode);
}

int Emulator::getBattery() {
// power_now = open("/sys/class/power_supply/BAT0/energy_now", "r").readline();
// power_full = open("/sys/class/power_supply/BAT0/energy_full", "r").readline();
// return int(float(power_now) / float(power_full) * 100);
  return 100;
}

