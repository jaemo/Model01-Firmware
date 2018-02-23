#!/bin/bash

make ARDUINO_BUILDER_PATH="/usr/bin/arduino-builder" \
	ARDUINO_PATH="/usr/share/arduino" \
	ARDUINO_LOCAL_LIB_PATH="../arduino-local" \
	AVR_GCC_PREFIX="/usr" \
	ARDUINO_TOOLS_PATH="/usr/share/arduino/tools" \
	AVR_SIZE_PATH="avr-size" \
	flash
