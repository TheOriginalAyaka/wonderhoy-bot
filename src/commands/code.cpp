//
// Created by Ayaka on 10/7/25.
//

#include "command_handler.h"

namespace Commands {
    Command code = {
        .name = "code",
        .description = "Get link to the source code",
        .options = {},
        .context_types = {dpp::itc_bot_dm, dpp::itc_guild, dpp::itc_private_channel},
        .handler = [](const dpp::slashcommand_t &event) {
            event.reply(
                dpp::message(
                    "The source code of the bot is available on GitHub: https://github.com/TheOriginalAyaka/wonderhoy-bot")
                .set_flags(dpp::m_ephemeral));
        }
    };

    REGISTER_COMMAND(code)
}
