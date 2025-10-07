//
// Created by Ayaka on 9/30/25.
//

#include "command_handler.h"
#include "data/characters.h"
#include "utils/parse_emoji.h"
#include "utils/parse_hex_color.h"

static std::string to_lower(std::string str) {
    std::ranges::transform(str, str.begin(),
                           [](const unsigned char c) { return std::tolower(c); });
    return str;
}

namespace Commands {
    CommandHandler &CommandHandler::get_instance() {
        static CommandHandler instance;
        return instance;
    }

    void CommandHandler::register_command(const Command &command) {
        this->commands[command.name] = command;
    }

    void CommandHandler::setup_handler(dpp::cluster &bot) {
        bot.on_slashcommand([this](const dpp::slashcommand_t &event) {
            handle_command(event);
        });

        bot.on_ready([&bot, this](const dpp::ready_t &) {
            if (dpp::run_once<struct register_bot_commands>()) {
                std::vector<dpp::slashcommand> slash_commands;

                for (const auto &[name, command]: this->commands) {
                    dpp::slashcommand cmd(name, command.description, bot.me.id);

                    for (const auto &option: command.options) {
                        cmd.add_option(option);
                    }

                    cmd.set_interaction_contexts(command.context_types);

                    slash_commands.push_back(cmd);
                }

                bot.global_bulk_command_create(slash_commands);

                bot.log(dpp::ll_info, "Registered " + std::to_string(slash_commands.size()) + " commands!");
            }
        });

        bot.on_autocomplete([&bot](const dpp::autocomplete_t &event) {
            for (const auto &option: event.options) {
                if (option.focused && option.name == "character") {
                    const auto name = std::get<std::string>(option.value);
                    auto user_input_lower = to_lower(name);

                    const auto &all_characters = get_all_characters();
                    int count = 0;
                    constexpr int MAX_CHOICES = 25;

                    dpp::interaction_response response(dpp::ir_autocomplete_reply);

                    for (const auto &character: all_characters) {
                        if (count >= MAX_CHOICES) break;

                        std::string name_lower = to_lower(character.name);

                        if (name.empty() || name_lower.find(user_input_lower) != std::string::npos) {
                            response.add_autocomplete_choice(
                                dpp::command_option_choice(character.name, std::to_string(character.id))
                            );
                            count++;
                        }
                    }

                    bot.interaction_response_create(event.command.id, event.command.token, response);
                }
            }
        });

        bot.on_select_click([](const dpp::select_click_t &event) {
            if (event.custom_id == "characters") {
                const auto selected = event.values[0];
                if (selected.empty()) return;
                const auto &previews = get_all_character_previews();
                const auto &selected_preview = get_character_preview(selected);
                if (!selected_preview) return;

                uint32_t color;
                (void) parse_hex_color(selected_preview->color.c_str(), &color);
                const dpp::embed embed = dpp::embed()
                        .set_title(selected_preview->name)
                        .set_image(selected_preview->img)
                        .set_color(color)
                        .set_footer(dpp::embed_footer().set_text("Made by Ayaka (@cuny.)"));

                dpp::component component = dpp::component()
                        .set_type(dpp::cot_selectmenu)
                        .set_placeholder("Select a character")
                        .set_id("characters");

                for (const auto &character: previews) {
                    if (character.name == selected_preview->name) continue;

                    const auto em = parse_emoji(character.emoji);

                    component.add_select_option(
                        dpp::select_option(character.name, character.id, character.name_jp).set_emoji(em->name, em->id));
                }

                dpp::message message;

                message.add_component(dpp::component().add_component(component));
                message.add_embed(embed);
                message.set_flags(dpp::m_ephemeral);

                event.reply(dpp::ir_update_message, message);
            }
        });
    }

    void CommandHandler::handle_command(const dpp::slashcommand_t &event) {
        const std::string command_name = event.command.get_command_name();

        const auto it = this->commands.find(command_name);
        if (it != this->commands.end()) {
            it->second.handler(event);
        }
    }
}
