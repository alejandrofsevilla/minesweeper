# [![MacOsWorkflow](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/MacOs.yml/badge.svg)](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/MacOs.yml) [![LinuxWorkflow](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/Linux.yml/badge.svg)](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/Linux.yml) [![WindowsWorkflow](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/Windows.yml/badge.svg)](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/Windows.yml)


# Minesweeper
C++ implementation of minesweeper game using [SFML](https://www.sfml-dev.org/) library.

![Microsoft Minesweeper Screenshot 2025 02 07 - 09 46 57 34](https://github.com/user-attachments/assets/b53fc220-f18c-41a3-9752-6296d3ed26f7)

## Requirements
* C++17 compiler.
* CMake 3.22.0
* [SFML 2.6.1 requirements](https://www.sfml-dev.org/tutorials/2.6/start-cmake.php#requirements). 

## Build and Install
- Clone repository.
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
