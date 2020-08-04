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

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(DWIN_CREALITY_LCD)
/**
 * dwin.cpp
 *
 * LCD display implementations for  DWIN by Creality3D.
 * This is the display on the Creality3d Ender 3 V2.
 */

#include "dwin.h"
#include "dwin_lcd.h" // For Draw items, like DWIN_Draw_Rectangle
#include "../ultralcd.h" // For MarlinUI
#include "../../module/temperature.h" // For thermalManager
#include "../../module/motion.h" // For feedrate_percentage

// Initialize Values
HMI_Flag HMI_flag{0};
millis_t dwin_heat_time = 0;
float zprobe_zoffset = 0;

uint16_t testCount = 0;

#define BABY_Z_VAR TERN(HAS_LEVELING, probe.offset.z, zprobe_zoffset)

/*
constexpr uint16_t TROWS = 6, MROWS = TROWS - 1,        // Total rows, and other-than-Back
                   TITLE_HEIGHT = 30,                   // Title bar height. SLATS - I moved to LAYOUT_TITLE_BAR_HEIGHT
                   MLINE = 53,                          // Menu line height
                   LBLX = 60,                           // Menu item label X
                   MENU_CHR_W = 8, STAT_CHR_W = 10;
*/
constexpr uint16_t STAT_CHR_W = 10; // TODO: This is used in layout need to understand, comes from the old static ints above

void HMI_Init(void) { }

void EachMomentUpdate(void) {
  DWIN_UpdateLCD();
}

void DWIN_Update(void) {
  EachMomentUpdate();   // Status update
  //HMI_SDCardUpdate();   // SD card update
  //DWIN_HandleScreen();  // Rotary encoder update
}

void DWIN_CompletedHoming() { }

void Popup_Window_Temperature(const bool toohigh) { }

inline void Clear_Menu_Area(void) {
  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, DWIN_COLOR_BACKGROUND_BLACK, DWIN_LCD_COORD_TOPLEFT_X,  LAYOUT_TITLE_BAR_HEIGHT+1, DWIN_WIDTH,  DWIN_HEIGHT - 120); // TODO: Original Lower Left was 272,360, the DWIN goes to 272,480.  Thinking these last 120 rows are reserved, to review.
}

inline void Clear_Title_Bar(void) {
  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, DWIN_COLOR_BACKGROUND_BLUE, DWIN_LCD_COORD_TOPLEFT_X,  DWIN_LCD_COORD_TOPLEFT_X,  DWIN_WIDTH,  LAYOUT_TITLE_BAR_HEIGHT);
}

void MarlinUI::clear_lcd() {
  //DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, DWIN_COLOR_Background_black, DWIN_LCD_COORD_TOPLEFT_X,  DWIN_LCD_COORD_TOPLEFT_Y, DWIN_LCD_COORD_BOTTOMRIGHT_X,  DWIN_LCD_COORD_BOTTOMRIGHT_Y);
  //Clear_Title_Bar();
  //Clear_Menu_Area();
}

#if ENABLED(SHOW_CUSTOM_BOOTSCREEN)
/*
    void MarlinUI::draw_custom_bootscreen(const uint8_t frame) { }
    void MarlinUI::show_custom_bootscreen() { }
    void MarlinUI::show_marlin_bootscreen() {  }
    void MarlinUI::show_bootscreen() {
      TERN_(SHOW_CUSTOM_BOOTSCREEN, show_custom_bootscreen());
      show_marlin_bootscreen();
    }
*/  
#endif

inline void Draw_Title_Bar_Background(void) {
  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, DWIN_COLOR_BACKGROUND_BLUE, DWIN_LCD_COORD_TOPLEFT_X,  DWIN_LCD_COORD_TOPLEFT_X,  DWIN_LCD_COORD_RIGHTMOST_X,  LAYOUT_TITLE_BAR_HEIGHT);
}

