#ifndef AMOLED_DISPLAY_H
#define AMOLED_DISPLAY_H

#include "IDisplay.h"

#ifdef LILYGO_TDISPLAY_AMOLED_SERIES
#include <LilyGo_AMOLED.h>
#include <TFT_eSPI.h>

namespace sensirion::upt::display {

class AmoledDisplay : public IDisplay {
  public:
    AmoledDisplay();
    ~AmoledDisplay() override = default;

    void init(Orientation orientation) override;
    void setRotation(Orientation orientation) override;

    uint16_t getWidth() const override;
    uint16_t getHeight() const override;
    Orientation getRotation() const override;

    void fillScreen(uint16_t color) override;
    void fillRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                       uint16_t r, uint16_t color) override;
    void drawVLine(uint16_t x, uint16_t y, uint16_t h, uint16_t color) override;
    void drawHLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color) override;
    void drawXBitmap(uint16_t x, uint16_t y, const uint8_t* bitmap, uint16_t w,
                     uint16_t h, uint16_t color) override;

    void drawText(const char* text, uint16_t x, uint16_t y, Font font,
                  uint16_t color, uint16_t bgColor) override;
    uint16_t getTextWidth(const char* text, Font font) const override;
    uint16_t getFontHeight(Font font) const override;

    void flush() override;

  private:
    mutable LilyGo_AMOLED _amoled;
    mutable TFT_eSPI _tft;
    mutable TFT_eSprite _spr;

    void _loadFont(Font font) const;
    static uint8_t _mapRotation(Orientation orientation);
};

}  // namespace sensirion::upt::display
#endif  // LILYGO_TDISPLAY_AMOLED_SERIES

#endif /* AMOLED_DISPLAY_H */
