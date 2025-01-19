# [![MacOsWorkflow](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/MacOs.yml/badge.svg)](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/MacOs.yml) [![LinuxWorkflow](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/Linux.yml/badge.svg)](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/Linux.yml) [![WindowsWorkflow](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/Windows.yml/badge.svg)](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/Windows.yml)


# Minesweeper
C++ implementation of minesweeper game using [SFML](https://www.sfml-dev.org/) library.

![Microsoft Minesweeper Screenshot 2025 01 19 - 11 31 06 65](https://github.com/user-attachments/assets/7523e6ae-f96b-42d0-8a50-64b5324e6ad8)

## Requirements
* C++17 compiler.
* CMake 3.22.0
* [SFML 2.6.1 requirements](https://www.sfml-dev.org/tutorials/2.6/start-cmake.php#requirements). 

## Build and Install
- Clone the repository to your local machine.
   ```terminal
   git clone https://github.com/alejandrofsevilla/minesweeper.git
   cd minesweeper
   ```
- Build.
   ```terminal
   cmake -S . -B build
   cmake --build build
   ```
- Portable installation.
   ```terminal
   cmake --install build
   ```