inline void Draw_Indicator_Frame_Background(void) {
  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, DWIN_COLOR_BACKGROUND_BLACK, DWIN_LCD_COORD_LEFTMOST_X,  DWIN_HEIGHT-120,  DWIN_LCD_COORD_RIGHTMOST_X, DWIN_HEIGHT-1); // TODO: 120 pixels reserved needs to be addressed
}

inline void Draw_Indicator_Temperature_Hotend(void) { // TODO: Work the locations into parameters
  DWIN_Draw_IntValue(true, true, 0, STAT_FONT, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, 3, 33, 382, thermalManager.temp_hotend[0].celsius);
  DWIN_Draw_IntValue(true, true, 0, STAT_FONT, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, 3, 33 + 4 * STAT_CHR_W + 6, 382, thermalManager.temp_hotend[0].target);  
}

inline void Draw_Indicator_Temperature_Bed(void) { // TODO: Work the locations into parameters
  DWIN_Draw_IntValue(true, true, 0, STAT_FONT, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, 3, 178, 382, thermalManager.temp_bed.celsius);
  DWIN_Draw_IntValue(true, true, 0, STAT_FONT, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, 3, 178 + 4 * STAT_CHR_W + 6, 382, thermalManager.temp_bed.target);
}

inline void Draw_Indicator_Feedrate(void) { // TODO: Work the locations into parameters
  DWIN_Draw_IntValue(true, true, 0, STAT_FONT, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, 3, 33 + 2 * STAT_CHR_W, 429, feedrate_percentage);
  DWIN_Draw_String(false, false, STAT_FONT, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, 33 + (2 + 3) * STAT_CHR_W + 2, 429, (char*)"%");
}

inline void Draw_Indicator_ZOffset(void) { // TODO: Work the locations into parameters
  // show_plus_or_minus(STAT_FONT, Background_black, 2, 2, 178, 429, BABY_Z_VAR * 100); // TODO: implement that show_plus_or_minus
  DWIN_Draw_String(false, false, STAT_FONT, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, 33 + 3 * STAT_CHR_W + 5, 383, (char*)"/");
  DWIN_Draw_String(false, false, STAT_FONT, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, 178 + 3 * STAT_CHR_W + 5, 383, (char*)"/");
}

inline void Clear_Main_Window(void) {
  //Clear_Title_Bar();
  Clear_Menu_Area();
}


/* Start of UI Loop */
void HMI_StartFrame(const bool with_update) {
  //  MarlinUI::clear_lcd();
  Clear_Main_Window();

  // Draw backgrounds
  Draw_Title_Bar_Background();
  Draw_Indicator_Frame_Background();

  // Test Items
  DWIN_Draw_String(false, false, STAT_FONT, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, DWIN_WIDTH/2, DWIN_HEIGHT/2, (char*)"Text Here");
  DWIN_Draw_Line(DWIN_COLOR_LINE_COLOR, 0, 100, DWIN_LCD_COORD_RIGHTMOST_X, 100);

/* -- TODO: Draw Icons
  DWIN_ICON_Show(ICON, ICON_HotendTemp, 13, 381);
  #if HOTENDS > 1
    // DWIN_ICON_Show(ICON,ICON_HotendTemp, 13, 381);
  #endif
  DWIN_ICON_Show(ICON, ICON_BedTemp,   158, 381);
  DWIN_ICON_Show(ICON, ICON_Speed,      13, 429);
  DWIN_ICON_Show(ICON, ICON_Zoffset,   158, 428);
*/
 
  if (with_update) {
    DWIN_UpdateLCD();
    delay(5);
  }
}

void MarlinUI::refresh() {
  // The card was mounted or unmounted
  // or some other status change occurred
}

#if ENABLED(LCD_PROGRESS_BAR)

  void MarlinUI::draw_progress_bar(const uint8_t percent) { }

#endif // LCD_PROGRESS_BAR

#if HAS_LCD_MENU

  #include "../menu/menu.h"

  #if ENABLED(LCD_HAS_STATUS_INDICATORS)

    void MarlinUI::update_indicators() { }

  #endif // LCD_HAS_STATUS_INDICATORS

#endif // HAS_LCD_MENU

#endif // HAS_CHARACTER_LCD
