//
// Created by Ayaka on 10/7/25.
//

#ifndef WONDERHOYCXX_PARSE_EMOJI_H
#define WONDERHOYCXX_PARSE_EMOJI_H

#include <dpp/dpp.h>

struct EmojiInfo {
    std::string name;
    dpp::snowflake id;
};

std::optional<EmojiInfo> parse_emoji(const std::string& emoji_str);

#endif //WONDERHOYCXX_PARSE_EMOJI_H