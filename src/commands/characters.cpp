//
// Created by Ayaka on 10/7/25.
//

#include "command_handler.h"
#include "data/characters.h"
#include "utils/parse_emoji.h"
#include "utils/parse_hex_color.h"

namespace Commands {
    Command characters = {
        .name = "characters",
        .description = "Display all available characters",
        .options = {},
        .context_types = {dpp::itc_bot_dm, dpp::itc_guild, dpp::itc_private_channel},
        .handler = [](const dpp::slashcommand_t &event) {
            const auto &previews = get_all_character_previews();

            uint32_t color;
            (void) parse_hex_color(previews[0].color.c_str(), &color);
            const dpp::embed embed = dpp::embed()
                    .set_title(previews[0].name)
                    .set_image(previews[0].img)
                    .set_color(color)
                    .set_footer(dpp::embed_footer().set_text("Made by Ayaka (@cuny.)"));

            dpp::component component = dpp::component()
                    .set_type(dpp::cot_selectmenu)
                    .set_placeholder("Select a character")
                    .set_id("characters");

            for (const auto &character: previews) {
                if (character.name == previews[0].name) continue;

                const auto em = parse_emoji(character.emoji);

                component.add_select_option(
                    dpp::select_option(character.name, character.id, character.name_jp).set_emoji(em->name, em->id));
            }

            dpp::message message;

            message.add_component(dpp::component().add_component(component));
            message.add_embed(embed);
            message.set_flags(dpp::m_ephemeral);

            event.reply(message);
        }
    };

    REGISTER_COMMAND(characters)
}
