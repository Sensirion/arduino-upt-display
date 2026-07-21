#ifndef I_DISPLAY_H
#define I_DISPLAY_H

#include <cstdint>

namespace sensirion::upt::display {

enum struct Orientation { portrait = 0, landscape = 1 };
enum struct Font { NORMAL, SMALL, MEDIUM, LARGE, XLARGE };

// Compatibility aliases for Orientation
constexpr auto portrait = Orientation::portrait;
constexpr auto landscape = Orientation::landscape;

class IDisplay {
  public:
    virtual ~IDisplay() = default;

    virtual void init(Orientation orientation) = 0;
    virtual void setRotation(Orientation orientation) = 0;

    [[nodiscard]] virtual uint16_t getWidth() const = 0;
    [[nodiscard]] virtual uint16_t getHeight() const = 0;
    [[nodiscard]] virtual Orientation getRotation() const = 0;

    virtual void fillScreen(uint16_t color) = 0;
    virtual void fillRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                               uint16_t r, uint16_t color) = 0;
    virtual void drawVLine(uint16_t x, uint16_t y, uint16_t h,
                               uint16_t color) = 0;
    virtual void drawHLine(uint16_t x, uint16_t y, uint16_t w,
                               uint16_t color) = 0;
    virtual void drawXBitmap(uint16_t x, uint16_t y, const uint8_t* bitmap,
                             uint16_t w, uint16_t h, uint16_t color) = 0;

    virtual void drawText(const char* text, uint16_t x, uint16_t y, Font font,
                          uint16_t color, uint16_t bgColor) = 0;
    virtual uint16_t getTextWidth(const char* text, Font font) const = 0;
    [[nodiscard]] virtual uint16_t getFontHeight(Font font) const = 0;
};

}  // namespace sensirion::upt::display

#endif /* I_DISPLAY_H */
