#ifndef _SENSIRION_COLORS_H_
#define _SENSIRION_COLORS_H_

namespace sensirion::upt::display {

// Comment next line to use light theme
#define UPT_DISPLAY_USE_DARK_THEME

/**
 * TFT takes RGB565 values. Conversion from HEX or RGB24 formats was made using
 * Karlsen Henning's calculator at
 * http://www.rinkydinkelectronics.com/calc_rgb565.php
 */
// clang-format off
#define UPT_DISPLAY_YELLOW_COLOR    0xEE24      /* HEX: 0XEAC724  RGB: 234, 199,  36 */
#define UPT_DISPLAY_GREEN_COLOR     0x8589      /* HEX: 0X86B34E  RGB: 134, 179,  78 */
#define UPT_DISPLAY_GRAY_COLOR      0xAD74      /* HEX: 0XAAACA7  RGB: 170, 172, 167 */
#define UPT_DISPLAY_BLUE_COLOR      0xAE19      /* HEX: 0XA8C0C9  RGB: 168, 192, 201 */
#define UPT_DISPLAY_RED_COLOR       0xDC92      /* HEX: 0XDE9297  RGB: 222, 146, 151 */
// clang-format on

// Theme dependant colors
#ifdef UPT_DISPLAY_USE_DARK_THEME

// clang-format off
#define UPT_DISPLAY_BACKGROUND_COLOR        0x0000      /* HEX: 0XF0F5EC  RGB: 240, 245, 236 */
#define UPT_DISPLAY_TILE_PRIMARY_COLOR      0x4247      /* HEX: 0XDEE5D9  RGB: 222, 229, 217 */
#define UPT_DISPLAY_FONT_PRIMARY_COLOR      0xE73A      /* HEX: 0X424940  RGB:  66,  73,  64 */
// clang-format on

#else

// clang-format off
#define UPT_DISPLAY_BACKGROUND_COLOR        0xF7BD      /* HEX: 0XF0F5EC  RGB: 240, 245, 236 */
#define UPT_DISPLAY_TILE_PRIMARY_COLOR      0xDF3B      /* HEX: 0XDEE5D9  RGB: 222, 229, 217 */
#define UPT_DISPLAY_FONT_PRIMARY_COLOR      0x4248      /* HEX: 0X424940  RGB:  66,  73,  64 */

// clang-format on

#endif /* UPT_DISPLAY_USE_DARK_THEME */

}  // namespace sensirion::upt::display

#endif /* _SENSIRION_COLORS_H_ */
