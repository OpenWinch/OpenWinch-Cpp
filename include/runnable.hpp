/**
 * @file runable.hpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2021
 */

#ifndef RUNNABLE_HPP_
#define RUNNABLE_HPP_

#include <atomic>
#include <thread>

class Runnable
{
 public:
  Runnable() { }
  Runnable(Runnable const&) = delete;
  Runnable& operator =(Runnable const&) = delete;

  virtual ~Runnable() {
    this->abort();
  }

  void run() {
    this->abortRequested.store(false);
    this->threadlocal = std::thread(&Runnable::internal_run, this);
  }

  void abort() {
    this->abortRequested.store(true);

    if (this->threadlocal.get_id() != std::this_thread::get_id()
        && this->threadlocal.joinable()) {
      this->threadlocal.join();
    }
  }

  bool isRunning() const {
    return this->running.load();
  }

  bool isNotAbort() const {
    return (false == this->abortRequested.load());
  }

 protected:
  virtual void runLoop() = 0;

 private:
  std::atomic_bool abortRequested = ATOMIC_VAR_INIT(false);
  std::atomic_bool running = ATOMIC_VAR_INIT(false);
  std::thread threadlocal;

  void internal_run() {
    this->running.store(true);
    this->runLoop();
    this->running.store(false);
  }
};

#endif  // RUNNABLE_HPP_