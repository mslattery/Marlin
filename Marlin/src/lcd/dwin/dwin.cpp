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
#include "../ultralcd.h" // For MarlinUI

// Initialize Values
HMI_Flag HMI_flag{0};
millis_t dwin_heat_time = 0;

void HMI_Init(void) { }

void HMI_StartFrame(const bool with_update) { } // Startup Screen

void DWIN_Update(void) { }

void DWIN_CompletedHoming() { }

void Popup_Window_Temperature(const bool toohigh) { }

void MarlinUI::clear_lcd() { }

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
