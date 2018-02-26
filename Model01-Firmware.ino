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
#include <Kaleidoscope-OneShot.h>
#include <Kaleidoscope-Escape-OneShot.h>
#include <Kaleidoscope-LEDEffect-FunctionalColor.h>
#include <Kaleidoscope-LED-ActiveModColor.h>
#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-NumPad.h"
#include "LED-Off.h"
#include "Kaleidoscope-LEDEffect-BootGreeting.h"
#include "Kaleidoscope-LEDEffect-SolidColor.h"
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
		 OSM(LeftControl),  Key_Backspace, Key_LeftAlt, Key_LeftShift,
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
		(___,      Key_F1,           Key_F2,      Key_F3,     Key_F4,        Key_F5,           XXX,
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


static kaleidoscope::LEDSolidColor solidIndigo(0, 0, 170);

kaleidoscope::LEDFunctionalColor FunColor;
kaleidoscope::LEDFunctionalColor FunColorMedium;
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
			&BootGreetingEffect,
			&TapDance,
			&OneShot,
      &EscapeOneShot,
			&LEDControl,
			&LEDOff,
      &solidIndigo,
			&FunColor,
			&FunColorMedium,
			&NumPad,
      &ActiveModColorEffect,
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

	cRGB antiquewhite = CRGB(250, 235, 215);
	cRGB blue = CRGB(0, 0, 255);
	cRGB cyan = CRGB(0, 255, 255);
	cRGB green = CRGB(0, 128, 0);
	cRGB lightskyblue = CRGB(135, 206, 250);
	cRGB lime = CRGB(0, 255, 0);
	cRGB mintcream = CRGB(245, 255, 250);
	cRGB orange = CRGB(255, 165, 0);
	cRGB orangered = CRGB(255, 100, 0);
	cRGB palegreen = CRGB(152, 251, 152);
	cRGB pink = CRGB(255, 192, 203);
	cRGB red = CRGB(255, 0, 0);
	cRGB skyblue = CRGB(135, 206, 235);
	cRGB slateblue = CRGB(106, 90, 205);
	cRGB purple = CRGB(150, 50, 255);
	cRGB violet = CRGB(255, 120, 255);
	cRGB white = CRGB(255, 255, 255);
	cRGB yellow = CRGB(255, 255, 0);
	cRGB yellowgreen = CRGB(154, 205, 50);


	// If your FUNCTION layer is not the default, you must set it here
	FunColor.functionLayer = FUNCTION;

	// Here we can set custom colors for your FunctionalColor instance.
	// You can optionally specify a brightness value, 0-255 to dim your lights.

	// Set this first to provide a "default" color for all keys, then override with the other settings.
	FunColor.all(CRGB(250, 235, 215));

	// Set this second to change all modifiers (non-alphabet/numeric/punctuation keys)
	FunColor.allModifiers(CRGB(250, 235, 215));

	// Set this before individual mouse settings to change all mouse-related keys
	FunColor.allMouse(CRGB(0, 200, 200));

	//Set individual groups of colors. You may delete any lines you don't need.
	FunColor.escape(orangered, 170);
	FunColor.numbers(orange, 120);
	FunColor.letters_top_row(yellow, 110);
	FunColor.letters_home_row(yellowgreen, 130);
	FunColor.letters_bottom_row(green, 170);
	FunColor.punctuation(cyan, 100);
	FunColor.brackets(blue, 200);
	FunColor.backslash(red, 170);
	FunColor.pipe(cyan, 170);
	FunColor.tab(lime, 200);
	FunColor.backspace(red, 170);
	FunColor.del(red, 170);
	FunColor.enter(orangered, 210);
	FunColor.arrows(green, 170);
	FunColor.nav(cyan, 170);
	FunColor.insert(green, 170);
	FunColor.shift(purple, 190);
	FunColor.ctrl(blue, 170);
	FunColor.alt(violet, 200);
	FunColor.cmd(violet, 200);
	FunColor.fkeys(red, 170);
	FunColor.fn(CRGB(250, 235, 215));
	FunColor.media(CRGB(250, 235, 215));
	FunColor.led(blue, 0);
	FunColor.mousemove(cyan, 170);
	FunColor.mousebuttons(orange, 170);
	FunColor.mousewarp(cyan, 100);
	FunColor.mousescroll(lightskyblue, 100);

	//Copy new settings to the dimmed versions
	FunColorMedium = FunColor;

	// You could make adjustments to your other versions' groups here, if desired.

	// Adjust the brightness of dimmed versions here from 0-255
	FunColorMedium.brightness(210);

  ActiveModColorEffect.highlight_color = CRGB(0x43, 0xab, 0xab);
  ActiveModColorEffect.sticky_color = CRGB(0xab, 0xab, 0x03);
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
