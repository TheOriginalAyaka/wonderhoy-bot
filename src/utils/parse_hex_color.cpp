//
// Created by Ayaka on 10/7/25.
//

#include "parse_hex_color.h"
#include <cstdlib>

bool parse_hex_color(const char *hex, uint32_t *out) {
    if (!hex || hex[0] != '#')
        return false;

    char *end;
    const unsigned long value = std::strtoul(hex + 1, &end, 16);

    if (end - (hex + 1) != 6)
        return false;

    const unsigned int r = value >> 16 & 0xFF;
    const unsigned int g = value >> 8 & 0xFF;
    const unsigned int b = value & 0xFF;

    *out = r << 16 | g << 8 | b;
    return true;
}