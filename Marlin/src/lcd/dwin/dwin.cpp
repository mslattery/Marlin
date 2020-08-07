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

#define BABY_Z_VAR TERN(HAS_LEVELING, probe.offset.z, zprobe_zoffset)
#define ENCODER_WAIT    50

#define DWIN_SCROLL_UPDATE_INTERVAL 2000

// Initialize Values
HMI_Flag HMI_flag{0};
millis_t dwin_heat_time   = 0;
float zprobe_zoffset      = 0;
millis_t Encoder_ms       = 0; // Encoder related timing
int currentScreenIndex    = 0; // Used to store Screen location in menu tree
int currentCursorPosition = 0; // Used to store Cursor Postion on Screen
float last_temp_hotend_target = 0, last_temp_bed_target = 0;
float last_temp_hotend_current = 0, last_temp_bed_current = 0;
uint16_t last_feedrate = 0; // Feedspeed
float last_zoffset = 0; // Last zOffset

void HMI_Init(void) { }

inline ENCODER_DiffState get_encoder_state() {
  const millis_t ms = millis();
  if (PENDING(ms, Encoder_ms)) return ENCODER_DIFF_NO;
  const ENCODER_DiffState state = Encoder_ReceiveAnalyze();
  if (state != ENCODER_DIFF_NO) Encoder_ms = ms + ENCODER_WAIT;
  return state;
}

void ShowMainMenuScreen(void) {
  currentScreenIndex = MainMenuScreen;
  Screen_DrawMainMenu(!HMI_flag.language_flag);
}

void ShowInfoScreen(void) {
        currentScreenIndex = InfoScreen;
        currentCursorPosition = 0; // reset for new screen
        Screen_DrawInfoMenu(!HMI_flag.language_flag);
}

void HMI_MainMenu(void) {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (currentCursorPosition >= 3) { currentCursorPosition = 0; } else { currentCursorPosition += 1; } //  >= takes care of Leveling (3) vs Info (4)
    //Draw_TitleBar_Background();  // Clear the text ONLY FOR TESTING
    //char str[80]; sprintf(str, "Rotary CW %i", currentCursorPosition); 
    //Draw_TitleText(str); // ONLY FOR TESTING
    Screen_MainMenu_Update(!HMI_flag.language_flag, currentCursorPosition);
  }  
  else if (encoder_diffState == ENCODER_DIFF_CCW) {     
    if (currentCursorPosition == 0 ) { currentCursorPosition = 3; } else { currentCursorPosition-=1; }
    //Draw_TitleBar_Background();  // Clear the text ONLY FOR TESTING
    //char str[80]; sprintf(str, "Rotary CCW %i", currentCursorPosition); Draw_TitleText(str); // ONLY FOR TESTING
    Screen_MainMenu_Update(!HMI_flag.language_flag, currentCursorPosition);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {  
    //Draw_TitleBar_Background();  // Clear the text ONLY FOR TESTING
    //char str[80]; sprintf(str, "Rotary Enter %i", currentCursorPosition); Draw_TitleText(str); // ONLY FOR TESTING
    switch (currentCursorPosition)
    {
    case 0: // Print
      ShowInfoScreen(); // TEMP TESTING
      break;     
    case 1: // Prepare
      ShowInfoScreen(); // TEMP TESTING
      break;
    case 2: // Control
      ShowInfoScreen(); // TEMP TESTING
      break;
    case 3: // Leveling & Info
      if (HAS_LEVELING) {
        ShowInfoScreen(); // TEMP TESTING
      } else {
        ShowInfoScreen();
      }
      break;
    default:
      break;
    }
  }
  DWIN_UpdateLCD();
}

/* Info Screen */
void HMI_Info(void) {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (encoder_diffState == ENCODER_DIFF_ENTER) {
    ShowMainMenuScreen();
  }
  //DWIN_UpdateLCD();
}

