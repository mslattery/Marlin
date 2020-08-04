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

#define BABY_Z_VAR TERN(HAS_LEVELING, probe.offset.z, zprobe_zoffset)
#define ENCODER_WAIT    20

#ifndef MACHINE_SIZE
  #define MACHINE_SIZE "220x220x250"
#endif
#ifndef CORP_WEBSITE_C
  #define CORP_WEBSITE_C "www.cxsw3d.com"
#endif
#ifndef CORP_WEBSITE_E
  #define CORP_WEBSITE_E "www.creality.com"
#endif

// Initialize Values
HMI_Flag HMI_flag{0};
millis_t dwin_heat_time = 0;
float zprobe_zoffset    = 0;
millis_t Encoder_ms     = 0; // Encoder related timing
int currentScreenIndex = 0, lastScreenIndex = 0; // Used to store location in menu tree

/*
constexpr uint16_t TROWS = 6, MROWS = TROWS - 1,        // Total rows, and other-than-Back
                   TITLE_HEIGHT = 30,                   // Title bar height. SLATS - I moved to LAYOUT_TITLE_BAR_HEIGHT
                   MENU_CHR_W = 8, STAT_CHR_W = 10;
*/
constexpr uint16_t MLINE = 53,                          // Menu line height
                   LBLX = 60,                           // Menu item label X
                   MENU_CHR_W = 8,                      // Menu Char Width
                   STAT_CHR_W = 10;                     // TODO: This is used in layout need to understand, STATIC CHAR WIDTH!

#define MBASE(L) (49 + (L)*MLINE)

void HMI_Init(void) { }

inline ENCODER_DiffState get_encoder_state() {
  const millis_t ms = millis();
  if (PENDING(ms, Encoder_ms)) return ENCODER_DIFF_NO;
  const ENCODER_DiffState state = Encoder_ReceiveAnalyze();
  if (state != ENCODER_DIFF_NO) Encoder_ms = ms + ENCODER_WAIT;
  return state;
}

// Clear Region
inline void Draw_Test() {
  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, DWIN_COLOR_GREEN, DWIN_LAYOUT_TITLE_BAR_TOPLEFT_X, DWIN_LAYOUT_TITLE_BAR_TOPLEFT_Y, DWIN_LAYOUT_TITLE_BAR_BOTTOMRIGHT_X, DWIN_LAYOUT_TITLE_BAR_BOTTOMRIGHT_Y);
  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, DWIN_COLOR_MAGENTA, DWIN_LAYOUT_DYNAMIC_AREA_TOPLEFT_X, DWIN_LAYOUT_DYNAMIC_AREA_TOPLEFT_Y, DWIN_LAYOUT_DYNAMIC_AREA_BOTTOMRIGHT_X, DWIN_LAYOUT_DYNAMIC_AREA_BOTTOMRIGHT_Y);
  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, DWIN_COLOR_BLUE, DWIN_LAYOUT_INDICATOR_AREA_TOPLEFT_X, DWIN_LAYOUT_INDICATOR_AREA_TOPLEFT_Y, DWIN_LAYOUT_INDICATOR_AREA_BOTTOMRIGHT_X, DWIN_LAYOUT_INDICATOR_AREA_BOTTOMRIGHT_Y);
}

inline void Draw_TitleBar_Background(void) {
  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, DWIN_COLOR_BACKGROUND_BLUE, DWIN_LAYOUT_TITLE_BAR_TOPLEFT_X, DWIN_LAYOUT_TITLE_BAR_TOPLEFT_Y, DWIN_LAYOUT_TITLE_BAR_BOTTOMRIGHT_X, DWIN_LAYOUT_TITLE_BAR_BOTTOMRIGHT_Y);
}

inline void Draw_DynamicArea_Background(void) {
  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, DWIN_COLOR_BACKGROUND_BLACK, DWIN_LAYOUT_DYNAMIC_AREA_TOPLEFT_X, DWIN_LAYOUT_DYNAMIC_AREA_TOPLEFT_Y, DWIN_LAYOUT_DYNAMIC_AREA_BOTTOMRIGHT_X, DWIN_LAYOUT_DYNAMIC_AREA_BOTTOMRIGHT_Y);
}

//inline void Clear_Menu_Area(void) {
//  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, DWIN_COLOR_BACKGROUND_BLACK, DWIN_LCD_COORD_TOPLEFT_X,  DWIN_LAYOUT_TITLE_BAR_HEIGHT+1, DWIN_WIDTH,  DWIN_HEIGHT - 120); // TODO: Original Lower Left was 272,360, the DWIN goes to 272,480.  Thinking these last 120 rows are reserved, to review.
//}

inline void Clear_Main_Window(void) {
  Draw_TitleBar_Background();
  Draw_DynamicArea_Background();
}
// End Clear Region

inline void Draw_Menu_Icon(const uint8_t line, const uint8_t icon) {
  DWIN_ICON_Show(ICON, icon, 26, 46 + line * MLINE);
}

