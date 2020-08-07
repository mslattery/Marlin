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
#pragma once

/**
 * dwin_layout.h
 *
 * DWIN Layout
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

#include "dwin_lcd.h"

/* Layout
===============================================================================
Coordinate System
===============================================================================
DWIN display is 272W x 480H in portrait orientation with 0,0 (X,Y) in the
upper left corner and 271,479 (X,Y) in lower right corner (see below).

===============================================================================
Overall Layout
===============================================================================
Generally...

- The layout is a title bar with contrasting background with the page title 
  text presented.

- A dynamic area below can be used for all other aspects such as indicators,
  menu items, etc.

0,0   |-------------------------------------------------| 271,0
      | Title Bar                                       |         (30 rows)
0,29  |-------------------------------------------------| 271,29
0,30  |                                                 | 271,30
      |                                                 |
      | Dynamic Area                                    |         (329 rows)
      |                                                 |
      |                                                 |
0,359 |-------------------------------------------------| 271,359
0,360 |                                                 | 271,360
      | Static Indicators                               |         (120 rows)
      |                                                 |
0,479 |-------------------------------------------------| 271,479
*/

// Title bar
#define DWIN_LAYOUT_TITLE_BAR_HEIGHT 30 // Y values of 0 - 29
#define DWIN_LAYOUT_TITLE_BAR_TOPLEFT_X DWIN_LCD_COORD_LEFTMOST_X
#define DWIN_LAYOUT_TITLE_BAR_TOPLEFT_Y DWIN_LCD_COORD_TOPLEFT_Y
#define DWIN_LAYOUT_TITLE_BAR_BOTTOMRIGHT_X DWIN_LCD_COORD_RIGHTMOST_X
#define DWIN_LAYOUT_TITLE_BAR_BOTTOMRIGHT_Y (DWIN_LCD_COORD_TOPRIGHT_Y + DWIN_LAYOUT_TITLE_BAR_HEIGHT - 1)

// Dynamic Area
#define DWIN_LAYOUT_DYNAMIC_AREA_HEIGHT 329 // Y values of 30 - 359
#define DWIN_LAYOUT_DYNAMIC_AREA_TOPLEFT_X DWIN_LCD_COORD_LEFTMOST_X
#define DWIN_LAYOUT_DYNAMIC_AREA_TOPLEFT_Y (DWIN_LAYOUT_TITLE_BAR_BOTTOMRIGHT_Y + 1)
#define DWIN_LAYOUT_DYNAMIC_AREA_BOTTOMRIGHT_X DWIN_LCD_COORD_RIGHTMOST_X
#define DWIN_LAYOUT_DYNAMIC_AREA_BOTTOMRIGHT_Y (DWIN_LAYOUT_DYNAMIC_AREA_TOPLEFT_Y + DWIN_LAYOUT_DYNAMIC_AREA_HEIGHT - 1)

// Indicator Area
#define DWIN_LAYOUT_INDICATOR_HEIGHT 120  // Y values of 360 - 479
#define DWIN_LAYOUT_INDICATOR_AREA_TOPLEFT_X DWIN_LCD_COORD_LEFTMOST_X
#define DWIN_LAYOUT_INDICATOR_AREA_TOPLEFT_Y (DWIN_HEIGHT - DWIN_LAYOUT_INDICATOR_HEIGHT - 1)
#define DWIN_LAYOUT_INDICATOR_AREA_BOTTOMRIGHT_X DWIN_LCD_COORD_RIGHTMOST_X
#define DWIN_LAYOUT_INDICATOR_AREA_BOTTOMRIGHT_Y DWIN_LCD_COORD_BOTTOMRIGHT_Y

/* From Thinkyhead - 

We'll use the selected font size to determine a row height, and then multiply by the row height to get the display line, 
and we'll draw a box on the selected line, and print the text of the menu item, and all that. The DWIN has the ability 
to cut one part of the screen and draw it in another position, and this is used by dwin.cpp to handle the scrolling.
Scrolling handled by the screen is a really good optimization that we haven't got in MarlinUI yet, so it makes sense 
to add that when working on the screen drawing. For that we will probably need to do some tweaks to support drawing each 
menu item only when: (1) doing a full screen refresh, and (2) when the menu item becomes the new top or bottom line.

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
0,359 |-------------------------------------------------| 271,359
0,360 |                                                 | 271,360
      |    HH 123 / 245            BB 10 / 70           |
      |                                                 |
      |    FF 100%                 ZZ 0.00              |         Indicators
      |                                                 |
0,479 |-------------------------------------------------| 271,479
*/

