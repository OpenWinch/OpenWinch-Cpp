/*
  logger.h - OpenWinch Project

  Copyright (C) 2020  Mickael Gaillard
*/

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#ifdef __cplusplus

#include <cstdarg>

class Logger {

 public:
  // Logger(Logger const &) = delete;
  // Logger &operator=(Logger const &) = delete;

  static Logger& get() {
    static Logger logger;
    return logger;
  }

  void info(const char*, ...);
  void debug(const char* ...);
  void warning(const char*, ...);
  void error(const char*, ...);
  void fatal(const char*, ...);

 private:
  Logger();
  ~Logger()= default;
  Logger(const Logger&)= delete;
  Logger& operator=(const Logger&)= delete;
};

// #else
//   typedef
//     struct Logger
//       Logger;
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__STDC__) || defined(__cplusplus)
  extern void c_function(Logger*);   /* ANSI C prototypes */
  extern Logger* logger_info(Logger*, const char*, ...);
  extern Logger* logger_debug(Logger*, const char*, ...);
  extern Logger* logger_warning(Logger*, const char*, ...);
  extern Logger* logger_error(Logger*, const char*, ...);
  extern Logger* logger_fatal(Logger*, const char*, ...);
#else
  extern void c_function();        /* K&R style */
  extern Logger* logger_info(const char*, ...);
  extern Logger* logger_debug(const char*, ...);
  extern Logger* logger_warning(const char*, ...);
  extern Logger* logger_error(const char*, ...);
  extern Logger* logger_fatal(const char*, ...);
#endif

#ifdef __cplusplus
}
#endif

#endif  // LOGGER_HPP_
