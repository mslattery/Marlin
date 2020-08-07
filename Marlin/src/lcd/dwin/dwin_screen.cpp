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
#include "dwin_screen.h"

/**
 * 
 * dwin_screen.cpp
 *
 * Screen implementations
 * 
*/
void Screen_DrawMainMenu(boolean EN) { // Only use when changing to Main Menu, use Update instead for changes
  Draw_MainWindowBackground();
  Draw_Logo();
  if (EN) {
    Draw_TitleText(GET_TEXT_F(MSG_MAIN));
  } else {
    DWIN_Frame_AreaCopy(1, 2, 2, 271 - 244, 479 - 465, 14, 9); // "Home"  // TODO: Figure out how CN works.
  }
  Draw_ICON_Button_Print(EN, true);
  Draw_ICON_Button_Prepare(EN, false);
  Draw_ICON_Button_Control(EN, false);

  if (HAS_LEVELING) {
    Draw_ICON_Button_Leveling(EN, false); 
  } else {
    Draw_ICON_Button_Info(EN, false);
  }
}

// Updates MainMenu screen
void Screen_MainMenu_Update(boolean EN, int currentCursorPosition) { //TODO: Updates based on cursor with DWIN_Frame_AreaCopy(1, 1, 447, 271 - 243, 479 - 19, 58, 201);

  //Draw_TitleBar_Background();  // Clear the text ONLY FOR TESTING
  //char str[80]; sprintf(str, "Update %i, %i", EN, currentCursorPosition); Draw_TitleText(str); // ONLY FOR TESTING

  if ((currentCursorPosition==3) && (!HAS_LEVELING)) {  // If no bed leveling then position 3 is really position 4 (INFO).
      currentCursorPosition = 4; 
  }

  switch (currentCursorPosition)  {   
    case MainMenuScreen_Cursor_Print:
      Draw_ICON_Button_Print(EN, true);
      Draw_ICON_Button_Prepare(EN, false);
      Draw_ICON_Button_Control(EN, false);
      TERN(HAS_LEVELING, Draw_ICON_Button_Leveling(EN, false), Draw_ICON_Button_Info(EN, false));
      break;

    case MainMenuScreen_Cursor_Prepare:
      Draw_ICON_Button_Print(EN, false);
      Draw_ICON_Button_Prepare(EN, true);
      Draw_ICON_Button_Control(EN, false);
      TERN(HAS_LEVELING, Draw_ICON_Button_Leveling(EN, false), Draw_ICON_Button_Info(EN, false));
      break;

    case MainMenuScreen_Cursor_Control:
      Draw_ICON_Button_Print(EN, false);
      Draw_ICON_Button_Prepare(EN, false);
      Draw_ICON_Button_Control(EN, true);
      TERN(HAS_LEVELING, Draw_ICON_Button_Leveling(EN, false), Draw_ICON_Button_Info(EN, false));
      break;

    case MainMenuScreen_Cursor_Leveling:
      Draw_ICON_Button_Print(EN, false);
      Draw_ICON_Button_Prepare(EN, false);
      Draw_ICON_Button_Control(EN, false);
      TERN(HAS_LEVELING, Draw_ICON_Button_Leveling(EN, true), Draw_ICON_Button_Info(EN, false));
      break;

    case MainMenuScreen_Cursor_Info:
      Draw_ICON_Button_Print(EN, false);
      Draw_ICON_Button_Prepare(EN, false);
      Draw_ICON_Button_Control(EN, false);
      TERN(HAS_LEVELING, Draw_ICON_Button_Leveling(EN, false), Draw_ICON_Button_Info(EN, true));
      break;

    default:
      break;
  }
}

