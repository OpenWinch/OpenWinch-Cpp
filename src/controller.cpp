/**
 * @file controller.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020-2021
 */

#include "openwinch.hpp"
#include "controller.hpp"

#include "display.hpp"
#include "input.hpp"
#include "mode.hpp"
#include "hardware.hpp"
#include "bridge_io.hpp"

#ifdef OW_BD_PI
#include "hardware_pi.hpp"
#endif  // OW_BD_PI

#if defined(OW_BD_EMU) || defined(OW_BD_PI)

#endif  // OW_BD_EMU || OW_BD_PI

#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <typeinfo>  

void terminate() {
  WinchControl::get().exit();
  Device::terminate_gpio();
  std::cout << "Stop !!" << std::endl;
}

/**
 * @brief Construct a new Winch:: Winch object
 */
WinchControl::WinchControl() : Winch() {
  this->logger = &Logger::get();
}

void WinchControl::boot() {
  const int result_1 = std::atexit(terminate);
  if (result_1 != 0) {
    std::cerr << "Registration failed\n";
    //return EXIT_FAILURE;
  }

  this->banner();
  this->loadConfig();
  this->initControlLoop();

  this->changeState(State::BOOTED);
}

/**
 * @brief Initialise Hardware.
 * 
 * Step :
 *  - Initialise Sensor
 *  - Position at origin
 */
void WinchControl::initialize() {
  this->logger->info("Initialize Winch hardware... Reset counter !");
  this->changeState(State::INIT);
}

/**
 * @brief Call when hardware stop completely. 
 */
void WinchControl::initialized() {
  if (this->state.isInit()) {
    this->changeState(State::IDLE);
  }
}

/**
 * @brief Command Start winch. 
 */
void WinchControl::start() {
  this->logger->info("Start");

  if (this->state.isStop()) {
    this->changeState(State::START);
  } else if (this->state == State::START) {
    this->logger->warning("Switch mode alway enable !");
  } else {
    this->logger->error("Not possible to start, re-initialize Winch !");
  }
}

/**
 * @brief Call when hardware process start completely.
 */
void WinchControl::started() {
  if (this->state == State::START) {
    this->changeState(State::RUNNING);
  }
}

/**
 * @brief Command Stop winch.
 */
void WinchControl::stop() {
  this->logger->info("Stop");

  if (this->state.isRun()) {
    this->changeState(State::STOP);
  } else if (this->state == State::STOP) {
    this->logger->warning("Switch mode alway enable !");
  } else {
    this->logger->error("Not possible to stop, re-initialize Winch !");
  }
}

/**
 * @brief Call when hardware stop completely.
 */
void WinchControl::stopped() {
  if (this->state == State::STOP) {
    this->changeState(State::IDLE);
  }
}

/**
 * @brief Command Emergency winch.
 */
void WinchControl::emergency() {
  this->logger->fatal("HALT EMERGENCY");
  this->changeState(State::ERROR);
}

/**
 * @brief Process display in console.
 */
void WinchControl::display() {
  printf("State\t: %s\nTarget Speed\t: %s\nCurrent speed\t: %s" ,
          std::string(this->state).c_str(),
          std::to_string(this->speed_target).c_str(),
          std::to_string(this->mode->getSpeedCurrent()).c_str());
}

void WinchControl::exit() {
  this->logger->info("Shutdown...");
  this->emergency();

  this->changeState(State::HALT);
  delete this->gui;
}

/**
 * @brief 
 * 
 * @return ModeType 
 */
ModeType WinchControl::getMode() {
  return ModeType::OneWay; // return nullptr;  // this->mode;
}

/**
 * @brief Get Target speed of winch.
 * 
 * @return uint8_t Speed targeted in KM/H
 */
uint8_t WinchControl::getSpeedTarget() {
  return this->speed_target;
}

/**
 * @brief Get actual state of winch.
 * 
 * @return State of winch
 */
State WinchControl::getState() {
  return this->state;
}

/**
 * @brief Get actual state of Battery.
 * 
 * @return uint8_t 
 */
uint8_t WinchControl::getBattery() {
  return this->board->getBattery();
}

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t WinchControl::getRemote() {
  return 15;
}

/**
 * @brief 
 * 
 * @return float 
 */
float WinchControl::getDistance() {
  return this->mode->getDistance();
}

/**
 * @brief Up speed.
 * 
 * @param value to increment speed (default is 1)
 */
void WinchControl::speedUp(uint8_t value) {
  if (this->speed_target < SPEED_MAX) {
    this->speed_target += value;
  }
}

/**
 * @brief Down speed.
 * 
 * @param value to decrement speed (default is 1)
 */
void WinchControl::speedDown(uint8_t value) {
  if (this->speed_target > SPEED_MIN) {
    this->speed_target -= value;
  }
}

/**
 * @brief Set speed.
 * 
 * @param value 
 */
void WinchControl::speedValue(uint8_t value) {
  if (value >= SPEED_MIN || value < SPEED_MAX) {
    this->speed_target = value;
  }
}

/**
 * @brief 
 * 
 * @param value 
 */
void WinchControl::enterGui(InputType value) {
  this->gui->enter(value);
}

void WinchControl::manualForward() {
  this->logger->debug(">>> Forward >>>");
}

void WinchControl::manualReverse() {
  this->logger->debug("<<< Reverse <<<");
}

/**
 * @brief Display Banner of OpenWinch
 */
void WinchControl::banner() {
  this->logger->info(R"(
   ____                 _       ___            __
  / __ \____  ___  ____| |     / (_)___  _____/ /_
 / / / / __ \/ _ \/ __ \ | /| / / / __ \/ ___/ __ \
/ /_/ / /_/ /  __/ / / / |/ |/ / / / / / /__/ / / /
\____/ .___/\___/_/ /_/|__/|__/_/_/ /_/\___/_/ /_/
    /_/                                            Ver. %s
)",
    VERSION);
}

/**
 * @brief Load configuration.
 */
void WinchControl::loadConfig() {
  this->logger->debug("Config stack...");

  this->logger->info("Gui config : %s", OW_GUI);
  this->gui = new Gui(this);
  this->gui->boot();

#ifdef OW_BD_EMU
  this->logger->info("Board config : Emulator");
  this->board = new Emulator(this);
#endif  // OW_BD_EMU

#ifdef OW_BD_PI
  this->logger->info("Board config : RaspberryPi");
  this->board = new Raspberrypi(this);
#endif  // OW_BD_PI

#if defined(OW_BD_EMU) || defined(OW_BD_PI)
  this->input = new Keyboard(this);
#endif  // OW_BD_EMU || OW_BD_PI

  this->logger->info("Mode config : %s", OW_MODE);
  this->mode = new OneWayMode(this->board, this);  // ModeFactory.modeFactory(this, this->board, OW_MODE);
}

/**
 * @brief Initialize Control Loop thread.
 */
void WinchControl::initControlLoop() {
  this->logger->debug("Initialize Control Loop...");

  this->mode->run();
}

/**
 * @brief Change State of machine-state.
 * 
 * @param value as state enum value.
 */
void WinchControl::changeState(State value) {
  if (this->state != value) {
    this->logger->debug("Switch state : %s", std::string(value).c_str());
    this->state = value;
  }
}
