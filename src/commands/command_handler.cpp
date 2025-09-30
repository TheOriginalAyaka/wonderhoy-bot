//
// Created by Ayaka on 9/30/25.
//

#include "command_handler.h"
#include "data/characters.h"

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
    }

    void CommandHandler::handle_command(const dpp::slashcommand_t &event) {
        const std::string command_name = event.command.get_command_name();

        const auto it = this->commands.find(command_name);
        if (it != this->commands.end()) {
            it->second.handler(event);
        }
    }
}
