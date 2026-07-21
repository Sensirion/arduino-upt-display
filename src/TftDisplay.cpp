#include "TftDisplay.h"
#include "fonts/DefaultFont.h"

namespace sensirion::upt::display {

TftDisplay::TftDisplay() : _tft(), _spr(&_tft) {
}

void TftDisplay::init(Orientation orientation) {
    _tft.init();
    _tft.setTextWrap(false);
    _tft.setRotation(static_cast<uint8_t>(orientation));
    _spr.setColorDepth(16);
}

void TftDisplay::setRotation(Orientation orientation) {
    _tft.setRotation(static_cast<uint8_t>(orientation));
}

uint16_t TftDisplay::getWidth() const {
    return _tft.width();
}

uint16_t TftDisplay::getHeight() const {
    return _tft.height();
}

Orientation TftDisplay::getRotation() const {
    return static_cast<Orientation>(_tft.getRotation());
}

void TftDisplay::fillScreen(uint16_t color) {
    _tft.fillScreen(color);
}

void TftDisplay::fillRoundRect(const uint16_t x, const uint16_t y,
                               const uint16_t w, const uint16_t h,
                               const uint16_t r, const uint16_t color) {
    _tft.fillRoundRect(x, y, w, h, r, color);
}

void TftDisplay::drawVLine(const uint16_t x, const uint16_t y, const uint16_t h,
                           const uint16_t color) {
    _tft.drawFastVLine(x, y, h, color);
}

void TftDisplay::drawHLine(const uint16_t x, const uint16_t y, const uint16_t w,
                           const uint16_t color) {
    _tft.drawFastHLine(x, y, w, color);
}

void TftDisplay::drawXBitmap(const uint16_t x, const uint16_t y,
                             const uint8_t* bitmap, const uint16_t w,
                             const uint16_t h, const uint16_t color) {
    _tft.drawXBitmap(x, y, bitmap, w, h, color);
}

void TftDisplay::drawText(const char* text, const uint16_t x, const uint16_t y,
                          const Font font, const uint16_t color,
                          const uint16_t bgColor) {
    _loadFont(font);
    _spr.setTextColor(color, bgColor);
    _tft.setCursor(x, y);
    _spr.printToSprite(text);
    _spr.unloadFont();
}

uint16_t TftDisplay::getTextWidth(const char* text, const Font font) const {
    _loadFont(font);
    uint16_t width = _spr.textWidth(text);
    _spr.unloadFont();
    return width;
}

uint16_t TftDisplay::getFontHeight(const Font font) const {
    _loadFont(font);
    const uint16_t height = _spr.fontHeight();
    _spr.unloadFont();
    return height;
}

void TftDisplay::_loadFont(const Font font) const {
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

}  // namespace sensirion::upt::display
