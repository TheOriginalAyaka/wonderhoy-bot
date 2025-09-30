//
// Created by Ayaka on 9/30/25.
//

#ifndef WONDERHOYCXX_COMMAND_HANDLER_H
#define WONDERHOYCXX_COMMAND_HANDLER_H

#include <dpp/dpp.h>
#include <functional>
#include <unordered_map>
#include <string>

namespace Commands {
    using CommandFunction = std::function<void(const dpp::slashcommand_t &)>;

    struct Command {
        std::string name;
        std::string description;
        std::vector<dpp::command_option> options;
        std::vector<dpp::interaction_context_type> context_types;
        CommandFunction handler;
    };

    class CommandHandler {
    public:
        static CommandHandler &get_instance();

        void register_command(const Command &command);

        void setup_handler(dpp::cluster &bot);

    private:
        CommandHandler() = default;

        void handle_command(const dpp::slashcommand_t &event);

        std::unordered_map<std::string, Command> commands;
    };

#define REGISTER_COMMAND(cmd) \
static bool cmd##_registered = []() { \
Commands::CommandHandler::get_instance().register_command(cmd); \
return true; \
}();
}

#endif //WONDERHOYCXX_COMMAND_HANDLER_H
