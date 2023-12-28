//===-- net/web_server.h - WebServer class definition -=======================//
//
// Project-Thunderstrike (PTS) collection header file.
// Find more information at:
// https://github.com/itsthatMatthew/Project-Thunderstrike
//
//===----------------------------------------------------------------------===//
///
/// \file This file contains the declarations of the WebServer class, which is
/// a singleton wrapper for simple http web server functionality.
/// The site rendered contains a table of name (key) - value - description (opt)
/// with minimal styling.
///
//===----------------------------------------------------------------------===//

#ifndef NET_WEB_SERVER_H
#define NET_WEB_SERVER_H

#include <mutex>
#include <map>
#include <string>
#include <optional>

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "modules/module_base.h"
#include "utils/sw/log.h"

#include "website.h"

namespace PTS
{

/// WebServer singleton class
class WebServer : public Module<4*1024, tskIDLE_PRIORITY, 2>
{
 private:
  /// Private constructor to implement singleton behaviour.
  explicit WebServer()
    : Module("webserver_module"),
      web_server(80),
      m_attributes(),
      m_attributes_lock()
  { }

 public:
  /// Returns the static instance of the WebServer as a const reference.
  static const WebServer& instance()
  {
    static WebServer instance_;
    return instance_;
  }

  /// Registers an attribute to be displayed on the site.
  /// \param name the unique name of the attribute.
  /// \param value the value of the attribute.
  /// \param description the description of the attribute.
  /// \return false, if the attribute already exists, true otherwise.
  bool registerAttribute(const std::string &name,
                         const std::string &value = "nil",
                         const std::string &description = "") const
  {
    std::lock_guard<std::mutex> lock(m_attributes_lock);

    if (m_attributes.find(name) != m_attributes.end()) return false;

    m_attributes[name] = std::make_pair(value, description);
    return true;
  }

  /// Updates a registered attribute.
  /// \param name the name of the attribute to be updated.
  /// \param value the value of the updated attribute.
  /// \return false, if the attribute does not exist, true otherwise.
  bool updateAttribute(const std::string &name,
                       const std::string &value) const
  {
    std::lock_guard<std::mutex> lock(m_attributes_lock);

    if (m_attributes.find(name) == m_attributes.end()) return false;

    m_attributes[name] = std::make_pair(value, m_attributes[name].second);
    return true;
  }

  /// Updates an attribute if it exists, or registers it otherwise.
  /// \param name the name of the attribute.
  /// \param value the value of the attribute.
  /// \param description the description of the attribute (does not update).
  void upsterAttribute(const std::string &name,
                       const std::string &value,
                       const std::string &description = "") const
  {
    std::lock_guard<std::mutex> lock(m_attributes_lock);
    
    if (m_attributes.find(name) == m_attributes.end())
      m_attributes[name] = std::make_pair(value, description);
    else
      m_attributes[name] = std::make_pair(value, m_attributes[name].second);
  }

  /// Returns the value of an attribute.
  /// \param name the name of the value to be returned.
  /// \return an optional storing the value, empty if it is not registered.
  std::optional<std::string> readAttribute(const std::string &name) const
  {
    std::lock_guard<std::mutex> lock(m_attributes_lock);

    if (m_attributes.find(name) == m_attributes.end()) return {};

    return {m_attributes[name].first};
  }

  /// Deletes a registered attribute.
  /// \param name the name of the attribute.
  /// \return false if the attribute does not exist, true otherwise. 
  bool deleteAttribute(const std::string &name) const
  {
    std::lock_guard<std::mutex> lock(m_attributes_lock);

    if (m_attributes.find(name) == m_attributes.end()) return false;

    m_attributes.erase(name);
    return true;
  }

  void begin() const override
  {
    LOG::I("Setting up AP...");

    WiFi.softAP("PTS-AP", nullptr);

    web_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      request->send(200, "html", website_content);
    });

    web_server.on("/getAttributes", HTTP_GET, [&](AsyncWebServerRequest *request)
    {
      std::string json_response = "[";

      for (const auto attribute : m_attributes)
      {
        const auto [name, data] = attribute;
        const auto [value, desc] = data;

        json_response += std::string{"{"} +
          "\"name\":\"" + name + "\"," +
          "\"value\":\"" + value + "\"," +
          "\"desc\":\"" + desc + "\"" +
        "},";
      }

      json_response.pop_back(); // remove trailing comma
      json_response += "]";

      request->send(200, "json", json_response.c_str());
    });

    web_server.onNotFound([](AsyncWebServerRequest *request)
    {
      request->send(404, "text/plain", "Not found");
    });

    web_server.begin();

    LOG::I("AP created, IP: %", WiFi.localIP());

    Module::begin();
  }

  void threadFunc() const override
  { }

 private:
  mutable AsyncWebServer web_server;
  mutable std::map<std::string, std::pair<std::string, std::string>> m_attributes;
  mutable std::mutex m_attributes_lock;
}; // class WebServer

} // namesapce PTS

#endif // NET_WEB_SERVER_H