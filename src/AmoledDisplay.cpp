#include "AmoledDisplay.h"

#ifdef LILYGO_TDISPLAY_AMOLED_SERIES
#include "fonts/DefaultFont.h"

namespace sensirion::upt::display {

AmoledDisplay::AmoledDisplay() : _spr(&_tft) {
}

void AmoledDisplay::init(const Orientation orientation) {
    _amoled.begin();
    _amoled.setRotation(_mapRotation(orientation));
    _spr.setColorDepth(16);
    _spr.createSprite(static_cast<int16_t>(_amoled.width()),
                      static_cast<int16_t>(_amoled.height()));
}

void AmoledDisplay::setRotation(Orientation orientation) {
    _amoled.setRotation(_mapRotation(orientation));
    _spr.deleteSprite();
    _spr.createSprite(static_cast<int16_t>(_amoled.width()),
                      static_cast<int16_t>(_amoled.height()));
}

uint16_t AmoledDisplay::getWidth() const {
    return _amoled.width();
}

uint16_t AmoledDisplay::getHeight() const {
    return _amoled.height();
}

Orientation AmoledDisplay::getRotation() const {
    const uint8_t rot = _amoled.getRotation();
    return (rot == _mapRotation(Orientation::landscape))
               ? Orientation::landscape
               : Orientation::portrait;
}

void AmoledDisplay::fillScreen(const uint16_t color) {
    _spr.fillScreen(color);
}

void AmoledDisplay::fillRoundRect(const uint16_t x, const uint16_t y,
                                  const uint16_t w, const uint16_t h,
                                  const uint16_t r, const uint16_t color) {
    _spr.fillRoundRect(x, y, w, h, r, color);
}

void AmoledDisplay::drawVLine(const uint16_t x, const uint16_t y,
                              const uint16_t h, const uint16_t color) {
    _spr.drawFastVLine(x, y, h, color);
}

void AmoledDisplay::drawHLine(const uint16_t x, const uint16_t y,
                              const uint16_t w, const uint16_t color) {
    _spr.drawFastHLine(x, y, w, color);
}

void AmoledDisplay::drawXBitmap(const uint16_t x, const uint16_t y,
                                const uint8_t* bitmap, const uint16_t w,
                                const uint16_t h, const uint16_t color) {
    _spr.drawXBitmap(static_cast<int16_t>(x), static_cast<int16_t>(y), bitmap,
                     static_cast<int16_t>(w), static_cast<int16_t>(h), color);
}

void AmoledDisplay::drawText(const char* text, const uint16_t x,
                             const uint16_t y, const Font font,
                             const uint16_t color, const uint16_t bgColor) {
    _loadFont(font);
    _spr.setTextColor(color, bgColor);
    _spr.setCursor(static_cast<int16_t>(x), static_cast<int16_t>(y));
    _spr.print(text);
    _spr.unloadFont();
}

uint16_t AmoledDisplay::getTextWidth(const char* text, const Font font) const {
    _loadFont(font);
    const uint16_t width = _spr.textWidth(text);
    _spr.unloadFont();
    return width;
}

uint16_t AmoledDisplay::getFontHeight(const Font font) const {
    _loadFont(font);
    const uint16_t height = _spr.fontHeight();
    _spr.unloadFont();
    return height;
}

void AmoledDisplay::flush() {
    _amoled.pushColors(0, 0, _amoled.width(), _amoled.height(),
                       static_cast<uint16_t*>(_spr.getPointer()));
}

void AmoledDisplay::_loadFont(const Font font) const {
    switch (font) {
        case Font::SMALL:
            _spr.loadFont(UPT_DISPLAY_FONT_SMALL);
            break;
        case Font::MEDIUM:
        case Font::NORMAL:
            _spr.loadFont(UPT_DISPLAY_FONT_MEDIUM);
            break;
        case Font::LARGE:
            _spr.loadFont(UPT_DISPLAY_FONT_LARGE);
            break;
        case Font::XLARGE:
            _spr.loadFont(UPT_DISPLAY_FONT_XLARGE);
            break;
    }
}

uint8_t AmoledDisplay::_mapRotation(const Orientation orientation) {
    if (orientation == Orientation::landscape) {
        return 0;
    }

    return 1;
}

}  // namespace sensirion::upt::display
#endif  // LILYGO_TDISPLAY_AMOLED_SERIES
