//
// Created by Ayaka on 9/30/25.
//

#ifndef WONDERHOYCXX_CREATE_STICKER_H
#define WONDERHOYCXX_CREATE_STICKER_H

#include <string>
#include <optional>

struct Options {
    std::optional<int> font_size;
    std::optional<int> horizontal;
    std::optional<int> vertical;
    std::optional<float> rotation;
};

std::string create_sticker(const char *text, int character_id, Options *options);

#endif //WONDERHOYCXX_CREATE_STICKER_H