inline void Draw_Menu_Line(const uint8_t line, const uint8_t icon=0, const char * const label=nullptr) {
  if (label) DWIN_Draw_String(false, false, font8x16, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, LBLX, 48 + line * MLINE, (char*)label);
  if (icon) Draw_Menu_Icon(line, icon);
  DWIN_Draw_Line(DWIN_COLOR_LINE, 16, 29 + (line + 1) * MLINE, 256, 30 + (line + 1) * MLINE);
}

// The "Back" label is always on the first line
inline void Draw_Back_Label(void) {
  if (HMI_flag.language_flag)
    DWIN_Frame_AreaCopy(1, 129, 72, 271 - 115, 479 - 395, LBLX, MBASE(0));
  else
    DWIN_Frame_AreaCopy(1, 226, 179, 271 - 15, 479 - 290, LBLX, MBASE(0));
}

inline void Draw_Menu_Cursor(const uint8_t line) {
  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, DWIN_COLOR_CURSOR, 0, 31 + line * MLINE, 14, 31 + (line + 1) * MLINE - 2);
}

// Draw "Back" line at the top
inline void MenuItem_Draw_Back(const bool is_sel=true) {
  Draw_Menu_Line(0, ICON_Back);
  Draw_Back_Label();
  if (is_sel) Draw_Menu_Cursor(0);
}

void HMI_MainMenu(void) {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, DWIN_COLOR_BACKGROUND_BLACK, 110, 200, DWIN_LCD_COORD_RIGHTMOST_X, 250); // Clear the text first

  if (encoder_diffState == ENCODER_DIFF_CW) {
    DWIN_Draw_String(false, false, STAT_FONT, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, 110, 200, (char*)"ROTARY CW");
  }  
  else if (encoder_diffState == ENCODER_DIFF_CCW) {  
    DWIN_Draw_String(false, false, STAT_FONT, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, 110, 200, (char*)"ROTARY CCW");
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {  
    DWIN_Draw_String(false, false, STAT_FONT, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, 110, 200, (char*)"ROTARY ENTER");
    currentScreenIndex = InfoScreen;
  }
  DWIN_UpdateLCD();
}

inline void Draw_Title_Text(const __FlashStringHelper * title) {
  DWIN_Draw_String(false, false, HEADER_FONT, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLUE, 14, 4, (char*)title);
}

inline void Draw_Info_Menu() {
  Clear_Main_Window();
  Draw_Title_Text(GET_TEXT_F(MSG_INFO_SCREEN));

  MenuItem_Draw_Back();

  DWIN_Draw_String(false, false, font8x16, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, (DWIN_WIDTH - strlen(MACHINE_SIZE) * MENU_CHR_W) / 2, 122, (char*)MACHINE_SIZE);
  DWIN_Draw_String(false, false, font8x16, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, (DWIN_WIDTH - strlen(SHORT_BUILD_VERSION) * MENU_CHR_W) / 2, 195, (char*)SHORT_BUILD_VERSION);
  DWIN_Draw_String(false, false, font8x16, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, (DWIN_WIDTH - strlen(CORP_WEBSITE_C) * MENU_CHR_W) / 2, 268, (char*)CORP_WEBSITE_C);

  LOOP_L_N(i, 3) {
    DWIN_ICON_Show(ICON, ICON_PrintSize + i, 26, 99 + i * 73);
    DWIN_Draw_Line(DWIN_COLOR_LINE, 16, MBASE(2) + i * 73, 256, 156 + i * 73);
  }
}

void Draw_MainMenu(void) {
  currentScreenIndex = MainMenuScreen;
  Clear_Main_Window();
  if (HMI_flag.language_flag) {
    DWIN_Frame_AreaCopy(1, 2, 2, 271 - 244, 479 - 465, 14, 9); // "Home"
  }
  else {
    Draw_Title_Text(GET_TEXT_F(MSG_MAIN));
  }
  DWIN_ICON_Show(ICON, ICON_LOGO, 71, 52);
  //ICON_Print();
  //ICON_Prepare();
  //ICON_Control();
  //TERN(HAS_LEVELING, ICON_Leveling, ICON_StartInfo)(select_page.now == 3);
}

/* Info Screen */
void HMI_Info(void) {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (encoder_diffState == ENCODER_DIFF_ENTER) {
    Draw_MainMenu();
  } else {
    Draw_Info_Menu();
    //Draw_Test();
  }
  DWIN_UpdateLCD();
}