void DWIN_HandleScreen(void) {

  Draw_TitleBar_Background();  // Clear the text ONLY FOR TESTING
  //char str[80]; sprintf(str, "HS Screen:%i Cursor:%i", currentScreenIndex, currentCursorPosition); Draw_TitleText(str); // ONLY FOR TESTING

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

void update_variable(void) {
  /* Bottom temperature update */
  float hotend_current = thermalManager.temp_hotend[0].celsius;
  float hotend_target = thermalManager.temp_hotend[0].target;
  float bed_current = thermalManager.temp_bed.celsius;
  float bed_target = thermalManager.temp_bed.target;
  float feedrate_current = feedrate_percentage; // TODO: This is hardcoded now, must come from somewhere
    
  if (hotend_current != last_temp_hotend_current) {
    Screen_Indicators_Update_Temperature_Hotend_Current(hotend_current);
    last_temp_hotend_current = hotend_current;
  }
  if (hotend_target != last_temp_hotend_target) {
    Screen_Indicators_Update_Temperature_Hotend_Target(hotend_target);
    last_temp_hotend_target = hotend_target;
  }
  if (bed_current != last_temp_bed_current) {
    Screen_Indicators_Update_Temperature_Bed_Current(bed_current);
    last_temp_bed_current = bed_current;
  }
  if (bed_target != last_temp_bed_target) {
    Screen_Indicators_Update_Temperature_Bed_Target(bed_target);
    last_temp_bed_target = bed_target;
  }
  if (feedrate_current != last_feedrate) {
    Screen_Indicators_Update_Feedrate_Percentage(feedrate_current);
    last_feedrate = feedrate_current;
  }
  if (zprobe_zoffset != last_zoffset) {
    Screen_Indicators_Draw_ZOffset(zprobe_zoffset);
    last_zoffset = zprobe_zoffset;
  }
  #if HAS_LEVELING
    if (last_probe_zoffset != probe.offset.z) {
      //show_plus_or_minus(STAT_FONT, Background_black, 2, 2, 178 + STAT_CHR_W, 429, probe.offset.z * 100);
      last_probe_zoffset = probe.offset.z;
    }
  #else
    if (last_zoffset != zprobe_zoffset) {
      //show_plus_or_minus(STAT_FONT, Background_black, 2, 2, 178 + STAT_CHR_W, 429, zprobe_zoffset * 100);
      last_zoffset = zprobe_zoffset;
    }
  #endif
}

void EachMomentUpdate(void) {
  static millis_t next_rts_update_ms = 0;
  const millis_t ms = millis();
  if (PENDING(ms, next_rts_update_ms)) return;
  next_rts_update_ms = ms + DWIN_SCROLL_UPDATE_INTERVAL;
  update_variable();
  DWIN_UpdateLCD();
}

// Main UI Loop - Continually called after startup
void DWIN_Update(void) {
  EachMomentUpdate();    // Status Updates
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
  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, THEME_COLOR_BACKGROUND_BLUE, DWIN_LCD_COORD_TOPLEFT_X,  DWIN_LCD_COORD_TOPLEFT_X,  DWIN_LCD_COORD_RIGHTMOST_X,  DWIN_LAYOUT_TITLE_BAR_HEIGHT);
}

inline void Draw_Indicator_Frame_Background(void) {
  DWIN_Draw_Rectangle(DWIN_DRAW_MODE_FILL, THEME_COLOR_BACKGROUND_BLACK, DWIN_LCD_COORD_LEFTMOST_X,  DWIN_HEIGHT-120,  DWIN_LCD_COORD_RIGHTMOST_X, DWIN_HEIGHT-1); // TODO: 120 pixels reserved needs to be addressed
}

inline void DrawIndicators() {
  // Draw indicators
  Draw_Indicator_Frame_Background();
  Screen_Indicators_Draw_Icons();
  Screen_Indicators_Draw_Temperature_Hotend(thermalManager.temp_hotend[0].celsius, thermalManager.temp_hotend[0].target);
  Screen_Indicators_Draw_Temperature_Bed(thermalManager.temp_bed.celsius, thermalManager.temp_bed.target);
  Screen_Indicators_Draw_Feedrate_Percentage(feedrate_percentage);
  Screen_Indicators_Draw_ZOffset(0.00);
}

/* Start of UI Loop - This is only called once at the startup of the LCD */
void HMI_StartFrame(const bool with_update) {
  Screen_DrawMainMenu(!HMI_flag.language_flag);
  DrawIndicators();
  if (with_update) {
    DWIN_UpdateLCD();
    delay(5);
  }
  // Screen_MainMenu_Update(!HMI_flag.language_flag, currentCursorPosition); // TODO: See if needed
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
