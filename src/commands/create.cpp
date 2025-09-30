//
// Created by Ayaka on 9/30/25.
//

#include "command_handler.h"
#include "modules/create_sticker.h"
#include "data/characters.h"

namespace Commands {
    Command create = {
        .name = "create",
        .description = "create a sticker",
        .options = {
            dpp::command_option(dpp::co_string, "text", "the text", true),
            dpp::command_option(dpp::co_string, "character", "the character", true).set_auto_complete(true)
        },
        .context_types = {dpp::itc_bot_dm, dpp::itc_guild, dpp::itc_private_channel},
        .handler = [](const dpp::slashcommand_t &event) {
            const auto text = std::get<std::string>(event.get_parameter("text"));
            const auto character_id = std::stoi(std::get<std::string>(event.get_parameter("character")));

            dpp::message message;

            Options options = {};
            const auto res = create_sticker(text.c_str(), character_id, &options);

            message.add_file("sticker.png", res);

            event.reply(message);
        }
    };

    REGISTER_COMMAND(create)
}
