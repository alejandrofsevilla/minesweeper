# ![MacOsWorkflow](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/MacOs.yml/badge.svg) ![LinuxWorkflow](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/Linux.yml/badge.svg) ![WindowsWorkflow](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/Windows.yml/badge.svg)

# Minesweeper
Implementation of Minesweeper game using [SFML](https://www.sfml-dev.org/).

![screenshot](https://github.com/user-attachments/assets/36a990b4-d763-4360-a212-9994964b4267)

## Requirements
* C++17 compiler.
* CMake

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
