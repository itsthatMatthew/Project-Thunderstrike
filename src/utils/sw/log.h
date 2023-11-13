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
#define DEBUG   0
#define INFO    1
#define WARNING 2
#define ERROR   3
#define NOTHING 4

#ifndef LOGLVL
#define LOGLVL DEBUG
#endif

/// Constexpr implementation of the strlen function
/// \param string the string (C-style char*) thats length should be determined.
/// \param length accumulator for length, should not be modified.
/// \return the length of the string.
constexpr size_t constexpr_strlen(const char *string, const size_t length = 0)
{
  return (!string || !*string)
    ? length
    : constexpr_strlen(string + 1, length + 1);
}

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

/// Logs simple space based tabulation with the given width.
/// \param width the width of the tabulation.
/// \param markers the width at which markers ('-') should appear.
constexpr void LOG_TAB(const size_t width, const size_t markers = 4)
{
  if (width == 0) return;

  if (width % markers)
    LOG(" ");
  else
    LOG("-");

  LOG_TAB(width - 1);
}

/// Logs a string width the specified max width
/// \param string the string (C-style char*) that should be logged.
/// \param max_width the max width for the string.
constexpr void LOG_MAX_WIDTH_STRING(const char *string, const size_t max_width)
{
  if (!string || !*string) return;

  if (max_width != 0)
  {
    LOG(*string);
    LOG_MAX_WIDTH_STRING(string + 1, max_width - 1);
  }
}

/// Logs a string width the specified 
/// \param string the string to be logged.
/// \param fixed_width teh desired fixed width.
/// \param slice the width at which the string should be sliced at ("...").
constexpr void LOG_FIXED_WITH_STRING(const char *string,
                                     const size_t fixed_width,
                                     const size_t slice = 19)
{
  const size_t string_width = constexpr_strlen(string);
  if (string_width > fixed_width)
  {
    LOG_MAX_WIDTH_STRING(string, slice);
    LOG("...");
    LOG_MAX_WIDTH_STRING(string + string_width - fixed_width + slice + 3,
                         fixed_width - slice - 3);
  }
  else
  {
    LOG(string);
    LOG_TAB(fixed_width - string_width);
  }
}

/// LOG wrapper for time, file, line and highlight information.
/// \tparam ...ARG_TYPES additional arguments to be printed.
/// \param time the source time of the log.
/// \param source the source file and line of the log.
/// \param highlight additional highlight.
/// \param format the format to be printed.
/// \param ...arg_values additional arguments to be printed.
template<typename ...ARG_TYPES>
constexpr void LOG_SRC(unsigned long time,
                    const char *source,
                    const char *highlight,
                    const char* format, ARG_TYPES ...arg_values)
{
  // Log time information
  LOG("%.%s  @", time / 1000000UL, time % 1000000UL / 1000UL);
  // Log the source file-line location (width a fixed width for readability)
  LOG_FIXED_WITH_STRING(source, 51);
  // Set highlight & coloring
  LOG(highlight);
  // Print formatted output.
  LOG(format, arg_values...);
  // Reset console styling.
  LOG("\033[0m\n");
}

/// Literally does nothing (used for disabled level macros)
constexpr void DISABLED_LOG_FUNCTION() { }

/// Helper macro to turn numeric literals into strings.
#define ITOS_(i) #i
/// Macro to turn numeric literals into strings.
#define ITOS(i) ITOS_(i)

/// Forwarding simple LOG calls, such as D and I, with several debug information
/// (such as timestamps and source - file, line - information).
#define LOGFWD(...) \
  LOG_SRC(micros(), __FILE__ ":" ITOS(__LINE__) ": ", __VA_ARGS__)

/// Debug style wrapper.
/// - Write D(value) to log the value.
/// - Write D(format, values...) to log the format with the values.
/// - '%' substitutes the next value in place.
/// - '\\' can be used as an escape character.
#if LOGLVL > DEBUG
  #define D(...) DISABLED_LOG_FUNCTION()
#else
  #define D(FORMAT, ...) \
    LOGFWD("\033[37;44;1m D \033[0m\033[34m ", FORMAT, ##__VA_ARGS__)
#endif

/// Info style wrapper.
/// - Write I(value) to log the value.
/// - Write I(format, values...) to log the format with the values.
/// - '%' substitutes the next value in place.
/// - '\\' can be used as an escape character.
#if LOGLVL > INFO
  #define I(...) DISABLED_LOG_FUNCTION()
#else
#define I(FORMAT, ...) \
  LOGFWD("\033[37;42;1m I \033[0m\033[32m ", FORMAT, ##__VA_ARGS__)
#endif

/// Warning style wrapper.
/// - Write W(value) to log the value.
/// - Write W(format, values...) to log the format with the values.
/// - '%' substitutes the next value in place.
/// - '\\' can be used as an escape character.
#if LOGLVL > WARNING
  #define W(...) DISABLED_LOG_FUNCTION()
#else
#define W(FORMAT, ...) \
  LOGFWD("\033[30;43;1m W \033[0m\033[33m ", FORMAT, ##__VA_ARGS__)
#endif

/// Error style wrapper.
/// - Write E(value) to log the value.
/// - Write E(format, values...) to log the format with the values.
/// - '%' substitutes the next value in place.
/// - '\\' can be used as an escape character.
#if LOGLVL > ERROR
  #define E(...) DISABLED_LOG_FUNCTION()
#else
#define E(FORMAT, ...) \
  LOGFWD("\033[30;41;1m E \033[0m\033[31m ", FORMAT, ##__VA_ARGS__)
#endif

} // namespace LOG

} // namespace PTS

#endif // UTILS_SW_LOG_H