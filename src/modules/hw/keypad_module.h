//===-- modules/hw/keypad_module.h - Keypad class definition --------------===//
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

#ifndef MODULES_HW_KEYPAD_MODULE_H
#define MODULES_HW_KEYPAD_MODULE_H

#include <mutex>
#include <array>
#include <initializer_list>
#include <optional>
#include "modules/module_base.h"
#include "utils/hw/button.h"
#include "utils/sw/circular_buffer.h"

namespace PTS
{

/// Keypad class
/// \tparam COLS the number of columns on the physical keypad. 
/// \tparam ROWS the number of rows on the physical keypad.
template<size_t COLS, size_t ROWS>
class Keypad : public Module<2048>
{
  /// Inner class for hardware wrapping.
  class PoweredButton
  {
   public:
    // Nulled default init for default ctor (std::array).
    explicit PoweredButton(const size_t col_num = 0,
                  const size_t row_num = 0,
                  const uint8_t power_pin = 0,
                  const uint8_t read_pin = 0)
    : c_button(read_pin),
      c_col_num(col_num),
      c_row_num(row_num),
      c_power_pin(power_pin)
    { }

    /// Sets up the power pin, the Button wrapper and callback.
    void begin() const
    {
      pinMode(c_power_pin, OUTPUT);
      c_button.begin();
      c_button.onRising([](const Keypad* handler,
                            const size_t col_num,
                            const size_t row_num)
      {
        handler->writeOne(col_num, row_num);
      });
    }

    /// Sets the power pin high, updates, then low.
    /// \param handler the "parent" Keypad needed for its writeOne() function.
    void update(const Keypad* handler) const
    {
      digitalWrite(c_power_pin, HIGH);
      c_button.update(handler, c_col_num, c_row_num);
      digitalWrite(c_power_pin, LOW);
    }

   private:
    const Button<100, void, const Keypad*, const size_t, const size_t> c_button;
    const size_t c_col_num;
    const size_t c_row_num;
    const uint8_t c_power_pin;
  }; // class PoweredButton

//===-- Instantiation specific functions ----------------------------------===//
 public:
  explicit Keypad(const std::string &module_name,
                  std::initializer_list<uint8_t> col_pins,
                  std::initializer_list<uint8_t> row_pins,
                  std::array<std::array<char, COLS>, ROWS> &&char_set)
  : Module(module_name),
    m_buttons(),
    c_char_set(char_set),
    m_input_buffer(),
    m_buffer_lock()
  {
    // For each row and column it sets up the buttons of the matrxi grid as
    // dedicated PoweredButton objects.
    // As the PoweredButton (specifically its Button member) is not trivially
    // copiable, the nuclear option of overwriting the default constructed
    // (nulled) one with placement new.
    for (size_t col_num = 0; col_num != COLS; col_num++)
      for (size_t row_num = 0; row_num != ROWS; row_num++)
        ::new (&m_buttons[row_num][col_num])
        PoweredButton(col_num,
              row_num,
              *(col_pins.begin() + col_num),
              *(row_pins.begin() + row_num));
  }

  /// Sets up the buttons.
  void begin() const override
  {
    for (size_t col_num = 0; col_num != COLS; col_num++)
      for (size_t row_num = 0; row_num != ROWS; row_num++)
        m_buttons[row_num][col_num].begin();
        
    Module::begin();
  }

//===-- Input handling functions ------------------------------------------===//

  /// Loops trough the buttons to determine which one is pressed.
  void threadFunc() const override
  {
    for (size_t col_num = 0; col_num != COLS; col_num++)
      for (size_t row_num = 0; row_num != ROWS; row_num++)
        m_buttons[row_num][col_num].update(this);
  }


  /// Reads one input character from the buffer.
  /// \return the next character as std::optional (empty if the buffer is too).
  std::optional<char> readOne() const
  {
    std::lock_guard<std::mutex> lock(m_buffer_lock);

    std::optional<char> temp{};
    if (!m_input_buffer.empty())
    {
      temp = m_input_buffer.front();
      m_input_buffer.pop();
    }
    return temp;
  }

  /// Returns true, if there is at least one character to be read.
  operator bool()
  {
    std::lock_guard<std::mutex> lock(m_buffer_lock);

    return !m_input_buffer.empty();
  }

 private:

  /// Writes one input character into the buffer.
  /// \param col the column number in the character set.
  /// \param row the row number in the character set.
  void writeOne(const size_t col, const size_t row) const
  {
    std::lock_guard<std::mutex> lock(m_buffer_lock);

    m_input_buffer.push(c_char_set[row][col]);

    LOG::D("New value in keypad buffer: %", c_char_set[row][col]);
  }

//===-- Member variables --------------------------------------------------===//

 private:
  // Matrix of the buttons on the keypad.
  std::array<std::array<PoweredButton, COLS>, ROWS> m_buttons;
  // The available character set on the keypad.
  std::array<std::array<char, COLS>, ROWS> c_char_set;
  // The input buffer.
  mutable CircularBuffer<char, 16> m_input_buffer;
  // Mutex for the locking of the input buffer.
  mutable std::mutex m_buffer_lock;
}; // class Keypad

} // namespace PTS

#endif // MODULES_HW_KEYPAD_MODULE_H