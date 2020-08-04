/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#ifndef DWIN_H // Guard against circular references
#define DWIN_H

#pragma once

/**
 * dwin.cpp
 *
 * LCD display implementations for DWIN by Creality3D.
 * This is the display on the Creality3d Ender 3 V2.
 * http://www.ampdisplay.com/documents/pdf/DWIN%20DGUS%20DEV%20GUIDE_V40_2014.pdf
 * 
 *               |\__-----__/|
            _____/::::::  :::\_____
           '__--(:::::::::..::)--__`
           /  _- \/: :::::::\/ -_  \
             /   /::.   .::::\   \
                |:::::::::::::|
               _|/:::::____::\|_
             /::/:::::/:_::\::\:.\
            |::|  ..:(_/ \::|::|::|
            |::|:::::. ::|: |::|.:|
             \:|::  :::_/::/: :|:/
           ((___\____\____/___/___)) - Slats
 */

#include "../../MarlinCore.h" // for millis_t
#include "rotary_encoder.h"
#include "dwin_icons.h"
#include "dwin_colors.h"
#include "dwin_fonts.h"

/* Screen Layout

===============================================================================
Coordinate System
===============================================================================
DWIN display is 272W x 480H in portrait orientation, with 0,0 (X,Y) in 
upper left corner and 272,480 (X,Y) in lower right corner(see below).

===============================================================================
Overall Layout
===============================================================================
Generally...

- The layout is a title bar with contrasting background with the page title 
  text presented.

- A dynamic area below can be used for all other aspects such as indicators,
  menu items, etc.

0,0   |-------------------------------------------------| 271,0
      | Title Bar                                       |
0,30  |-------------------------------------------------| 271,30
0,31  |                                                 | 271,31
      |                                                 |
      |                                                 |
      |      Dynamic Area                               |
      |                                                 |
      |                                                 |
0,479 |-------------------------------------------------| 271,479

===============================================================================
Main Screen Layout
===============================================================================
- Title is just the text
    Main
- Logo is Creality logo, in the future may be able to use a custom logo
- Print, Prepare, Control, Info are selectable menu items with icons
- Indicators, Letters are placeholders for icons
  *HH - Hot End temperature current and set to, in the example this would be
        currently 123 degrees, set to 245.
  *BB - Bed Temperature current and set to, in the example this would be 
        currently 10 degrees, set to 70.
  *FF - Feedrate percentage
  *ZZ - Z offset

0,0   |-------------------------------------------------| 271,0
      | Main                                            |         Title Bar
0,30  |-------------------------------------------------| 271,30
0,31  |                                                 | 271,31
      |                  |  / \/__/ \                   |         Logo Area
      |                  |__\_/\_|\_/                   |
      |                                                 |
      |                                                 | x,y
      |    |---------------|       |---------------|    | x,y
      |    |               |       |               |    |
      |    |               |       |               |    |
      |    |               |       |               |    |
      |    |     Print     |       |    Prepare    |    |
      |    |               |       |               |    |
      |    |---------------|       |---------------|    |
      |                                                 |         Menu Items
      |    |---------------|       |---------------|    |
      |    |               |       |               |    |
      |    |               |       |               |    |
      |    |               |       |               |    |
      |    |    Control    |       |      Info     |    |
      |    |               |       |               |    |
      |    |---------------|       |---------------|    |
      |                                                 | x,y
      |                                                 | x,y
      |    HH 123 / 245            BB 10 / 70           |
      |                                                 |
      |    FF 100%                 ZZ 0.00              |         Indicators
      |                                                 |
0,479 |-------------------------------------------------| 271,479


From Thinkyhead - 

We'll use the selected font size to determine a row height, and then multiply by the row height to get the display line, 
and we'll draw a box on the selected line, and print the text of the menu item, and all that. The DWIN has the ability 
to cut one part of the screen and draw it in another position, and this is used by dwin.cpp to handle the scrolling.
Scrolling handled by the screen is a really good optimization that we haven't got in MarlinUI yet, so it makes sense 
to add that when working on the screen drawing. For that we will probably need to do some tweaks to support drawing each 
menu item only when: (1) doing a full screen refresh, and (2) when the menu item becomes the new top or bottom line.
*/

// Size attributes for this implementation
#define LAYOUT_TITLE_BAR_HEIGHT 30 // Height in y pixels of title bar

extern millis_t dwin_heat_time;

typedef struct {
  bool language_flag;  // 0: EN, 1: CN
  bool pause_flag:1;
  bool print_finish:1;
  bool confirm_flag:1;
  bool select_flag:1;
  bool home_flag:1;
  bool heat_flag:1;  // 0: heating done  1: during heating
  #if HAS_HOTEND
    bool ETempTooLow_flag:1;
  #endif
  #if HAS_LEVELING
    bool leveling_offset_flag:1;
  #endif
  #if HAS_FAN
    char feedspeed_flag;
  #endif
  char acc_flag;
  char corner_flag;
  char step_flag;
} HMI_Flag;
extern HMI_Flag    HMI_flag;

enum ScreenIDs { // Screen IDs for moving around menu
  MainMenuScreen,
  SelectFile,
  Prepare,
  Control,
  Leveling,
  PrintProcess,
  AxisMove,
  TemperatureID,
  Motion,
  InfoScreen,
  Tune,
  #if HAS_HOTEND
    PLAPreheat,
    ABSPreheat,
  #endif
  MaxSpeed,
  MaxSpeed_value,
  MaxAcceleration,
  MaxAcceleration_value,
  MaxCorner,
  MaxCorner_value,
  Step,
  Step_value,

  // Last Process ID
  Last_Prepare,

  // Back Process ID
  Back_Main,
  Back_Print,

  // Date variable ID
  Move_X,
  Move_Y,
  Move_Z,
  Extruder,
  Homeoffset,
  #if HAS_HOTEND
    ETemp,
  #endif
  #if HAS_HEATED_BED
    BedTemp,
  #endif
  #if HAS_FAN
    FanSpeed,
  #endif
  PrintSpeed,

  // Window ID
  Print_window,
  Popup_Window
};

#if HAS_HOTEND
  void Popup_Window_Temperature(const bool toohigh);
#endif

void HMI_Init(void);
void HMI_StartFrame(const bool with_update); // startup screen

void DWIN_Update(void);
void DWIN_CompletedHoming(void);

#endif // end include guard