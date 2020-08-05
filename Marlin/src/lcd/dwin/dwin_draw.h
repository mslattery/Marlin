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

#include "dwin_lcd.h" // For hardware draw calls
#include "dwin_layout.h" // For Coord/Theme
#include "dwin_icons.h" // For icons
#include "../../MarlinCore.h" // for __FlashStringHelper

/**
 * 
 * dwin_draw.h
 *
 * Abstracted draw implementations
 * 
*/
void Draw_MainWindowBackground(void);
void Draw_TitleBar_Background(void);
void Draw_DynamicArea_Background(void);
void Draw_TitleText(const __FlashStringHelper * titleText);
void Draw_Logo();
void Draw_ICON_Print(boolean EN);