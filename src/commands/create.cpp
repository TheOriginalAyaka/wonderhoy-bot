//
// Created by Ayaka on 9/30/25.
//

#include "command_handler.h"
#include "modules/create_sticker.h"
#include "data/characters.h"

namespace Commands {
    Command create = {
        .name = "create",
        .description = "Create a new sticker ✨",
        .options = {
            dpp::command_option(dpp::co_string, "text", "The text in the sticker", true),
            dpp::command_option(dpp::co_string, "character", "The character to use", true).set_auto_complete(true),
            dpp::command_option(dpp::co_integer, "horizontal", "Text horizontal offset", false),
            dpp::command_option(dpp::co_integer, "vertical", "Text vertical offset", false),
            dpp::command_option(dpp::co_number, "rotation", "Text rotation", false),
            dpp::command_option(dpp::co_integer, "size", "Text size", false)
        },
        .context_types = {dpp::itc_bot_dm, dpp::itc_guild, dpp::itc_private_channel},
        .handler = [](const dpp::slashcommand_t &event) {
            const auto text = std::get<std::string>(event.get_parameter("text"));
            const auto character_id = std::stoi(std::get<std::string>(event.get_parameter("character")));

            Options options ;

            const auto horizontal_param = event.get_parameter("horizontal");
            if (std::holds_alternative<int64_t>(horizontal_param)) {
                options.horizontal = static_cast<int>(std::get<int64_t>(horizontal_param));
            }

            const auto vertical_param = event.get_parameter("vertical");
            if (std::holds_alternative<int64_t>(vertical_param)) {
                options.vertical = static_cast<int>(std::get<int64_t>(vertical_param));
            }

            const auto rotation_param = event.get_parameter("rotation");
            if (std::holds_alternative<double>(rotation_param)) {
                options.rotation = static_cast<float>(std::get<double>(rotation_param));
            }

            const auto size_param = event.get_parameter("size");
            if (std::holds_alternative<int64_t>(size_param)) {
                options.font_size = static_cast<int>(std::get<int64_t>(size_param));
            }

            const auto res = create_sticker(text.c_str(), character_id, &options);

            dpp::message message;

            message.add_file("sticker.png", res);

            event.reply(message);
        }
    };

    REGISTER_COMMAND(create)
}