// Draws Info screen
void Screen_DrawInfoMenu(boolean EN) { 
  Draw_MainWindowBackground();
  if (EN) {
    Draw_TitleText(GET_TEXT_F(MSG_INFO_SCREEN));
  } else {
    DWIN_Frame_AreaCopy(1, 2, 2, 271 - 244, 479 - 465, 14, 9); // "Info"  // TODO: Figure out how CN works.
  } 

  #define TEXT "TESTING..."
  DWIN_Draw_String(false, false, THEME_INFO_FONT, THEME_INFO_TEXT_COLOR, THEME_COLOR_BACKGROUND_BLACK, DWIN_WIDTH/2, 50, (char*)TEXT);
  
  /*
  //MenuItem_Draw_Back(); // TODO: Draw Back Item

  DWIN_Draw_String(false, false, THEME_INFO_FONT, THEME_INFO_TEXT_COLOR, THEME_COLOR_BACKGROUND_BLACK, (DWIN_WIDTH - strlen(MACHINE_SIZE) * MENU_CHR_W) / THEME_INFO_LINE1_X, THEME_INFO_LINE1_Y, (char*)MACHINE_SIZE);
  DWIN_Draw_String(false, false, THEME_INFO_FONT, THEME_INFO_TEXT_COLOR, THEME_COLOR_BACKGROUND_BLACK, (DWIN_WIDTH - strlen(SHORT_BUILD_VERSION) * MENU_CHR_W) / THEME_INFO_LINE2_X, THEME_INFO_LINE2_Y, (char*)SHORT_BUILD_VERSION);
  DWIN_Draw_String(false, false, THEME_INFO_FONT, THEME_INFO_TEXT_COLOR, THEME_COLOR_BACKGROUND_BLACK, (DWIN_WIDTH - strlen(CORP_WEBSITE_C) * MENU_CHR_W) / THEME_INFO_LINE3_X, THEME_INFO_LINE3_Y, (char*)CORP_WEBSITE_C);
  LOOP_L_N(i, 3) {
    DWIN_ICON_Show(ICON, ICON_PrintSize + i, THEME_INFO_ICON_X, 99 + i * 73);
    DWIN_Draw_Line(THEME_COLOR_LINE, 16, MBASE(2) + i * 73, 256, 156 + i * 73);
  }
  */
}

// Indicators
void Screen_Indicators_Draw_Temperature_Hotend(float current, float target) { 
  DWIN_Draw_IntValue(DWIN_LCD_DRAWINT_WITH_BACKGROUND, DWIN_LCD_DRAWINT_WITH_ZEROFILL, DWIN_LCD_DRAWINT_ZEROMODE_LEADING_SPACE, THEME_FONT_STAT, DWIN_COLOR_WHITE, THEME_COLOR_BACKGROUND_BLACK, DWIN_LCD_DRAWINT_DIGITS, THEME_INDICATOR_HOTEND_X, THEME_INDICATOR_HOTEND_Y, current);
  DWIN_Draw_String(false, false, THEME_FONT_STAT, DWIN_COLOR_WHITE, THEME_COLOR_BACKGROUND_BLACK, 33 + 3 * STAT_CHR_W + 5, 383, (char*)"/");
  DWIN_Draw_IntValue(DWIN_LCD_DRAWINT_WITH_BACKGROUND, DWIN_LCD_DRAWINT_WITH_ZEROFILL, DWIN_LCD_DRAWINT_ZEROMODE_LEADING_SPACE, THEME_FONT_STAT, DWIN_COLOR_WHITE, THEME_COLOR_BACKGROUND_BLACK, DWIN_LCD_DRAWINT_DIGITS, THEME_INDICATOR_HOTEND_X + 4 * STAT_CHR_W + 6, 382, target);  
}

void Screen_Indicators_Update_Temperature_Hotend_Current(float temp) {
  DWIN_Draw_IntValue(DWIN_LCD_DRAWINT_WITH_BACKGROUND, DWIN_LCD_DRAWINT_WITH_ZEROFILL, DWIN_LCD_DRAWINT_ZEROMODE_LEADING_SPACE, THEME_FONT_STAT, DWIN_COLOR_WHITE, THEME_COLOR_BACKGROUND_BLACK, DWIN_LCD_DRAWINT_DIGITS, THEME_INDICATOR_HOTEND_X, THEME_INDICATOR_HOTEND_Y, temp);
}

void Screen_Indicators_Update_Temperature_Hotend_Target(float temp) {
  DWIN_Draw_IntValue(DWIN_LCD_DRAWINT_WITH_BACKGROUND, DWIN_LCD_DRAWINT_WITH_ZEROFILL, DWIN_LCD_DRAWINT_ZEROMODE_LEADING_SPACE, THEME_FONT_STAT, DWIN_COLOR_WHITE, THEME_COLOR_BACKGROUND_BLACK, DWIN_LCD_DRAWINT_DIGITS, 33 + 4 * STAT_CHR_W + 6, THEME_INDICATOR_HOTEND_Y, temp);
}

