/***
 * @file FalconLogging.h
 * @brief Logging defines for log4cxx
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://libnifalcon.nonpolynomial.com/
 *
 */

/*
 * Found this code at http://gonium.net/md/ in the Code Kata articles - Can't find a license, but I'm hoping it's MIT. :) Much thanks to the author
 * for saving me a massive amount of time, since I was planning on doing this myself.
 *
 * This file allows us to use Log4cxx (http://logging.apache.org/log4cxx/index.html) throughout the library without requiring it to be
 * compiled in at all times. Great for debugging.
 *
 * To initialize a logger for console output:
 *
 * 
 * #ifdef ENABLE_LOGGING
 * #include <log4cxx/logger.h>
 * #include <log4cxx/basicconfigurator.h>
 * #include <log4cxx/helpers/exception.h>
 * #include <log4cxx/patternlayout.h>
 * #include <log4cxx/consoleappender.h>
 * static const log4cxx::LogString TTCC_CONVERSION_PATTERN(LOG4CXX_STR("%-5p [%c] - %m%n"));
 * #endif
 * 
 * #ifdef ENABLE_LOGGING
 * void configureLogging(const std::string logString, const log4cxx::LevelPtr level) {
 *   log4cxx::LayoutPtr layout(new log4cxx::PatternLayout(logString));
 *   log4cxx::AppenderPtr appender(new log4cxx::ConsoleAppender(layout));
 *   log4cxx::BasicConfigurator::configure(appender);
 *   log4cxx::LoggerPtr rootlogger = log4cxx::Logger::getRootLogger();
 *   rootlogger->setLevel(level);
 * }
 * #endif
 * 
 * int main()
 * {
 * #ifdef ENABLE_LOGGING
 *   std::string logPattern(TTCC_CONVERSION_PATTERN);
 *   log4cxx::LevelPtr logLevel = log4cxx::Level::toLevel("DEBUG");
 *   configureLogging(logPattern, logLevel);
 * #endif
 * ...
 * }
 * 
 *
 * To log:
 *
 * 
 * class Foo
 * {
 * 	Foo() : INIT_LOGGING("Foo") {}
 * 	LogSomething() { int num = 1;  LOG_INFO("Look! A Number! " << num);  }
 * }
 * 
 */

#ifndef FALCON_LOGGING
#define FALCON_LOGGING

/* Logging */
#if ENABLE_LOGGING
#include <log4cxx/logger.h>
 /*
#ifndef LOG4CXX_STR
#  define LOG4CXX_VERSION 1002
#else
#  define LOG4CXX_STR(a) _T(a)
namespace log4cxx {
   typedef char logchar;
   typedef std::basic_string<logchar> LogString;
}
#  define LOG4CXX_VERSION 907
#endif
 */

#define LDECLARE_LOGGER(logger)           ::log4cxx::LoggerPtr logger
#define LDEFINE_LOGGER(logger, hierarchy) ::log4cxx::LoggerPtr LINIT_LOGGER(logger, hierarchy)
#define LINIT_LOGGER(logger, hierarchy)   logger(::log4cxx::Logger::getLogger(hierarchy))

#define DECLARE_LOGGER()         LDECLARE_LOGGER(logger)
#define DEFINE_LOGGER(hierarchy) LDEFINE_LOGGER(logger, hierarchy)
#define INIT_LOGGER(hierarchy)   LINIT_LOGGER(logger, hierarchy)

#define LLOG_DEBUG(logger, msg) LOG4CXX_DEBUG(logger, msg)
#define LLOG_INFO(logger, msg)  LOG4CXX_INFO(logger, msg)
#define LLOG_WARN(logger, msg)  LOG4CXX_WARN(logger, msg)
#define LLOG_ERROR(logger, msg) LOG4CXX_ERROR(logger, msg)
#define LLOG_FATAL(logger, msg) LOG4CXX_FATAL(logger, msg)

#define LOG_DEBUG(msg) LLOG_DEBUG(logger, msg)
#define LOG_INFO(msg)  LLOG_INFO(logger, msg)
#define LOG_WARN(msg)  LLOG_WARN(logger, msg)
#define LOG_ERROR(msg) LLOG_ERROR(logger, msg)
#define LOG_FATAL(msg) LLOG_FATAL(logger, msg)

#else

#define LDECLARE_LOGGER(logger)   void* __unused_##logger
#define LDEFINE_LOGGER(logger, h)
#define LINIT_LOGGER(logger, h)   __unused_##logger(0)

#define DECLARE_LOGGER()          LDECLARE_LOGGER(logger)
#define DEFINE_LOGGER(hierarchy)  LDEFINE_LOGGER(logger, hierarchy)
#define INIT_LOGGER(hierarchy)    LINIT_LOGGER(logger, hierarchy)

#define LOG_DEBUG(msg)
#define LOG_INFO(msg)
#define LOG_WARN(msg)
#define LOG_ERROR(msg)
#define LOG_FATAL(msg)

#endif

#endif
