#include <stdint.h>

#ifndef __SSD1306_FONTS_H__
#define __SSD1306_FONTS_H__

#include "ssd1306_conf.h"

typedef struct 
{
	const uint8_t FontWidth;    /*!< Font width in pixels */
	uint8_t       FontHeight;   /*!< Font height in pixels */
        uint8_t       With_in_bytes;
        uint8_t       min_code;
        uint8_t       max_code;
	const uint8_t *data; /*!< Pointer to data font data array */
} FontDef;

#ifdef SSD1306_INCLUDE_FONT_12x22
extern FontDef Font_12x22;
#endif

#ifdef SSD1306_INCLUDE_FONT_18x21
extern FontDef Font_18x21;
#endif

#endif // __SSD1306_FONTS_H__
