/**
 * @file state.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#include "openwinch.hpp"
#include "state.hpp"

bool State::checkRun(State current) {
    return (State::RUNNING == current || State::START == current);
}

bool State::checkStop(State current) {
    return (State::STOP == current || State::IDLE == current);
}

bool State::checkFault(State current) {
    return (State::ERROR == current);
}

bool State::checkInit(State current) {
    return (State::INIT == current);
}

bool State::checkBoot(State current) {
    return (State::UNKNOWN != current);
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
