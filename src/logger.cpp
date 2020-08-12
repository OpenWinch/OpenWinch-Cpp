
#include <slog.h>

#include <cstdio>
#include <string>
#include <cstdarg>

#include "logger.hpp"

Logger::Logger() {
    slog_init("openwinch.log", "slog.cfg", 3, 1);
    printf("slog Version: %s\n", slog_version(0));
    this->debug("Initialize Logger...");
}

void Logger::info(const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    slog(0, SLOG_INFO, msg, args);
    va_end(args);
}

void Logger::debug(const char* msg...) {
    va_list args;
    va_start(args, msg);
    slog(0, SLOG_DEBUG, msg, args);
    va_end(args);
}

void Logger::warning(const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    slog(0, SLOG_WARN, msg, args);
    va_end(args);
}

void Logger::error(const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    slog(0, SLOG_ERROR, msg, args);
    va_end(args);
}

void Logger::fatal(const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    slog(0, SLOG_FATAL, msg, args);
    va_end(args);
}

// For C ansi

Logger *logger_info(Logger *logger, const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    logger->info(msg, args);
    va_end(args);
    return logger;
}

Logger *logger_debug(Logger *logger, const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    logger->debug(msg, args);
    va_end(args);
    return logger;
}

Logger *logger_warning(Logger *logger, const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    logger->warning(msg, args);
    va_end(args);
    return logger;
}

Logger *logger_error(Logger *logger, const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    logger->error(msg, args);
    va_end(args);
    return logger;
}

Logger *logger_fatal(Logger *logger, const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    logger->fatal(msg, args);
    va_end(args);
    return logger;
}
