/**
 * @file controller.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#include "config.hpp"
#include "controller.hpp"
#include "display.hpp"
#include "input.hpp"
#include "mode.hpp"
#include "logger.hpp"
#include "hardware.hpp"
#include "version.hpp"

#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>


/**
 * @brief Construct a new Winch:: Winch object
 */
Winch::Winch() {
  this->logger = &Logger::get();

//   const int result_1 = std::atexit(this->emergency);
//   if (result_1 != 0) {
//     std::cerr << "Registration failed\n";
//     //return EXIT_FAILURE;
//   }

  this->banner();
  this->loadConfig();
  this->initControlLoop();
}

/**
 * @brief Initialise Hardware.
 * 
 * Step :
 *  - Initialise Sensor
 *  - Position at origin
 */
void Winch::initialize() {
  this->logger->debug("Initialize Winch hardware...");
  this->changeState(State::INIT);
}

/**
 * @brief Call when hardware stop completely. 
 */
void Winch::initialized() {
  if (this->state.isInit()) {
    this->changeState(State::IDLE);
  }
}

/**
 * @brief Command Start winch. 
 */
void Winch::start() {
  this->logger->info("Press Start");

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
void Winch::started() {
  if (this->state == State::START) {
    this->changeState(State::RUNNING);
  }
}

/**
 * @brief Command Stop winch.
 */
void Winch::stop() {
  this->logger->info("Press Stop");

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
void Winch::stopped() {
  if (this->state == State::STOP) {
    this->changeState(State::IDLE);
  }
}

/**
 * @brief Command Emergency winch.
 */
void Winch::emergency() {
  this->logger->fatal("HALT EMERGENCY");
  this->changeState(State::ERROR);
}

/**
 * @brief Process display in console.
 */
void Winch::display() {
  printf("State\t: %s\nTarget Speed\t: %s\nCurrent speed\t: %s" ,
          std::string(this->state).c_str(),
          std::to_string(this->speed_target).c_str(),
          std::to_string(this->mode->getSpeedCurrent()).c_str());
}

/**
 * @brief 
 * 
 * @return ModeType 
 */
ModeType Winch::getMode() {
  return ModeType::OneWay; // return nullptr;  // this->mode;
}

/**
 * @brief Get Target speed of winch.
 * 
 * @return uint8_t Speed targeted in KM/H
 */
uint8_t Winch::getSpeedTarget() {
  return this->speed_target;
}

/**
 * @brief Get actual state of winch.
 * 
 * @return State of winch
 */
State Winch::getState() {
  return this->state;
}

/**
 * @brief Get actual state of Battery.
 * 
 * @return uint8_t 
 */
uint8_t Winch::getBattery() {
  return this->board->getBattery();
}

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t Winch::getRemote() {
  return 15;
}

/**
 * @brief 
 * 
 * @return float 
 */
float Winch::getDistance() {
  return this->mode->getDistance();
}

/**
 * @brief Up speed.
 * 
 * @param value to increment speed (default is 1)
 */
void Winch::speedUp(uint8_t value) {
  if (this->speed_target < SPEED_MAX) {
    this->speed_target += value;
  }
}

/**
 * @brief Down speed.
 * 
 * @param value to decrement speed (default is 1)
 */
void Winch::speedDown(uint8_t value) {
  if (this->speed_target > SPEED_MIN) {
    this->speed_target -= value;
  }
}

/**
 * @brief Set speed.
 * 
 * @param value 
 */
void Winch::speedValue(uint8_t value) {
  if (value >= SPEED_MIN || value < SPEED_MAX) {
    this->speed_target = value;
  }
}

/**
 * @brief 
 * 
 * @param value 
 */
void Winch::enterGui(InputType value) {
  this->gui->enter(value);
}

/**
 * @brief Display Banner of OpenWinch
 */
void Winch::banner() {
  this->logger->info("\n"
    "   ____                 _       ___            __\n"
    "  / __ \\____  ___  ____| |     / (_)___  _____/ /_\n"
    " / / / / __ \\/ _ \\/ __ \\ | /| / / / __ \\/ ___/ __ \\\n"
    "/ /_/ / /_/ /  __/ / / / |/ |/ / / / / / /__/ / / /\n"
    "\\____/ .___/\\___/_/ /_/|__/|__/_/_/ /_/\\___/_/ /_/\n"
    "    /_/                                            Ver. %s",
    VERSION);
}

/**
 * @brief Load configuration.
 */
void Winch::loadConfig() {
  this->logger->debug("Gui config : %s", OW_GUI);
  this->gui = new Gui(this);
  this->gui->boot();
  //this->input = Keyboard(this);

  this->logger->debug("Board config : %s", OW_BOARD);
  this->board = new Emulator(this);  // loadClass( OW_BOARD, this);
  //this->logger->info("Board : %s", type(this->board).__name__);

  this->logger->debug("Mode config : %s", OW_MODE);
  this->mode = new OneWayMode(this->board, this);  // ModeFactory.modeFactory(this, this->board, OW_MODE);
  this->logger->info("Mode : %s", std::string(this->getMode()).c_str());
}

/**
 * @brief Initialize Control Loop thread.
 */
void Winch::initControlLoop() {
  this->logger->debug("Initialize Control Loop...");

  this->controlLoop = new std::thread(&ModeEngine::runControlLoop, this->mode);
  this->changeState(State::BOOTED);
}

/**
 * @brief Change State of machine-state.
 * 
 * @param value as state enum value.
 */
void Winch::changeState(State value) {
  if (this->state != value) {
    this->logger->debug("Switch state : %s", std::string(value).c_str());
    this->state = value;
  }
}
