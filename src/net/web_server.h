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
#include "modules/module_base.h"
#include "utils/sw/log.h"

namespace PTS
{

/// WebServer class
class WebServer : public Module<4*1024, tskIDLE_PRIORITY, 2>
{
 private:
  explicit WebServer()
    : Module("webserver_module"),
      wifi_server(WiFiServer(80)),
      ip_address(),
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
                         const std::string &description = "nil") const
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
                       const std::string &description = "nil") const
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
    ip_address = WiFi.softAPIP();
    wifi_server.begin();
    LOG::I("AP created, IP: %", ip_address);
  }

  void threadFunc() const override
  {
    WiFiClient client = wifi_server.available();
    
    if (client)
    {
      LOG::D("New client connected @ %:%", client.remoteIP(), client.remotePort());
      String current_line = "";

      while (client.connected())
      {
        if (client.available())
        {
          char c = client.read();

          if (c == '\n')
          {
            if (current_line.length() == 0) {
            std::lock_guard<std::mutex> lock(m_attributes_lock);

              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();

              client.println("<!DOCTYPE html><html>");
              client.println("<head>");
              client.println("<title>Title of the document</title>");
              client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<style>body{font-family:Arial;background-color:#2c3e50;color:#ecf0f1;}table{font-size:3vw;text-align:center;}th{font-weight:bold;}td{font-weight:normal;padding:1vw;border-top:solid#ecf0f1;}</style>");
              client.println("</head>");
              client.println("<body>");
              client.println("<h1>ESP32 Web Server</h1>");
              client.print("<h2>Registered attributes:<h2>");
              client.println("<table>");
              client.println("<tr><th>Name</th><th>Value</th><th>Description</th></tr>");
              for (auto attribute : m_attributes)
              {
                client.print("<tr>");
                client.print("<td>");
                client.print(attribute.first.c_str());
                client.print("</td>");
                client.print("<td>");
                client.print(attribute.second.first.c_str());
                client.print("</td>");
                client.print("<td>");
                client.print(attribute.second.second.c_str());
                client.print("</td>");
                client.println("</tr>");
              }
              client.println("</table>");
              client.println("</body>");
              client.println("</html>");
              client.println();

              break;
            }
            else 
            {
              current_line = "";
            }
          }
          else if (c != '\r')
          {
            current_line += c;
          }
        }
      }

      client.stop();
      LOG::D("Client disconnected.");
    }
  }

 private:
  mutable WiFiServer wifi_server;
  mutable IPAddress ip_address;
  mutable std::map<std::string, std::pair<std::string, std::string>> m_attributes;
  mutable std::mutex m_attributes_lock;
}; // class WebServer

} // namesapce PTS

#endif // NET_WEB_SERVER_H