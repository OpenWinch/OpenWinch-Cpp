/**
 * @file mode.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#include "mode.hpp"
#include "controller.hpp"
#include "hardware.hpp"
#include "logger.hpp"
#include "constantes.hpp"

// ModeEngine

ModeEngine::ModeEngine(Board *_board, Winch *_winch) : board(_board), winch(_winch) {
    this->logger = &Logger::get();
    this->speed_ratio = 1 / MOTOR_MAX;
}

void ModeEngine::applyThrottleValue() {
  this->logger->live("Calculate throttle value.");
  uint32_t value = this->speed_ratio * this->speed_current;

  if (this->board->getThrottleValue() != value) {
    this->logger->debug("Apply throttle value.");
    this->board->setThrottleValue(value);
  }
}

// Move to Board or Winch
float ModeEngine::getDistance() {
  this->logger->debug("Calculate distance.");
  return -999; //rotate2distance(this->board->getRotationFromBegin());
}

uint8_t ModeEngine::getSpeedCurrent() {
  return this->speed_current;
}

void ModeEngine::runControlLoop() {
  //auto t = std::this_thread;
  this->logger->debug("Starting Control Loop.");

  while (true) {
      this->logger->live("Current state : %s - speed : %s - limit : %s",
                          std::string(this->winch->getState()).c_str(),
                          std::to_string(this->speed_current).c_str(),
                          std::to_string(this->board->getRotationFromBegin()).c_str());

      // INIT
      if (this->winch->getState().isInit()) {
        this->initialize();
      }

      // STARTING or RUNNING
      if (this->winch->getState().isRun()) {
        this->starting();
      }

      // STOP
      if (this->winch->getState().isStop()) {
        this->stopping();
      }

      // Specifical mode
      this->extraMode();

      // EMERGENCY
      if (this->winch->getState().isFault()) {
        this->fault();
      }

      this->applyThrottleValue();

      // CPU idle
      std::this_thread::sleep_for(std::chrono::milliseconds(LOOP_DELAY));
  }

  this->logger->debug("Stopping Control Loop.");
}

bool ModeEngine::isBeginSecurity() {
  return (this->board->getRotationFromBegin() - this->security_begin <= 0);
}

void ModeEngine::initialize() {
  this->logger->debug("Initialize mode.");
  this->speed_current = 0;
  this->board->initialize();
  this->winch->initialized();
}

void ModeEngine::starting() {
  // Increment speed
  if (this->speed_current < this->winch->getSpeedTarget()) {
    this->speed_current += this->velocity_start;

    if (this->speed_current >= this->winch->getSpeedTarget()) {
        this->winch->started();
    }
  }

  // Decrement speed
  if (this->speed_current > this->winch->getSpeedTarget()) {
    uint16_t vel_stop = this->velocity_stop;
    uint16_t diff_stop = this->speed_current - this->winch->getSpeedTarget();

    if (vel_stop > diff_stop) {
      vel_stop = diff_stop;
    }

    if (this->speed_current > vel_stop) {
      this->speed_current -= vel_stop;
    } else {
      this->speed_current = 0;
    }
  }
}

void ModeEngine::stopping() {
  if (this->speed_current > 0) {
    uint16_t vel_stop = this->velocity_stop;
    uint16_t diff_stop = this->speed_current - 0;

    if (vel_stop > diff_stop) {
      vel_stop = diff_stop;
    }

    if (this->speed_current > vel_stop) {
      this->speed_current -= vel_stop;
    } else {
      this->speed_current = 0;
      this->winch->stopped();
    }

  } else if (this->speed_current < 0) {
    this->speed_current = 0;
    this->winch->stopped();
  }

}

void ModeEngine::fault() {
  this->board->emergency();
  this->speed_current = 0;
}


// OneWayMode

OneWayMode::OneWayMode(Board *_board, Winch *_winch) : ModeEngine{_board, _winch} {

}

void OneWayMode::extraMode() {
  if (this->winch->getState().isRun() && this->isBeginSecurity()) {  // Limit position START
    this->winch->stop();
  }
}


// TwoWayMode

TwoWayMode::TwoWayMode(Board *_board, Winch *_winch) : ModeEngine{_board, _winch} {

}

void TwoWayMode::extraMode() {

}

bool TwoWayMode::isEndSecurity() {
  return false;
}


// InfinityMode

InfinityMode::InfinityMode(Board *_board, Winch *_winch) : ModeEngine{_board, _winch} {

}

void InfinityMode::extraMode() {

}
