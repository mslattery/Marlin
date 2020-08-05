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
void Screen_DrawMainMenu(boolean EN) { // EN is English
  Draw_MainWindowBackground();
  Draw_Logo();
  if (EN) {
    Draw_TitleText(GET_TEXT_F(MSG_MAIN));
  } else {
    DWIN_Frame_AreaCopy(1, 2, 2, 271 - 244, 479 - 465, 14, 9); // "Home"  // TODO: Figure out how CN works.
  }

  Draw_ICON_Print(EN);
  //ICON_Prepare();
  //ICON_Control();
  //TERN(HAS_LEVELING, ICON_Leveling, ICON_StartInfo)(select_page.now == 3);
}