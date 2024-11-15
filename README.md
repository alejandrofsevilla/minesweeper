# [![MacOsWorkflow](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/MacOs.yml/badge.svg)](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/MacOs.yml) [![LinuxWorkflow](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/Linux.yml/badge.svg)](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/Linux.yml) [![WindowsWorkflow](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/Windows.yml/badge.svg)](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/Windows.yml)


# Minesweeper
C++ implementation of minesweeper game using [SFML](https://www.sfml-dev.org/) library.

![screenshot](https://github.com/user-attachments/assets/0009216c-f447-4e70-9a16-b7421378a51b)

## Requirements
* C++17 compiler.
* CMake
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