void Screen_Indicators_Draw_Temperature_Bed(float current, float target) { 
  DWIN_Draw_IntValue(DWIN_LCD_DRAWINT_WITH_BACKGROUND, DWIN_LCD_DRAWINT_WITH_ZEROFILL, DWIN_LCD_DRAWINT_ZEROMODE_LEADING_SPACE, THEME_FONT_STAT, DWIN_COLOR_WHITE, THEME_COLOR_BACKGROUND_BLACK, DWIN_LCD_DRAWINT_DIGITS, THEME_INDICATOR_BED_X, THEME_INDICATOR_BED_Y, current);
  DWIN_Draw_String(false, false, THEME_FONT_STAT, DWIN_COLOR_WHITE, THEME_COLOR_BACKGROUND_BLACK, 178 + 3 * STAT_CHR_W + 5, 383, (char*)"/");
  DWIN_Draw_IntValue(DWIN_LCD_DRAWINT_WITH_BACKGROUND, DWIN_LCD_DRAWINT_WITH_ZEROFILL, DWIN_LCD_DRAWINT_ZEROMODE_LEADING_SPACE, THEME_FONT_STAT, DWIN_COLOR_WHITE, THEME_COLOR_BACKGROUND_BLACK, DWIN_LCD_DRAWINT_DIGITS, THEME_INDICATOR_BED_X + 4 * STAT_CHR_W + 6, THEME_INDICATOR_BED_Y, target);
}

void Screen_Indicators_Update_Temperature_Bed_Current(float temp) {
    DWIN_Draw_IntValue(DWIN_LCD_DRAWINT_WITH_BACKGROUND, DWIN_LCD_DRAWINT_WITH_ZEROFILL, DWIN_LCD_DRAWINT_ZEROMODE_LEADING_SPACE, THEME_FONT_STAT, DWIN_COLOR_WHITE, THEME_COLOR_BACKGROUND_BLACK, DWIN_LCD_DRAWINT_DIGITS, THEME_INDICATOR_BED_X, THEME_INDICATOR_BED_Y, temp);
}

void Screen_Indicators_Update_Temperature_Bed_Target(float temp) {
  DWIN_Draw_IntValue(DWIN_LCD_DRAWINT_WITH_BACKGROUND, DWIN_LCD_DRAWINT_WITH_ZEROFILL, DWIN_LCD_DRAWINT_ZEROMODE_LEADING_SPACE, THEME_FONT_STAT, DWIN_COLOR_WHITE, THEME_COLOR_BACKGROUND_BLACK, DWIN_LCD_DRAWINT_DIGITS, THEME_INDICATOR_BED_X + 4 * STAT_CHR_W + 6, THEME_INDICATOR_BED_Y, temp);
}

void Screen_Indicators_Draw_Feedrate_Percentage(int16_t feedratePercentage) { 
  DWIN_Draw_IntValue(DWIN_LCD_DRAWINT_WITH_BACKGROUND, DWIN_LCD_DRAWINT_WITH_ZEROFILL, DWIN_LCD_DRAWINT_ZEROMODE_LEADING_SPACE, THEME_FONT_STAT, DWIN_COLOR_WHITE, THEME_COLOR_BACKGROUND_BLACK, DWIN_LCD_DRAWINT_DIGITS, 33 + 2 * STAT_CHR_W, THEME_INDICATOR_FEEDRATE_Y, feedratePercentage);
  DWIN_Draw_String(false, false, THEME_FONT_STAT, DWIN_COLOR_WHITE, THEME_COLOR_BACKGROUND_BLACK, 33 + (2 + 3) * STAT_CHR_W + 2, 429, (char*)"%");
}

void Screen_Indicators_Update_Feedrate_Percentage(float rate) {
  DWIN_Draw_IntValue(DWIN_LCD_DRAWINT_WITH_BACKGROUND, DWIN_LCD_DRAWINT_WITH_ZEROFILL, DWIN_LCD_DRAWINT_ZEROMODE_LEADING_SPACE, THEME_FONT_STAT, DWIN_COLOR_WHITE, THEME_COLOR_BACKGROUND_BLACK, DWIN_LCD_DRAWINT_DIGITS, 33 + 2 * STAT_CHR_W, THEME_INDICATOR_FEEDRATE_Y, rate);
}

