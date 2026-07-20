#ifndef ARDUINO_UPT_DISPLAY_VECTOR2_H
#define ARDUINO_UPT_DISPLAY_VECTOR2_H

struct Vector2 {
    int16_t x = 0;
    int16_t y = 0;
    Vector2() = default;
    Vector2(const int16_t& x, const int16_t& y) : x(x), y(y) {};
};

#endif  // ARDUINO_UPT_DISPLAY_VECTOR2_H
