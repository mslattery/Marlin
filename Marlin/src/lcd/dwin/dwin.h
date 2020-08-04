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

// Color - TODO: See if color definitions exist elsewhere in the Marlin ecosystem
// https://demmel.com/ilcd/help/16BitColorValues.htm
#define DWIN_COLOR_WHITE             0xFFFF
#define DWIN_COLOR_MAGENTA           0xF81F
#define DWIN_COLOR_BACKGROUND_WINDOW 0x31E8  // Popup background color
#define DWIN_COLOR_BACKGROUND_BLUE   0x1125  // Dark blue background color
#define DWIN_COLOR_BACKGROUND_BLACK  0x0841  // black background color
#define DWIN_COLOR_FONT_WINDOW       0xD6BA  // Popup font background color
#define DWIN_COLOR_LINE_COLOR        0x3A6A  // Split line color
#define DWIN_COLOR_RECTANGLE_COLOR   0xEE2F  // blue square cursor color
#define DWIN_COLOR_PERCENT_COLOR     0xFE29  // percentage color
#define DWIN_COLOR_BARFILL_COLOR     0x10E4  // fill color of progress bar
#define DWIN_COLOR_SELECT_COLOR      0x33BB  // selected color

/** FONTS
 * 3-.0：The font size, 0x00-0x09, corresponds to the font size below:
 * 0x00=6*12   0x01=8*16   0x02=10*20  0x03=12*24  0x04=14*28
 * 0x05=16*32  0x06=20*40  0x07=24*48  0x08=28*56  0x09=32*64
 */
#define font6x12  0x00
#define font8x16  0x01
#define font10x20 0x02
#define font12x24 0x03
#define font14x28 0x04
#define font16x32 0x05
#define font20x40 0x06
#define font24x48 0x07
#define font28x56 0x08
#define font32x64 0x09

#define MENU_FONT   font8x16
#define STAT_FONT   font10x20
#define HEADER_FONT font10x20

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

#if HAS_HOTEND
  void Popup_Window_Temperature(const bool toohigh);
#endif

void HMI_Init(void);
void HMI_StartFrame(const bool with_update); // startup screen

void DWIN_Update(void);
void DWIN_CompletedHoming(void);

#endif // end include guard