void Screen_Indicators_Draw_ZOffset(float value) {
  if (value < 0) {
    DWIN_Draw_String(false, true, THEME_FONT_STAT, DWIN_COLOR_WHITE, THEME_FONT_STAT, THEME_INDICATOR_ZOFFSET_X - 6, THEME_INDICATOR_ZOFFSET_Y, (char*)"-");
    DWIN_Draw_FloatValue(true, true, 0, THEME_FONT_STAT, DWIN_COLOR_WHITE, THEME_FONT_STAT, 2, 2, THEME_INDICATOR_ZOFFSET_X, THEME_INDICATOR_ZOFFSET_Y, -value * 100);
  }
  else {
    DWIN_Draw_String(false, true, THEME_FONT_STAT, DWIN_COLOR_WHITE, THEME_FONT_STAT, THEME_INDICATOR_ZOFFSET_X - 6, THEME_INDICATOR_ZOFFSET_Y, (char*)" ");
    DWIN_Draw_FloatValue(true, true, 0, THEME_FONT_STAT, DWIN_COLOR_WHITE, THEME_FONT_STAT, 2, 2, THEME_INDICATOR_ZOFFSET_X, THEME_INDICATOR_ZOFFSET_Y, value * 100);
  }
}

// TODO: These moved from dwin.cpp, not updated yet!
inline void Draw_Menu_Icon(const uint8_t line, const uint8_t icon) {
  DWIN_ICON_Show(ICON, icon, 26, 46 + line * MLINE);
}

inline void Draw_Menu_Line(const uint8_t line, const uint8_t icon=0, const char * const label=nullptr) {
  if (label) DWIN_Draw_String(false, false, font8x16, DWIN_COLOR_WHITE, THEME_COLOR_BACKGROUND_BLACK, LBLX, 48 + line * MLINE, (char*)label);
  if (icon) Draw_Menu_Icon(line, icon);
  DWIN_Draw_Line(THEME_COLOR_LINE, 16, 29 + (line + 1) * MLINE, 256, 30 + (line + 1) * MLINE);
}

// The "Back" label is always on the first line
inline void Draw_Back_Label(boolean EN) {
  if (!EN)
    DWIN_Frame_AreaCopy(1, 129, 72, 271 - 115, 479 - 395, LBLX, MBASE(0));
  else
    DWIN_Frame_AreaCopy(1, 226, 179, 271 - 15, 479 - 290, LBLX, MBASE(0));
}

inline void Draw_Menu_Cursor(const uint8_t line) {
  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, THEME_COLOR_CURSOR, 0, 31 + line * MLINE, 14, 31 + (line + 1) * MLINE - 2);
}

// Draw "Back" line at the top
inline void MenuItem_Draw_Back(const bool is_sel=true) {
  Draw_Menu_Line(0, ICON_Back);
  Draw_Back_Label(true);
  if (is_sel) Draw_Menu_Cursor(0);
}

inline void Draw_Title_Bar_Background(void) {
  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, THEME_COLOR_BACKGROUND_BLUE, DWIN_LCD_COORD_TOPLEFT_X,  DWIN_LCD_COORD_TOPLEFT_X,  DWIN_LCD_COORD_RIGHTMOST_X,  DWIN_LAYOUT_TITLE_BAR_HEIGHT);
}

inline void Draw_Indicator_Frame_Background(void) {
  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, THEME_COLOR_BACKGROUND_BLACK, DWIN_LCD_COORD_LEFTMOST_X,  DWIN_HEIGHT-120,  DWIN_LCD_COORD_RIGHTMOST_X, DWIN_HEIGHT-1); // TODO: 120 pixels reserved needs to be addressed
}

void Screen_Indicators_Draw_Icons(void) {
  DWIN_ICON_Show(ICON, ICON_HotendTemp, THEME_ICON_HOTEND_X, THEME_ICON_HOTEND_Y);
  DWIN_ICON_Show(ICON, ICON_BedTemp, THEME_ICON_BED_X, THEME_ICON_BED_Y);
  DWIN_ICON_Show(ICON, ICON_Speed, THEME_ICON_FEEDRATE_X, THEME_ICON_FEEDRATE_Y);
  DWIN_ICON_Show(ICON, ICON_Zoffset, THEME_ICON_ZOFFSET_X, THEME_ICON_ZOFFSET_Y);
}