void DWIN_HandleScreen(void) {
  switch (currentScreenIndex) {
    case MainMenuScreen:              HMI_MainMenu(); break;
    case InfoScreen:                  HMI_Info(); break;
    /*
    case SelectFile:            HMI_SelectFile(); break;
    case Prepare:               HMI_Prepare(); break;
    case Control:               HMI_Control(); break;
    case Leveling:              break;
    case PrintProcess:          HMI_Printing(); break;
    case Print_window:          HMI_PauseOrStop(); break;
    case AxisMove:              HMI_AxisMove(); break;
    case TemperatureID:         HMI_Temperature(); break;
    case Motion:                HMI_Motion(); break;
    case Tune:                  HMI_Tune(); break;
    case PLAPreheat:            HMI_PLAPreheatSetting(); break;
    case ABSPreheat:            HMI_ABSPreheatSetting(); break;
    case MaxSpeed:              HMI_MaxSpeed(); break;
    case MaxAcceleration:       HMI_MaxAcceleration(); break;
    case MaxCorner:             HMI_MaxCorner(); break;
    case Step:                  HMI_Step(); break;
    case Move_X:                HMI_Move_X(); break;
    case Move_Y:                HMI_Move_Y(); break;
    case Move_Z:                HMI_Move_Z(); break;
    case Extruder:              HMI_Move_E(); break;
    case Homeoffset:            HMI_Zoffset(); break;
    #if HAS_HOTEND
      case ETemp:               HMI_ETemp(); break;
    #endif
    #if HAS_HEATED_BED
      case BedTemp:             HMI_BedTemp(); break;
    #endif
    #if HAS_FAN
      case FanSpeed:            HMI_FanSpeed(); break;
    #endif
    case PrintSpeed:            HMI_PrintSpeed(); break;
    case MaxSpeed_value:        HMI_MaxFeedspeedXYZE(); break;
    case MaxAcceleration_value: HMI_MaxAccelerationXYZE(); break;
    case MaxCorner_value:       HMI_MaxCornerXYZE(); break;
    case Step_value:            HMI_StepXYZE(); break;
    */
    default: break;
  }
}

// Main UI Loop
void DWIN_Update(void) {
  //HMI_SDCardUpdate();   // SD card update
  DWIN_HandleScreen();  // Handle current screen state
}

void DWIN_CompletedHoming() { }

void Popup_Window_Temperature(const bool toohigh) { }

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
  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, DWIN_COLOR_BACKGROUND_BLUE, DWIN_LCD_COORD_TOPLEFT_X,  DWIN_LCD_COORD_TOPLEFT_X,  DWIN_LCD_COORD_RIGHTMOST_X,  DWIN_LAYOUT_TITLE_BAR_HEIGHT);
}

inline void Draw_Indicator_Frame_Background(void) {
  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, DWIN_COLOR_BACKGROUND_BLACK, DWIN_LCD_COORD_LEFTMOST_X,  DWIN_HEIGHT-120,  DWIN_LCD_COORD_RIGHTMOST_X, DWIN_HEIGHT-1); // TODO: 120 pixels reserved needs to be addressed
}

inline void Draw_Indicator_Temperature_Hotend(void) { // TODO: Work the locations into parameters
  DWIN_Draw_IntValue(true, true, 0, STAT_FONT, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, 3, 33, 382, thermalManager.temp_hotend[0].celsius);
  DWIN_Draw_String(false, false, STAT_FONT, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, 33 + 3 * STAT_CHR_W + 5, 383, (char*)"/");
  DWIN_Draw_IntValue(true, true, 0, STAT_FONT, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, 3, 33 + 4 * STAT_CHR_W + 6, 382, thermalManager.temp_hotend[0].target);  
}

inline void Draw_Indicator_Temperature_Bed(void) { // TODO: Work the locations into parameters
  DWIN_Draw_IntValue(true, true, 0, STAT_FONT, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, 3, 178, 382, thermalManager.temp_bed.celsius);
  DWIN_Draw_String(false, false, STAT_FONT, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, 178 + 3 * STAT_CHR_W + 5, 383, (char*)"/");
  DWIN_Draw_IntValue(true, true, 0, STAT_FONT, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, 3, 178 + 4 * STAT_CHR_W + 6, 382, thermalManager.temp_bed.target);
}

inline void Draw_Indicator_Feedrate(void) { // TODO: Work the locations into parameters
  DWIN_Draw_IntValue(true, true, 0, STAT_FONT, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, 3, 33 + 2 * STAT_CHR_W, 429, feedrate_percentage);
  DWIN_Draw_String(false, false, STAT_FONT, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, 33 + (2 + 3) * STAT_CHR_W + 2, 429, (char*)"%");
}

inline void Draw_Indicator_ZOffset(void) { // TODO: Work the locations into parameters
  // show_plus_or_minus(STAT_FONT, Background_black, 2, 2, 178, 429, BABY_Z_VAR * 100); // TODO: implement that show_plus_or_minus
}

/* Start of UI Loop */
void HMI_StartFrame(const bool with_update) {
  Clear_Main_Window();

  // Draw backgrounds
  Draw_Title_Bar_Background();
  Draw_Indicator_Frame_Background();

  // Test Items
  //DWIN_Draw_String(false, false, STAT_FONT, DWIN_COLOR_WHITE, DWIN_COLOR_BACKGROUND_BLACK, DWIN_WIDTH/2, DWIN_HEIGHT/2, (char*)"Text Here");
  //DWIN_Draw_Line(DWIN_COLOR_LINE_COLOR, 0, 100, DWIN_LCD_COORD_RIGHTMOST_X, 100);

// Draw indicators
  Draw_Indicator_Temperature_Hotend();
  Draw_Indicator_Temperature_Bed();
  Draw_Indicator_Feedrate();
  Draw_Indicator_ZOffset();

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
