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
#include "../../inc/MarlinConfigPre.h" // for HAS_LEVELING

#ifndef HAS_LEVELING  // If bed leveling not defined
#define HAS_LEVELING 0
#endif

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

void Screen_MainMenu_Update(boolean EN, int currentCursorPosition) { //TODO: Updates based on cursor with DWIN_Frame_AreaCopy(1, 1, 447, 271 - 243, 479 - 19, 58, 201);

  Draw_TitleBar_Background();  // Clear the text ONLY FOR TESTING
  char str[80]; sprintf(str, "Update %i, %i", EN, currentCursorPosition); Draw_TitleText(str); // ONLY FOR TESTING

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