//===-- utils/sw/log.h - LOG collection definitions -----------------------===//
//
// Project-Thunderstrike (PTS) collection header file.
// Find more information at:
// https://github.com/itsthatMatthew/Project-Thunderstrike
//
//===----------------------------------------------------------------------===//
///
/// \file This file contains the declarations of the LOG collection, which is a
/// handful of software utility functions and defines for logging operations.
///
/// It is able to log in several preset modes, such as:
///   - Info (I)
///   - Debug (D)
///   - Warning (W)
///   - Error (E)
///
/// It uses the Serial interface from the Arduino library for communication.
///
//===----------------------------------------------------------------------===//

#ifndef UTILS_SW_LOG_H
#define UTILS_SW_LOG_H

#include <Arduino.h>

namespace PTS
{

namespace LOG
{
/// Prints the given argument to the serial monitor.
/// \tparam TYPE the type of the argument to be printed.
/// \param arg the argument to be printed.
template<typename TYPE>
constexpr void LOG(TYPE arg) { Serial.print(arg); }

/// Prints the given format to the console.
/// '\\' works as an escape character.
/// '%' substitutes the next argument to be printed.
/// \tparam TYPE type of the next argument to be printed.
/// \tparam ...ARG_TYPES types of the remaining arguments.
/// \param format the format to be printed.
/// \param arg the next argument to be printed.
/// \param ...arg_values the remaining arguments.
template<typename TYPE, typename ...ARG_TYPES>
constexpr void LOG(const char *format, TYPE arg, ARG_TYPES ...arg_values)
{
  if (!format || !*format) return;

  switch (*format)
  {
    case '\\':
      LOG(*(format + 1));
      LOG(format + 2, arg, arg_values...);
      break;
    case '%':
      LOG(arg);
      LOG(format + 1, arg_values...);
      break;
    default:
      LOG(*format);
      LOG(format + 1, arg, arg_values...);
      break;
  }
}

/// LOG wrapper for file, line and highlight information.
/// @tparam ...ARG_TYPES additional arguments to be printed.
/// @param file the source file of the log.
/// @param line the source line of the log.
/// @param highlight additional highlight.
/// @param format the format to be printed.
/// @param ...arg_values additional arguments to be printed.
template<typename ...ARG_TYPES>
constexpr void LOG(unsigned long time,
                    const char *file, int line,
                    const char *highlight,
                    const char* format, ARG_TYPES ...arg_values)
{
  // Log debug source information and set console styling.
  LOG("%.%s @%:%: %", time / 1000000UL, time % 1000000UL, file, line, highlight);
  // Print formatted output.
  LOG(format, arg_values...);
  // Reset console styling.
  LOG("\033[0m\n");
}

/// Forwarding simple LOG calls, such as D and I, with several debug information
/// (such as timestamps and source - file, line - information).
#define LOGFWD(...) \
  LOG(micros(), __FILE__, __LINE__, __VA_ARGS__)

/// Debug style wrapper.
/// - Write D(value) to log the value.
/// - Write D(format, values...) to log the format with the values.
/// - '%' substitutes the next value in place.
/// - '\\' can be used as an escape character.
#define D(FORMAT, ...) \
  LOGFWD("\033[37;44;1m D \033[0m\033[34m ", FORMAT, ##__VA_ARGS__)

/// Info style wrapper.
/// - Write I(value) to log the value.
/// - Write I(format, values...) to log the format with the values.
/// - '%' substitutes the next value in place.
/// - '\\' can be used as an escape character.
#define I(FORMAT, ...) \
  LOGFWD("\033[37;42;1m I \033[0m\033[32m ", FORMAT, ##__VA_ARGS__)

/// Warning style wrapper.
/// - Write W(value) to log the value.
/// - Write W(format, values...) to log the format with the values.
/// - '%' substitutes the next value in place.
/// - '\\' can be used as an escape character.
#define W(FORMAT, ...) \
  LOGFWD("\033[30;43;1m W \033[0m\033[33m ", FORMAT, ##__VA_ARGS__)

/// Error style wrapper.
/// - Write E(value) to log the value.
/// - Write E(format, values...) to log the format with the values.
/// - '%' substitutes the next value in place.
/// - '\\' can be used as an escape character.
#define E(FORMAT, ...) \
  LOGFWD("\033[30;41;1m E \033[0m\033[31m ", FORMAT, ##__VA_ARGS__)

} // namespace LOG

} // namespace PTS

#endif // UTILS_SW_LOG_H