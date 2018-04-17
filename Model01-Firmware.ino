// -*- mode: c++ -*-
// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION "locally built"
#endif

/**
 * These #include directives pull in the Kaleidoscope firmware core,
 * as well as the Kaleidoscope plugins we use in the Model 01's firmware
 */

// The Kaleidoscope core
#include "Kaleidoscope.h"
// Now all the tweaks.
#include <Kaleidoscope-TapDance.h>
#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-NumPad.h"
#include "LED-Off.h"
#include <Kaleidoscope-LED-Wavepool.h>
#include "Kaleidoscope-LEDEffect-Rainbow.h"
#include "Kaleidoscope-HostPowerManagement.h"

// tapdance config
enum {
	TD_BRACES,
	TD_UPDOWN,
	TD_HOMEEND,
};

void tapDanceAction(uint8_t tap_dance_index, byte row, byte col, uint8_t tapCount, kaleidoscope::TapDance::ActionType tapDanceAction) {
	switch (tap_dance_index) {

		case TD_BRACES:
			return tapDanceActionKeys(tapCount, tapDanceAction,
					Key_LeftBracket,
					Key_RightBracket);

		case TD_UPDOWN:
			return tapDanceActionKeys(tapCount, tapDanceAction, Key_PageDown, Key_PageUp);

		case TD_HOMEEND:
			return tapDanceActionKeys(tapCount, tapDanceAction, Key_Home, Key_End);
	}
}

enum { QWERTY, NUMPAD, FUNCTION};

/* This comment temporarily turns off astyle's indent enforcement
 *   so we can make the keymaps actually resemble the physical key layout better
 */
// *INDENT-OFF*

const Key keymaps[][ROWS][COLS] PROGMEM = {

	[QWERTY] = KEYMAP_STACKED
		(Key_Escape,          Key_1, Key_2, Key_3, Key_4, Key_5, Key_LEDEffectNext,
		 Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
		 TD(TD_HOMEEND),   Key_A, Key_S, Key_D, Key_F, Key_G,
		 TD(TD_UPDOWN), Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,
		 Key_LeftControl, Key_LeftAlt, Key_Backspace, Key_LeftShift,
		 ShiftToLayer(FUNCTION),

		 TD(TD_BRACES),  Key_6, Key_7, Key_8, Key_9, Key_0 ,LockLayer(NUMPAD),
		 Key_Tab,     Key_Y, Key_U, Key_I,     Key_O,         Key_P,         Key_Equals,
		 Key_H, Key_J, Key_K,     Key_L,         Key_Semicolon, Key_Quote,
		 Key_Enter,  Key_N, Key_M, Key_Comma, Key_Period,    Key_Slash,     Key_Minus,
		 Key_RightShift, Key_Spacebar, Key_RightAlt, Key_RightControl,
		 ShiftToLayer(FUNCTION)),


	[NUMPAD] =  KEYMAP_STACKED
		(___, ___, ___, ___, ___, ___, ___,
		 ___, ___, ___, ___, ___, ___, ___,
		 ___, ___, ___, ___, ___, ___,
		 ___, ___, ___, ___, ___, ___, ___,
		 ___, ___, ___, ___,
		 ___,

		 ___,  ___, Key_Keypad7, Key_Keypad8,   Key_Keypad9,        Key_KeypadSubtract, ___,
		 ___,                    ___, Key_Keypad4, Key_Keypad5,   Key_Keypad6,        Key_KeypadAdd,      ___,
		 ___, Key_Keypad1, Key_Keypad2,   Key_Keypad3,        Key_Equals,         Key_Quote,
		 ___,                    ___, Key_Keypad0, Key_KeypadDot, Key_KeypadMultiply, Key_KeypadDivide,   Key_Enter,
		 ___, ___, ___, ___,
		 ___),

	[FUNCTION] =  KEYMAP_STACKED
		(
     M(0),      Key_F1,           Key_F2,      Key_F3,     Key_F4,        Key_F5,           XXX,
		 Key_Tab,  ___,              Key_mouseUp, ___,        Key_mouseBtnR, Key_mouseWarpEnd, Key_mouseWarpNE,
		 Key_Home, Key_mouseL,       Key_mouseDn, Key_mouseR, Key_mouseBtnL, Key_mouseWarpNW,
		 Key_End,  Key_PrintScreen,  Key_Insert,  ___,        Key_mouseBtnM, Key_mouseWarpSW,  Key_mouseWarpSE,
		 ___,  ___, ___, Key_Delete,
		 ___,

		 Consumer_ScanPreviousTrack, Key_F6,                 Key_F7,                   Key_F8,                   Key_F9,          Key_F10,          Key_F11,
		 Consumer_PlaySlashPause,    Consumer_ScanNextTrack, Key_LeftCurlyBracket,     Key_RightCurlyBracket,    Key_LeftBracket, Key_RightBracket, Key_F12,
		 Key_LeftArrow,          Key_DownArrow,            Key_UpArrow,              Key_RightArrow,  ___,              ___,
		 Key_PcApplication,          Consumer_Mute,          Consumer_VolumeDecrement, Consumer_VolumeIncrement, ___,             Key_Backslash,    Key_Pipe,
		 ___, Key_Enter, ___, ___,
		 ___)

};

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
    if (macroIndex == 0 && key_toggled_off (keyState)) {
      //
    }

    return MACRO_NONE;
}
/* Re-enable astyle's indent enforcement */
// *INDENT-ON*



