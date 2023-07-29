# Colorize
> A library for colorizing texts in terminal.

<h1 align="center">Demo</h1>
<p align="center" width=100%">
  <img width="48%" src="assets/colorize-demo.gif" alt="Demo GIF">
  &nbsp; &nbsp;
  <img width="45%" src="assets/gradient-demo.png" alt="Demo Gradiant">
</p>

<br>

## Table of Contents

- [Supported Platforms](#supported-on)
- [Usage](#using-at-a-glance)
- [Installation](#installation)
- [Documentation](/docs/documentation.md)

<br>

## Supported on

> - [x] Linux
> - [x] MacOS
> - [x] Windows  <sup>Terminal-Emulator Needed! <sup> Win-Terminals are crappy so, kinda working...</sup></sup>

<br>

# Using at a glance

### 24 BIT <sup>True Color</sup>
```C++
// 24 BIT provides a color set of 16,777,216 colors.
std::cout << Colorize::rich24<0xeb106f, 0x263640>("Hello World", {Beautifier::emphasis::BOLD}) << std::endl;
```
##### which gives
<img width="38%" src="assets/example-001.png" alt="Example">

***

### 8 BIT <sup>0-255</sup>
```C++
// 8 BIT provides a color set of 0-255 colors.
std::cout << Colorize::rich<199, 253>("Hello World", {Beautifier::emphasis::BOLD}) << std::endl;
```
##### which gives
<img width="38%" src="assets/example-002.png" alt="Example">

<br>

# Installation
Since it's **Header-Only**,
<br>
Just copy and paste [**colorize.h**](/src/colorize.hpp) next to your **Project/Path** (`e.g. on linux /usr/include/`)
