#include "config.hpp"
#include "controller.hpp"
#include "mode.hpp"
#include "logger.hpp"
#include "version.hpp"

#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>

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

void Winch::initialize() {
  this->logger->debug("Initialize Winch hardware...");
  this->changeState(State::INIT);
}

void Winch::initialized() {
  if (this->state.isInit()) {
    this->changeState(State::IDLE);
  }
}

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

void Winch::started() {
  if (this->state == State::START) {
    this->changeState(State::RUNNING);
  }
}

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

void Winch::stopped() {
  if (this->state == State::STOP) {
    this->changeState(State::IDLE);
  }
}

void Winch::emergency() {
  this->logger->fatal("HALT EMERGENCY");
  this->changeState(State::ERROR);
}

void Winch::display() {
  printf("State\t: %s\nTarget Speed\t: %s\nCurrent speed\t: %s" ,
          std::string(this->state).c_str(),
          std::to_string(this->speed_target).c_str(),
          std::to_string(this->mode->getSpeedCurrent()).c_str());
}

ModeType Winch::getMode() {
  // return nullptr;  // this->mode;
}

int Winch::getSpeedTarget() {
  return this->speed_target;
}

State Winch::getState() {
  return this->state;
}

int Winch::getBattery() {
  return this->board->getBattery();
}

int Winch::getRemote() {
  return 15;
}

float Winch::getDistance() {
  return this->mode->getDistance();
}

void Winch::speedUp(int value) {
  if (this->speed_target < SPEED_MAX) {
    this->speed_target += value;
  }
}

void Winch::speedDown(int value) {
  if (this->speed_target > SPEED_MIN) {
    this->speed_target -= value;
  }
}

void Winch::speedValue(int value) {
  if (value >= SPEED_MIN || value < SPEED_MAX) {
    this->speed_target = value;
  }
}

void Winch::enterGui(InputType value) {
  this->gui->enter(value);
}

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
  this->logger->info("Mode : %s", this->getMode());
}

void Winch::initControlLoop() {
  this->logger->debug("Initialize Control Loop...");

  this->controlLoop = new std::thread(&ModeEngine::runControlLoop, this->mode);
  this->changeState(State::BOOTED);
}

void Winch::changeState(State value) {
  if (this->state != value) {
    this->logger->debug("Switch state : %s", value);
    this->state = value;
  }
}
