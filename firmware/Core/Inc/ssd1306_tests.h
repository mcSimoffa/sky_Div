#ifndef __SSD1306_TEST_H__
#define __SSD1306_TEST_H__

#include <stdbool.h>
#include "ssd1306.h"
typedef enum
{
  FIT_LEFT,
  FIT_RIGHT
} bar_type_t;

/*---------------------------------------------------------------
API for display altitude
parameter [in] altitude - altitude in meters
          [in] unitKilometrs - flag to indicate in kilometers
                                like '7.1' for example
----------------------------------------------------------------*/
void altitude_display(int16_t altitude, bool unitKilometrs);

void tempr_display(int8_t tempr);

void progress_bar(bar_type_t _type, uint8_t level);

/*---------------------------------------------------------------
API for display a battery icon on a display
the icon fit are customizing by defines inside
parameter [in] level - sprips quatity on the icon
          [in] isFlash - the battery boundary will be flash
                          (to charge process indicate)
----------------------------------------------------------------*/
void battery_bar(uint8_t level, bool isFlash);


/*---------------------------------------------
function for display time like HH:MM
it should launck once per second for bliking ':'
correctly
parameter hours and mins in RTC_FORMAT_BIN
---------------------------------------------- */
void time_display(uint8_t _hour, uint8_t _min);
#endif // __SSD1306_TEST_H__
