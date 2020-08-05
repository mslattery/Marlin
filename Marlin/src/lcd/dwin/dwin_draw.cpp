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

#include "dwin_draw.h"

/**
 * 
 * dwin_draw.cpp
 *
 * Abstracted draw implementations
 * 
*/

void Draw_MainWindowBackground(void){
  Draw_TitleBar_Background();
  Draw_DynamicArea_Background();
}

void Draw_TitleText(const char * titleText) { 
  DWIN_Draw_String(false, false, THEME_FONT_HEADER, THEME_COLOR_TITLE, THEME_COLOR_BACKGROUND_BLUE, THEME_TITLE_X, THEME_TITLE_Y, (char*)titleText);
}

void Draw_TitleText(const __FlashStringHelper * titleText) {
  DWIN_Draw_String(false, false, THEME_FONT_HEADER, THEME_COLOR_TITLE, THEME_COLOR_BACKGROUND_BLUE, THEME_TITLE_X, THEME_TITLE_Y, (char*)titleText);
}

void Draw_TitleBar_Background(void) {
  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, THEME_COLOR_BACKGROUND_BLUE, DWIN_LAYOUT_TITLE_BAR_TOPLEFT_X, DWIN_LAYOUT_TITLE_BAR_TOPLEFT_Y, DWIN_LAYOUT_TITLE_BAR_BOTTOMRIGHT_X, DWIN_LAYOUT_TITLE_BAR_BOTTOMRIGHT_Y);
}

void Draw_DynamicArea_Background(void) {
  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, THEME_COLOR_BACKGROUND_BLACK, DWIN_LAYOUT_DYNAMIC_AREA_TOPLEFT_X, DWIN_LAYOUT_DYNAMIC_AREA_TOPLEFT_Y, DWIN_LAYOUT_DYNAMIC_AREA_BOTTOMRIGHT_X, DWIN_LAYOUT_DYNAMIC_AREA_BOTTOMRIGHT_Y);
}

void Draw_Logo() {
  DWIN_ICON_Show(ICON, ICON_LOGO, THEME_LOGO_X, THEME_LOGO_Y);  
}

void Draw_ICON_Button_Print(boolean EN, boolean selected) {
  DWIN_ICON_Show(ICON, TERN(selected, ICON_Print_0, ICON_Print_1), THEME_ICON_PRINT_X, THEME_ICON_PRINT_Y);  // ICON_BLAH_1 is selected icon look
}

void Draw_ICON_Button_Prepare(boolean EN, boolean selected) {
  DWIN_ICON_Show(ICON, TERN(selected,ICON_Prepare_0, ICON_Prepare_1), THEME_ICON_PREPARE_X, THEME_ICON_PREPARE_Y);
}

void Draw_ICON_Button_Control(boolean EN, boolean selected) {
  DWIN_ICON_Show(ICON, TERN(selected,ICON_Control_0, ICON_Control_1), THEME_ICON_CONTROL_X, THEME_ICON_CONTROL_Y);
}

void Draw_ICON_Button_Info(boolean EN, boolean selected) {
  DWIN_ICON_Show(ICON, TERN(selected,ICON_Info_0,ICON_Info_1), THEME_ICON_INFO_X, THEME_ICON_INFO_Y);
}

void Draw_ICON_Button_Leveling(boolean EN, boolean selected) { 
  DWIN_ICON_Show(ICON, TERN(selected,ICON_Leveling_0,ICON_Leveling_1), THEME_ICON_LEVELING_X, THEME_ICON_LEVELING_Y);
}

// TESTING AREA - KEEP OUT!
void Draw_Test() {
  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, DWIN_COLOR_GREEN, DWIN_LAYOUT_TITLE_BAR_TOPLEFT_X, DWIN_LAYOUT_TITLE_BAR_TOPLEFT_Y, DWIN_LAYOUT_TITLE_BAR_BOTTOMRIGHT_X, DWIN_LAYOUT_TITLE_BAR_BOTTOMRIGHT_Y);
  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, DWIN_COLOR_MAGENTA, DWIN_LAYOUT_DYNAMIC_AREA_TOPLEFT_X, DWIN_LAYOUT_DYNAMIC_AREA_TOPLEFT_Y, DWIN_LAYOUT_DYNAMIC_AREA_BOTTOMRIGHT_X, DWIN_LAYOUT_DYNAMIC_AREA_BOTTOMRIGHT_Y);
  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, DWIN_COLOR_BLUE, DWIN_LAYOUT_INDICATOR_AREA_TOPLEFT_X, DWIN_LAYOUT_INDICATOR_AREA_TOPLEFT_Y, DWIN_LAYOUT_INDICATOR_AREA_BOTTOMRIGHT_X, DWIN_LAYOUT_INDICATOR_AREA_BOTTOMRIGHT_Y);
}