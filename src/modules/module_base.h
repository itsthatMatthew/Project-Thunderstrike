//===-- modules/module_base.h - Module base class definition --------------===//
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
/// Modules can start new threads that are bound to them, making the usage
/// as robust as possible. Each object may have a single task running.
///
/// The Module class is threadsafe
///
//===----------------------------------------------------------------------===//

#ifndef MODULES_MODULE_BASE_H
#define MODULES_MODULE_BASE_H

#include <mutex>
#include <string>
#include "utils/sw/log.h"
#include <Arduino.h>

namespace PTS
{

/// Module base class.
/// \tparam STACK_DEPTH the desired stack depth in words (defaults to 1024).
/// \tparam PRIORITY the desired task priority (defaults to tskIDLE_PRIORITY).
/// \tparam FREQUENCY the desired task frequency in HZ (defaults to 10).
template<uint32_t STACK_DEPTH = 1024,
         uint32_t PRIORITY = tskIDLE_PRIORITY,
         uint32_t FREQUENCY = 10>
class Module
{
//===-- Instantiation specific functions ----------------------------------===//

 public:
  /// \param module_name the object's and starting thread's name.
  explicit Module(const std::string &module_name)
    : c_module_name(module_name),
      m_task_handle(nullptr),
      m_handle_lock(/*default*/)
  { LOG::I("Module \"%\" constructed.", module_name.c_str()); }
  
  /// Virtual destructor.
  virtual ~Module() = default;

  /// Deleted copy ctor and assignment operator - a module should not be copied.
  Module(const Module&) = delete;
  Module& operator=(const Module&) = delete;

  /// Setup function for additional component initializations.
  virtual void begin() const = 0;

  /// \return the module's name.
  [[nodiscard]] std::string getName() const { return c_module_name; }

//===-- Threading specific functions --------------------------------------===//

  /// Thread worker function that gets executed in a loop as start() is called.
  virtual void threadFunc() const = 0;

  /// Creates a thread for the object running threadFunc().
  void start() const
  {
    std::lock_guard<std::mutex> lock(m_handle_lock);
    // only start new thread if none exists yet
    if (!m_task_handle)
    {
      xTaskCreate(
        [](void *obj) constexpr // wrapper lambda
        {
          uint32_t last_tick = xTaskGetTickCount();
          for(;;) // runs the threadFunc() in an infinite loop
          {
            static_cast<decltype(this)>(obj)->threadFunc();
            if (pdFALSE == xTaskDelayUntil(&last_tick, configTICK_RATE_HZ / FREQUENCY))
              LOG::W("Module \"%\" was not delayed (frequency set to %)!",
                     static_cast<decltype(this)>(obj)->c_module_name.c_str(),
                     FREQUENCY);
          }
        },
        c_module_name.c_str(),
        STACK_DEPTH,
        const_cast<Module*>(this), // remove const qualifyer (API needs void*)
        PRIORITY,
        &m_task_handle
      );
      LOG::I("Module \"%\" started.", c_module_name.c_str());
    }
  }
  // Explainer about the black magic fuckery going on inside the function:
  // Since the xTaskCreate(...) function takes a function pointer to run and
  // the parameters as a void*, to be able to override the threadFunc() method
  // yet still run it, some odd decisions had to be made.
  // The wrapper lambda is the function to be ran, so it cannot have any capture
  // clause, and takes a void* as its single parameter. Since the parameter is
  // the parent object of the thread, we want its overriden threadFunc() to be
  // ran, so it's casted back into its "original" type from void*, then its
  // threadFunc() is called.
  // The xTaskCreate() functions "pvParameters" argument is then this parent
  // object, which is achieved with removing the const qualifyer (the method is
  // marked const), so that the lambda can use it.

  /// Suspends the object's thread.
  void suspend() const
  {
    std::lock_guard<std::mutex> lock(m_handle_lock);

    if (m_task_handle)
    {
      vTaskSuspend(m_task_handle);
      LOG::I("Module \"%\" suspended.", c_module_name.c_str());
    }
  }

  /// Resumes the object's thread.
  void resume() const
  {
    std::lock_guard<std::mutex> lock(m_handle_lock);

    if (m_task_handle)
    {
      vTaskResume(m_task_handle);
      LOG::I("Module \"%\" resumed.", c_module_name.c_str());
    }
  }

  /// Deletes the object's thread.
  void destroy() const
  {
    std::lock_guard<std::mutex> lock(m_handle_lock);

    if (m_task_handle)
    {
      vTaskDelete(m_task_handle);
      m_task_handle = nullptr;
      LOG::I("Module \"%\" destroyed.", c_module_name.c_str());
    }
  }

//===-- Member variables --------------------------------------------------===//

 private:
  const std::string c_module_name;
  mutable TaskHandle_t m_task_handle;
  mutable std::mutex m_handle_lock;
}; // class Module

} // namespace PTS

#endif // MODULES_MODULE_BASE_H