// https://demmel.com/ilcd/help/16BitColorValues.htm
#define DWIN_COLOR_WHITE              0xFFFF
#define DWIN_COLOR_MAGENTA            0xF81F
#define DWIN_COLOR_BLUE               0x001F
#define DWIN_COLOR_GREEN              0x07E0

// Font Sizes
#define font6x12                      0x00
#define font8x16                      0x01
#define font10x20                     0x02
#define font12x24                     0x03
#define font14x28                     0x04
#define font16x32                     0x05
#define font20x40                     0x06
#define font24x48                     0x07
#define font28x56                     0x08
#define font32x64                     0x09

// Theme Fonts
#define THEME_FONT_MENU               font8x16
#define THEME_FONT_STAT               font10x20
#define THEME_FONT_HEADER             font10x20

// Theme Colors
#define THEME_COLOR_TITLE             DWIN_COLOR_WHITE
#define THEME_COLOR_BACKGROUND_WINDOW 0x31E8  // Popup background color
#define THEME_COLOR_BACKGROUND_BLUE   0x1125  // Dark blue background color
#define THEME_COLOR_BACKGROUND_BLACK  0x0841  // black background color
#define THEME_COLOR_FONT_WINDOW       0xD6BA  // Popup font background color
#define THEME_COLOR_LINE              0x3A6A  // Split line color
#define THEME_COLOR_CURSOR            0xEE2F  // blue square cursor color
#define THEME_COLOR_PERCENT           0xFE29  // percentage color
#define THEME_COLOR_BARFILL           0x10E4  // fill color of progress bar
#define THEME_COLOR_SELECT            0x33BB  // selected color

// Main Screen Items Icons
#define THEME_LOGO_X                  71
#define THEME_LOGO_Y                  52
#define THEME_ICON_PRINT_X            17
#define THEME_ICON_PRINT_Y            130
#define THEME_ICON_PREPARE_X          145
#define THEME_ICON_PREPARE_Y          130
#define THEME_ICON_CONTROL_X          17 
#define THEME_ICON_CONTROL_Y          246
#define THEME_ICON_INFO_X             145
#define THEME_ICON_INFO_Y             246
#define THEME_ICON_LEVELING_X         145 // NOTE Same Location as INFO, so only one can be shown at a time
#define THEME_ICON_LEVELING_Y         246

// Theme Text Locations
#define THEME_TITLE_X                 14
#define THEME_TITLE_Y                 4

// INFO Screen Layout Items
#define THEME_INFO_FONT               font8x16
#define THEME_INFO_TEXT_COLOR         DWIN_COLOR_WHITE
#define THEME_INFO_LINE1_X            2
#define THEME_INFO_LINE1_Y            122
#define THEME_INFO_LINE2_X            2
#define THEME_INFO_LINE2_Y            195
#define THEME_INFO_LINE3_X            2
#define THEME_INFO_LINE3_Y            268
#define THEME_INFO_ICON_X             26

// Indicators Layout Items
#define THEME_ICON_HOTEND_X           13
#define THEME_ICON_HOTEND_Y           381
#define THEME_ICON_BED_X              158
#define THEME_ICON_BED_Y              381
#define THEME_ICON_FEEDRATE_X         13
#define THEME_ICON_FEEDRATE_Y         429
#define THEME_ICON_ZOFFSET_X          158
#define THEME_ICON_ZOFFSET_Y          428

#define THEME_INDICATOR_HOTEND_X      33
#define THEME_INDICATOR_HOTEND_Y      382
#define THEME_INDICATOR_BED_X         178
#define THEME_INDICATOR_BED_Y         382
#define THEME_INDICATOR_FEEDRATE_Y    429
#define THEME_INDICATOR_ZOFFSET_X     178
#define THEME_INDICATOR_ZOFFSET_Y     429

// Character Widths for Calculations
#define MBASE(L) (49 + (L)*MLINE)
constexpr uint16_t MLINE = 53,                          // Menu line height
                   LBLX = 60,                           // Menu item label X
                   MENU_CHR_W = 8,                      // Menu Char Width
                   STAT_CHR_W = 10;      