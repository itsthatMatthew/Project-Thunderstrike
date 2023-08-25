//===-- modules/stateful_base.h - Stateful class definitions ---------------==//
//
// Project-Thunderstrike (PTS) collection header file.
// Find more information at:
// https://github.com/itsthatMatthew/Project-Thunderstrike
//
//===----------------------------------------------------------------------===//
///
/// \file This file contains the declarations of the Stateful class, which is a
/// special base class mainly for specific PTS module classes.
///
/// Stateful derivations let modules be state machines for robust functionality.
/// States have a predefined flow that changes depending on whether the module
/// failed or passed.
///
/// The Stateful class is threadsafe.
///
//===----------------------------------------------------------------------===//

#ifndef MODULES_STATEFUL_BASE_H
#define MODULES_STATEFUL_BASE_H

#include <mutex>

namespace PTS
{

class Stateful
{
 public:
  /// Enumerated values storing the module's possible state.
  enum State : uint8_t
  {
    INVALID = 0b00,
    ACTIVE  = 0b01,
    PASSED  = 0b10,
    FAILED  = 0b11,
  };

  explicit Stateful() : m_state(INVALID), m_state_lock(/*default*/) { }

//===-- State and transition specific functions ---------------------------===//

  /// \return the module's current state.
  [[nodiscard]] State getState() const { return m_state; }

  /// Resets the module's state to INVALID.
  void invalidateState() const { m_state = INVALID; }

  /// Makes the module's state active only if it has been in an invalid state.
  /// \return the module's new state. 
  State makeActive() const
  {
    if (m_state == INVALID)
      m_state = ACTIVE;
    return m_state;
  } 

  /// Advances the module's state in a "passing" way.
  /// \return the module's new state.
  State passState() const
  {
    std::lock_guard<std::mutex> lock(m_state_lock);

    switch (m_state)
    {
      case INVALID: m_state = ACTIVE; break;
      case ACTIVE: m_state = PASSED; break;
      default: /*do nothing*/ break;
    }
    return m_state;
  }

  /// Advances the module's state in a "failing" way.
  /// \return the module's new state.
  State failState() const
  {
    std::lock_guard<std::mutex> lock(m_state_lock);

    switch (m_state)
    {
      case ACTIVE: m_state = FAILED; break;
      default: /*do nothing*/ break;
    }
    return m_state;
  }

 private:
  mutable State m_state;
  mutable std::mutex m_state_lock;
}; // class Stateful

} // namespace PTS

#endif // MODULES_STATEFUL_BASE_H