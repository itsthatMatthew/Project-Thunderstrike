//===-- main.cpp - Entry point of the application --==========================//
//
// Project-Thunderstrike (PTS) collection source file.
// Find more information at:
// https://github.com/itsthatMatthew/Project-Thunderstrike
//
//===----------------------------------------------------------------------===//
///
/// \file This file contains an example definition of the application entry
/// point with the Arduino framework.
/// This example also utilizes a lot of already existing parts project.
///
//===----------------------------------------------------------------------===//

#include <Arduino.h>
#include "utils/sw/log.h" // Logging header
#include "net/web_server.h" // WebServer header
#include "modules/hw/keypad_module.h" // Keypad module

// Set up a web server and get its instance.
const PTS::WebServer& web_server = PTS::WebServer::instance();

// Create an instance of the keypad module, with the corresponding pins set.
const PTS::Keypad<3, 4> keypad_module("keypad_module",
                                      {25, 33, 32}, {35, 34, 39, 36},
                                      std::array<std::array<char, 3>, 4>{
                                        std::array<char, 3>{'1', '2', '3'},
                                        std::array<char, 3>{'4', '5', '6'},
                                        std::array<char, 3>{'7', '8', '9'},
                                        std::array<char, 3>{'*', '0', '#'}});

// Setup: the entry point of the application.
void setup() {
  // Set up serial port.
  // The MONITOR_SPEED is initialized in the platformio.ini file.
  // This should be done before any print call.
  Serial.begin(MONITOR_SPEED);

  // Log that init started, this shows up on the serial monitor.
  PTS::LOG::D("Initializing started...");

  // Use the webserver to register a new attribute to be displayed.
  web_server.registerAttribute("example_attribute",
                               "example_value",
                               "This is an example attribute.");

  // Setup the keypad and webserver module.
  keypad_module.begin();
  web_server.begin();

  // Register an attribute that will contain the data read by the keypad.
  web_server.registerAttribute("keypad_buffer_content", "");

  // Start the modules on new threads.
  keypad_module.start();
  web_server.start();

  PTS::LOG::I("Initializing finished.");
}

// Loop: run in succession after the setup function finished.
void loop() {
  // Update or register (the first time loop will run) attribute.
  web_server.upsterAttribute("seconds",
                             std::to_string(millis() / 1000),
                             "Seconds since startup.");

  // Try to read a new value from the keypad. If successful, append it.
  if (auto keypad_buffer = keypad_module.readOne(); keypad_buffer)
  {
    // Read the value of the attribute.
    char buffer_value = keypad_buffer.value();
    auto attribute = web_server.readAttribute("keypad_buffer_content").value();

    switch (buffer_value)
    {
    case '*': if (!attribute.empty()) attribute.pop_back(); break; // Backspace.
    case '#': attribute.clear(); break; // Clear all.
    default: attribute += keypad_buffer.value(); break; // Append.
    }

    // Update the attribute.
    web_server.updateAttribute("keypad_buffer_content", attribute);
  }
}