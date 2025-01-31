/***************************************************************************//**
 * @file
 * @brief OS dependent sleep functionality.
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#include "app_sleep.h"

#if defined(POSIX) && POSIX == 1
inline void app_sleep_us(uint64_t usec)
{
  usleep(usec);
}
#else
void app_sleep_us(uint64_t usec)
{
  HANDLE timer;
  LARGE_INTEGER due_time;

  if (usec == 0) {
    SwitchToThread();
    return;
  }

  // Convert to 100 nanosec interval, negative value indicates relative time
  due_time.QuadPart = -((10 * usec) - 1);

  timer = CreateWaitableTimer(NULL, TRUE, NULL);
  SetWaitableTimer(timer, &due_time, 0, NULL, NULL, 0);
  WaitForSingleObject(timer, INFINITE);
  CloseHandle(timer);
}
#endif
