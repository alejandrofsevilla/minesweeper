# [![MacOsWorkflow](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/MacOs.yml/badge.svg)](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/MacOs.yml) [![LinuxWorkflow](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/Linux.yml/badge.svg)](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/Linux.yml) [![WindowsWorkflow](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/Windows.yml/badge.svg)](https://github.com/alejandrofsevilla/minesweeper/actions/workflows/Windows.yml)

# Minesweeper
C++ implementation of minesweeper game using [SFML](https://www.sfml-dev.org/) library.
<p align="left">
  <img src="https://github.com/user-attachments/assets/926196a8-4080-498a-9f5c-2f838103df9a"  width="450" />
</p>

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
