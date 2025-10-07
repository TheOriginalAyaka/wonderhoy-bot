//
// Created by Ayaka on 10/7/25.
//

#include "command_handler.h"

namespace Commands {
    Command support = {
        .name = "support",
        .description = "Get the support server invite",
        .options = {},
        .context_types = {dpp::itc_bot_dm, dpp::itc_guild, dpp::itc_private_channel},
        .handler = [](const dpp::slashcommand_t &event) {
            event.reply(
                dpp::message("Join our support server for more help!\nhttps://discord.gg/PAxxFUsNdc")
                .set_flags(dpp::m_ephemeral));
        }
    };

    REGISTER_COMMAND(support)
}
