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
#ifndef DWIN_COLORS_H // Guard against circular references
#define DWIN_COLORS_H

#pragma once

/**
 * dwin_colors.h
 *
 * LCD display colors - 16 bit representation
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
// Color - TODO: See if color definitions exist elsewhere in the Marlin ecosystem
// https://demmel.com/ilcd/help/16BitColorValues.htm
#define DWIN_COLOR_WHITE             0xFFFF
#define DWIN_COLOR_MAGENTA           0xF81F
#define DWIN_COLOR_BLUE              0x001F
#define DWIN_COLOR_GREEN             0x07E0
#define DWIN_COLOR_BACKGROUND_WINDOW 0x31E8  // Popup background color
#define DWIN_COLOR_BACKGROUND_BLUE   0x1125  // Dark blue background color
#define DWIN_COLOR_BACKGROUND_BLACK  0x0841  // black background color
#define DWIN_COLOR_FONT_WINDOW       0xD6BA  // Popup font background color
#define DWIN_COLOR_LINE              0x3A6A  // Split line color
#define DWIN_COLOR_CURSOR            0xEE2F  // blue square cursor color
#define DWIN_COLOR_PERCENT           0xFE29  // percentage color
#define DWIN_COLOR_BARFILL           0x10E4  // fill color of progress bar
#define DWIN_COLOR_SELECT            0x33BB  // selected color

#endif