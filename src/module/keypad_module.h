//===-- module/keypad_module.h - Keypad class definition ------------------===//
//
// Project-Thunderstrike (PTS) collection header file.
// Find more information at:
// https://github.com/itsthatMatthew/Project-Thunderstrike
//
//===----------------------------------------------------------------------===//
///
/// \file This file contains the declarations of the Keypad class, which is a
/// wrapper class to handle compley keypad communications.
///
/// Keypad dimensions, character set and buffer size can be set as template
/// arguments. The module constantly polls for new keypresses, and registeres
/// them as characters in a FIFO, the contents of which can be then read.
///
//===----------------------------------------------------------------------===//

#ifndef MODULE_KEYPAD_MODULE_H
#define MODULE_KEYPAD_MODULE_H

#include <mutex>
#include <array>
#include <queue>
#include "module.h"

namespace PTS
{

/// Keypad class
/// @tparam COLS the number of columns on the physical keypad. 
/// @tparam ROWS the number of rows on the physical keypad. 
template<size_t COLS, size_t ROWS>
class Keypad : public Module<>
{
//===-- Instantiation specific functions ----------------------------------===//
 public:
  explicit Keypad(const char *const name,
                  std::array<uint8_t, COLS> &&col_pins,
                  std::array<uint8_t, ROWS> &&row_pins,
                  std::array<std::array<char, COLS>, ROWS> &&char_set)
  : Module(name),
    c_col_pins(col_pins),
    c_row_pins(row_pins),
    c_char_set(char_set),
    m_input_buffer(),
    m_buffer_lock()
  { }

  /// Sets up the pins to their correspondig modes.
  void begin() const override
  {
    for (auto col_pin : c_col_pins)
      pinMode(col_pin, OUTPUT);

    for (auto row_pin : c_row_pins)
      pinMode(row_pin, INPUT);
  }

//===-- Input handling functions ------------------------------------------===//

// TODO: stack based circular buffer implementation, std::queue for now (heap!)
// POSSIBLE CIRCULAR INPUT BUFFER EXPLAINER
//
//  default pos of m_buffer_write_ptr
//  |       after writing 4 values
//  |       |
//  v       v
// +-+-+-+-+-+-+-+ cell amount set by BUFFER_SIZE
// |r|r|w|w|*|*|*|
// +-+-+-+-+-+-+-+ ptrs wrap around at the end of buffer
//  ^   ^
//  |   |
//  |   after reading 2 values
//  default pos of m_buffer_read_ptr

// TODO: button stlye state implementation
// the current crude approach fills up the queue fast as it's only concerned
// with high states: only rising edges should be registered

  /// Loops trough the pins to determine which one is pressed.
  void threadFunc() const override
  {
    for (size_t col_num = 0; col_num != COLS; col_num++)
    {
      digitalWrite(c_col_pins[col_num], HIGH);

      for (size_t row_num = 0; row_num != ROWS; row_num++)
        if (digitalRead(c_row_pins[row_num]) == HIGH)
          writeOne(c_char_set[row_num][col_num]);

      digitalWrite(c_col_pins[col_num], LOW);
    }
  }

  /// Writes one input character into the buffer.
  /// @param input_char the character to be written.
  void writeOne(char input_char) const
  {
    std::lock_guard<std::mutex> lock(m_buffer_lock);

    m_input_buffer.push(input_char);
  }

  /// Reads one input character from the buffer.
  /// @return the next character.
  char readOne() const
  {
    std::lock_guard<std::mutex> lock(m_buffer_lock);

    auto temp = m_input_buffer.front();
    m_input_buffer.pop();
    return temp;
  }

  /// Returns true, if there is at least one character to be read.
  operator bool()
  {
    std::lock_guard<std::mutex> lock(m_buffer_lock);

    return !m_input_buffer.empty();
  }

//===-- Member variables --------------------------------------------------===//

 private:
  const std::array<uint8_t, COLS> c_col_pins;
  const std::array<uint8_t, ROWS> c_row_pins;
  std::array<std::array<char, COLS>, ROWS> c_char_set;
  mutable std::queue<char> m_input_buffer;
  mutable std::mutex m_buffer_lock;
}; // class Keypad

} // namespace PTS

#endif // MODULE_KEYPAD_MODULE_H