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
 * 
 * dwin_screen.h
 *
 * Screen implementations
 * 
*/
#include "dwin_draw.h"
#include "./src/module/temperature.h"
#include "../../inc/MarlinConfigPre.h" // for HAS_LEVELING

#ifndef HAS_LEVELING  // If bed leveling not defined
#define HAS_LEVELING 0
#endif

#ifndef MACHINE_SIZE
  #define MACHINE_SIZE "220x220x250"
#endif
#ifndef CORP_WEBSITE_C
  #define CORP_WEBSITE_C "www.cxsw3d.com"
#endif
#ifndef CORP_WEBSITE_E
  #define CORP_WEBSITE_E "www.creality.com"
#endif

enum Cursor_MainScreen {
  MainMenuScreen_Cursor_Print,
  MainMenuScreen_Cursor_Prepare,
  MainMenuScreen_Cursor_Control,
  MainMenuScreen_Cursor_Leveling,
  MainMenuScreen_Cursor_Info
};

void Screen_DrawMainMenu(boolean EN);
void Screen_MainMenu_Update(boolean EN, int currentCursorPosition);
void Screen_DrawInfoMenu(boolean EN);

void Screen_Indicators_Draw_Temperature_Hotend(float current, float target);
void Screen_Indicators_Update_Temperature_Hotend_Current(float temp);
void Screen_Indicators_Update_Temperature_Hotend_Target(float temp);
void Screen_Indicators_Draw_Temperature_Bed(float current, float target);
void Screen_Indicators_Update_Temperature_Bed_Current(float temp);
void Screen_Indicators_Update_Temperature_Bed_Target(float temp);
void Screen_Indicators_Draw_Feedrate_Percentage(int16_t feedratePercentage);
void Screen_Indicators_Update_Feedrate_Percentage(float rate);
void Screen_Indicators_Draw_ZOffset(float value);