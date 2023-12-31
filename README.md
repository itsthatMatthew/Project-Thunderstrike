# Project-Thunderstrike

[![PlatformIO CI](https://github.com/itsthatMatthew/Project-Thunderstrike/actions/workflows/platformioci.yaml/badge.svg)](https://github.com/itsthatMatthew/Project-Thunderstrike/actions/workflows/platformioci.yaml)
[![CodeQL](https://github.com/itsthatMatthew/Project-Thunderstrike/actions/workflows/codeql.yaml/badge.svg)](https://github.com/itsthatMatthew/Project-Thunderstrike/actions/workflows/codeql.yaml)

## Overview

Demo bomb-defusal game project mostly for airsoft games and other sorts of activity, based on an ESP32 micro-controller. Inspired by Keep Talking And Nobody Explodes.

The goal is a modular software and hardware design, where new games and tasks can be easily added to the "main board".

## Getting involved

- If you want to contribute to the project, check out our [Project Contribution Guidelines](.github/CONTRIBUTING.md)!
- If you want to report bugs, tackle existing ones, or request features, check out the project's [Issues](https://github.com/itsthatMatthew/Project-Thunderstrike/issues) page!
- If you just want to chat with others, discuss and brainstorm ideas, check out the project's [Discussions](https://github.com/itsthatMatthew/Project-Thunderstrike/discussions) page!

## Software

The project is written in C++ based on the Arduino framework.

Any project related code (such as classes, util functions and definitions) *should be* placed within the `PTS` namespace (standing for **P**roject **T**hunder**S**trike). The current implementation aims at a header only approach, which might drastically change in the future.

A common wish is a modular software-design in the sense that XML or JSON files can serve as setup inputs or active changes, providing an easy interface and ease of modification even on the field.

## Hardware

As previously mentioned, the project's main target platform is the ESP32, as it's available to us and generally provides nice features. The "building" is done on a breadboard for now, as it's more of a curious exploration of our possibilities.

In the future we intend on creating more complex contraptions with some 3D printing and DIY-ing - such as a hard case bomb launch terminal; and a plastic explosive IED with a ticking timer and an overwhelming number of wires out of which only the correct ones can be disconnected from - but that kind of progress is far ahead.
