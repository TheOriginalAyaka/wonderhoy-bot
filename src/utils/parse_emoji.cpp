//
// Created by Ayaka on 10/7/25.
//

#include "parse_emoji.h"

std::optional<EmojiInfo> parse_emoji(const std::string &emoji_str) {
    if (emoji_str.size() < 5 || emoji_str[0] != '<' || emoji_str.back() != '>') {
        return std::nullopt;
    }

    size_t start = 0;
    if (emoji_str[1] == 'a' && emoji_str[2] == ':') {
        start = 3;
    } else if (emoji_str[1] == ':') {
        start = 2;
    } else {
        return std::nullopt;
    }

    size_t colon_pos = emoji_str.find(':', start);
    if (colon_pos == std::string::npos) {
        return std::nullopt;
    }

    EmojiInfo info;
    info.name = emoji_str.substr(start, colon_pos - start);
    info.id = emoji_str.substr(colon_pos + 1, emoji_str.size() - colon_pos - 2);

    if (info.id.empty() || info.name.empty()) {
        return std::nullopt;
    }

    return info;
}
