/*
  tachometer.h - setting variables for OpenWinch

  Copyright (C) 2020  Mickael Gaillard
*/

#include "state.hpp"

bool State::checkRun(State current) {
    return (State::RUNNING == current.value || State::START == current.value);
}

bool State::checkStop(State current) {
    return (State::STOP == current.value || State::IDLE == current.value);
}

bool State::checkFault(State current) {
    return (State::ERROR == current.value);
}

bool State::checkInit(State current) {
    return (State::INIT == current.value);
}

bool State::checkBoot(State current) {
    return (State::UNKNOWN != current.value);
}

bool State::isRun() {
    return State::checkRun(*this);
}

bool State::isStop() {
    return State::checkStop(*this);
}

bool State::isFault() {
    return State::checkFault(*this);
}

bool State::isInit() {
    return State::checkInit(*this);
}

bool State::isBoot() {
    return State::checkBoot(*this);
}
