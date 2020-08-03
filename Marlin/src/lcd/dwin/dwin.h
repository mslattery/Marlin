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
 * dwin.cpp
 *
 * LCD display implementations for DWIN by Creality3D.
 * This is the display on the Creality3d Ender 3 V2.
 */

#include "../../MarlinCore.h" // for millis_t

extern millis_t dwin_heat_time;

typedef struct {
  bool language_flag;  // 0: EN, 1: CN
  bool pause_flag:1;
  bool print_finish:1;
  bool confirm_flag:1;
  bool select_flag:1;
  bool home_flag:1;
  bool heat_flag:1;  // 0: heating done  1: during heating
  #if HAS_HOTEND
    bool ETempTooLow_flag:1;
  #endif
  #if HAS_LEVELING
    bool leveling_offset_flag:1;
  #endif
  #if HAS_FAN
    char feedspeed_flag;
  #endif
  char acc_flag;
  char corner_flag;
  char step_flag;
} HMI_Flag;
extern HMI_Flag    HMI_flag;

#if HAS_HOTEND
  void Popup_Window_Temperature(const bool toohigh);
#endif

void HMI_Init(void);
void HMI_StartFrame(const bool with_update); // startup screen

void DWIN_Update(void);
void DWIN_CompletedHoming(void);
