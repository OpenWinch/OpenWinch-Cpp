/**
 * @file logger_slog.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020-2021
 */

#include "openwinch.hpp"
#include "logger.hpp"

//#ifdef OW_BD_PI
#ifdef OW_LOG_SLOG

#ifdef __cplusplus
extern "C" {
#endif
#include <slog.h>
#ifdef __cplusplus
}
#endif

#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <string>

Logger::Logger() {
    slog_init("openwinch.log", "slog.cfg", 100, 1);

    printf("slog Version: %s\n", slog_version(0));
    this->debug("Initialize Logger...");
}

void Logger::live(const char *msg, ...) {
    char sInput[MAXMSG];
    memset(sInput, 0, sizeof(sInput));

    va_list args;
    va_start(args, msg);
    vsprintf(sInput, msg, args);
    va_end(args);

    slog(99, SLOG_LIVE, sInput);
}

void Logger::info(const char *msg, ...) {
    char sInput[MAXMSG];
    memset(sInput, 0, sizeof(sInput));

    va_list args;
    va_start(args, msg);
    vsprintf(sInput, msg, args);
    va_end(args);

    slog(2, SLOG_INFO, sInput);
}

void Logger::debug(const char * msg, ...) {
    char sInput[MAXMSG];
    memset(sInput, 0, sizeof(sInput));

    va_list args;
    va_start(args, msg);
    vsprintf(sInput, msg, args);
    va_end(args);

    slog(3, SLOG_DEBUG, sInput);
}

void Logger::warning(const char *msg, ...) {
    char sInput[MAXMSG];
    memset(sInput, 0, sizeof(sInput));

    va_list args;
    va_start(args, msg);
    vsprintf(sInput, msg, args);
    va_end(args);

    slog(1, SLOG_WARN, sInput);
}

void Logger::error(const char *msg, ...) {
    char sInput[MAXMSG];
    memset(sInput, 0, sizeof(sInput));

    va_list args;
    va_start(args, msg);
    vsprintf(sInput, msg, args);
    va_end(args);

    slog(0, SLOG_ERROR, sInput);
}

void Logger::fatal(const char *msg, ...) {
    char sInput[MAXMSG];
    memset(sInput, 0, sizeof(sInput));

    va_list args;
    va_start(args, msg);
    vsprintf(sInput, msg, args);
    va_end(args);

    slog(0, SLOG_FATAL, sInput);
}

// For C ansi

// Logger *logger_info(Logger *logger, const char *msg, ...) {
//     va_list args;
//     va_start(args, msg);
//     logger->info(msg, args);
//     va_end(args);
//     return logger;
// }

// Logger *logger_debug(Logger *logger, const char *msg, ...) {
//     va_list args;
//     va_start(args, msg);
//     logger->debug(msg, args);
//     va_end(args);
//     return logger;
// }

// Logger *logger_warning(Logger *logger, const char *msg, ...) {
//     va_list args;
//     va_start(args, msg);
//     logger->warning(msg, args);
//     va_end(args);
//     return logger;
// }

// Logger *logger_error(Logger *logger, const char *msg, ...) {
//     va_list args;
//     va_start(args, msg);
//     logger->error(msg, args);
//     va_end(args);
//     return logger;
// }

// Logger *logger_fatal(Logger *logger, const char *msg, ...) {
//     va_list args;
//     va_start(args, msg);
//     logger->fatal(msg, args);
//     va_end(args);
//     return logger;
// }

#endif  // OW_LOG_SLOG
//#endif  // OW_BD_PI
