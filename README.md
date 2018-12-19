# NOTE: THIS PROJECT HAS BEEN PUT ON ICE FOR AN INDEFINITE AMOUNT OF TIME

# About
Ambii is an ambient lightning system for windows. The goal of the project is to create a fully functional dynamic lighting system using as little hardware as possible. Ambii is written in C++ using the WIN32 API.

# Features
- Multi-monitor support
- Software-based configuration
- View screen capture and output values

# Architecture
Ambii communicates directly with windows using the WIN32 API from which it gets the screen data. This data is then used to compute an average RGB-value over a square surrounding every LED-position. All the output values are written to a serial port in a serial manner, i.e. the first and last output value corresponds to the first respectively last LED, so Ambii should work with any microcontroller as long as it handles serial data (in the Arduino folder there is a small script that should work for most Arduino boards and any LED-strips supported by the FastLED library).

# Demo
TODO: Links to demo here
  
# Credits
http://fastled.io/ - for the Arduino LED library used in the Arduino script
