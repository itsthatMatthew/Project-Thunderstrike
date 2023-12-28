# Project-Thunderstrike collection python script.
# Find more information at:
# https://github.com/itsthatMatthew/Project-Thunderstrike

# This script is used to turn an html source file into a C style header to be
# used in the project as a variable necessary for an ESP32 based web server. 

Import("env") # used by pio build to install necessary packages

try:
    from bs4 import BeautifulSoup, Comment
except ImportError:
    env.Execute("$PYTHONEXE -m pip install BeautifulSoup4")

# Modify these values if the path or filename changes
Path = './src/net/'
Filename = 'website'

# Modify these values to influence the script's behaviour
IncludeGuard = 'NET_WEBSITE_H' # C style header include guard
RawStringGuard = '###' # Raw string guard for a non-trivial escape sequence
WarningText = 'THIS IS AN AUTOMATICALLY GENERATED FILE, DO NOT MODIFY IT DIRECTLY'

# Script body

with open(Path + Filename + '.html', 'r') as html_file:
  raw_html = BeautifulSoup(html_file.read(), 'html.parser')

for element in raw_html:
  if isinstance(element, Comment):
    element.extract()

clean_html = ''
for line in str(raw_html).splitlines():
  clean_html += line.strip()

with open(Path + Filename + '.h', 'w') as header_file:
  header_file.write(f'/*{WarningText}*/\n#ifndef {IncludeGuard}\n#define {IncludeGuard}\n#include <Arduino.h>\nconstexpr char website_content[] PROGMEM = R"###({clean_html})###";\n#endif')

print(f'HTML CONVERTER: Generated C-style char array with a size of {len(clean_html) + 1} bytes.')