/** toggleLedsOnSuspendResume toggles the LEDs off when the host goes to sleep,
 * and turns them back on when it wakes up.
 */
void toggleLedsOnSuspendResume(kaleidoscope::HostPowerManagement::Event event) {
	switch (event) {
		case kaleidoscope::HostPowerManagement::Suspend:
			LEDControl.paused = true;
			LEDControl.set_all_leds_to({0, 0, 0});
			LEDControl.syncLeds();
			break;
		case kaleidoscope::HostPowerManagement::Resume:
			LEDControl.paused = false;
			LEDControl.refreshAll();
			break;
		case kaleidoscope::HostPowerManagement::Sleep:
			break;
	}
}

/** hostPowerManagementEventHandler dispatches power management events (suspend,
 * resume, and sleep) to other functions that perform action based on these
 * events.
 */
void hostPowerManagementEventHandler(kaleidoscope::HostPowerManagement::Event event) {
	toggleLedsOnSuspendResume(event);
}


/** The 'setup' function is one of the two standard Arduino sketch functions.
 * It's called when your keyboard first powers up. This is where you set up
 * Kaleidoscope and any plugins.
 */

void setup() {
	// First, call Kaleidoscope's internal setup function
	Kaleidoscope.setup(

	);
	// The order can be important. For example, LED effects are
	// added in the order they're listed here.
	Kaleidoscope.use(
			&LEDOff,
			&TapDance,
			&LEDControl,
      &LEDRainbowWaveEffect,
      &WavepoolEffect,
			&NumPad,
			&Macros,
			&MouseKeys,
			&HostPowerManagement
	);

	// While we hope to improve this in the future, the NumPad plugin
	// needs to be explicitly told which keymap layer is your numpad layer
	NumPad.numPadLayer = NUMPAD;


	// We want the keyboard to be able to wake the host up from suspend.
	HostPowerManagement.enableWakeup();

	// We want to make sure that the firmware starts with LED effects off
	// This avoids over-taxing devices that don't have a lot of power to share
	// with USB devices
	LEDOff.activate();
  LEDRainbowWaveEffect.brightness(110);
  WavepoolEffect.idle_timeout = 5000;  // 15 seconds
}

/** loop is the second of the standard Arduino sketch functions.
 * As you might expect, it runs in a loop, never exiting.
 *
 * For Kaleidoscope-based keyboard firmware, you usually just want to
 * call Kaleidoscope.loop(); and not do anything custom here.
 */

void loop() {
	Kaleidoscope.loop();
}
