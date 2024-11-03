![License](https://img.shields.io/github/license/julianxhokaxhiu/FindSteamGamePath) ![Overall Downloads](https://img.shields.io/github/downloads/julianxhokaxhiu/FindSteamGamePath/total?label=Overall%20Downloads) ![Latest Stable Downloads](https://img.shields.io/github/downloads/julianxhokaxhiu/FindSteamGamePath/latest/total?label=Latest%20Stable%20Downloads&sort=semver) ![Latest Canary Downloads](https://img.shields.io/github/downloads/julianxhokaxhiu/FindSteamGamePath/canary/total?label=Latest%20Canary%20Downloads) ![GitHub Actions Workflow Status](https://github.com/julianxhokaxhiu/FindSteamGamePath/actions/workflows/main-0.1.0.yml/badge.svg?branch=master)

# FindSteamGamePath

A simple tool that returns where a Steam game has been installed

## How to use

Download the latest release then run it passing as first argument the Steam AppID.

```bash
$ FindSteamGamePath 39140
C:\Program Files (x86)\Steam\steamapps\common\FINAL FANTASY VII
```

## How to integrate in CMakeLists.txt

Download the latest release, unpack it next to your `CMakeLists.txt` and add this code:

```cmake
execute_process(
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/FindSteamGamePath 39140
  OUTPUT_VARIABLE GAME_PATH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(NOT "${GAME_PATH}" STREQUAL "")
  message("Game path found: ${GAME_PATH}")
endif()
```
