//===-- module/module.h - Module base class definition --------------------===//
//
// Project-Thunderstrike (PTS) collection header file.
// Find more information at:
// https://github.com/itsthatMatthew/Project-Thunderstrike
//
//===----------------------------------------------------------------------===//
///
/// \file This file contains the declarations of the Module class, which is the
/// base class for all of the PTS module classes.
///
/// Modules are state machines for a robust functionality.
/// States have a predefined flow that changes depending on whether the module
/// failed or passed.
///
//===----------------------------------------------------------------------===//

#ifndef MODULE_MODULE_H
#define MODULE_MODULE_H

#include <mutex>
#include <Arduino.h>

namespace PTS
{

/// Module base class.
/// \tparam STACK_DEPTH the desired stack depth.
/// \tparam PRIORITY the desired task priority.
template<uint32_t STACK_DEPTH = 1024, uint32_t PRIORITY = tskIDLE_PRIORITY>
class Module
{
 public:
  /// Enumerated values storing the module's possible state.
  enum ModuleState : uint8_t
  {
    INVALID = 0b00,
    ACTIVE  = 0b01,
    PASSED  = 0b10,
    FAILED  = 0b11,
  };

//===-- Instantiation specific functions ----------------------------------===//

  /// \param name the object's name - it will be passed to the starting thread.
  explicit Module(const char *const name)
    : c_name(name),
      m_state(INVALID),
      m_state_lock(/*default*/),
      m_task_handle(nullptr),
      m_handle_lock(/*default*/)
  { }
  
  /// Virtual destructor.
  virtual ~Module() = default;

  /// Deleted copy ctor and assignment operator - a module should not be copied.
  Module(const Module&) = delete;
  Module& operator=(const Module&) = delete;

  /// Setup function for additional component initializations.
  virtual void begin() const = 0;

//===-- State and transition specific functions ---------------------------===//

  /// \return the module's current state.
  [[nodiscard]] ModuleState getState() const { return m_state; }

  /// \return the module's name.
  [[nodiscard]] char *const getName() const { return c_name; }

  /// Resets the module's state to INVALID.
  void invalidateState() const { m_state = INVALID; }

  /// Advances the module's state in a "passing" way.
  /// \return the module's new state
  ModuleState passState() const
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
  /// \return the module's new state
  ModuleState failState() const
  {
    std::lock_guard<std::mutex> lock(m_state_lock);

    switch (m_state)
    {
      case ACTIVE: m_state = FAILED; break;
      default: /*do nothing*/ break;
    }
    return m_state;
  }

//===-- Threading specific functions --------------------------------------===//

  /// Thread worker function that gets executed in a loop as start() is called.
  virtual void threadFunc() const = 0;

  /// Creates a thread for the object running threadFunc().
  void start() const
  {
    std::lock_guard<std::mutex> lock(m_handle_lock);
    // only start new thread if none exists yet and the module is active
    if (!m_task_handle)
    {
      xTaskCreate(
        [](void *obj) constexpr // wrapper lambda
        {
          for(;;) // runs the threadFunc() in an infinite loop
          { 
            static_cast<decltype(this)>(obj)->threadFunc();
          }
        },
        c_name,
        STACK_DEPTH,
        const_cast<Module*>(this), // remove const qualifyer (API needs void*)
        PRIORITY,
        &m_task_handle
      );
    }
  }

  /// Suspends the object's thread.
  void suspend() const {
    std::lock_guard<std::mutex> lock(m_handle_lock);

    if (m_task_handle)
    {
      vTaskSuspend(m_task_handle);
    }
  }

  /// Resumes the object's thread.
  void resume() const {
    std::lock_guard<std::mutex> lock(m_handle_lock);

    if (m_task_handle)
    {
      vTaskResume(m_task_handle);
    }
  }

  /// Deletes the object's thread.
  void destroy() const {
    std::lock_guard<std::mutex> lock(m_handle_lock);

    if (m_task_handle)
    {
      vTaskDelete(m_task_handle);
      m_task_handle = nullptr;
    }
  }

//===-- Member variables --------------------------------------------------===//

 private:
  const char *const c_name;
  mutable ModuleState m_state;
  mutable std::mutex m_state_lock;
  mutable TaskHandle_t m_task_handle;
  mutable std::mutex m_handle_lock;
}; // class Module

} // namespace PTS

#endif // MODULE_MODULE_H