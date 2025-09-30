//
// Created by Ayaka on 9/30/25.
//

#include "bot.h"
#include "commands/command_handler.h"

Bot::Bot(const std::string &token) {
    this->cluster = std::make_unique<dpp::cluster>(token);
    this->cluster->on_log(dpp::utility::cout_logger());
}

void Bot::setup_commands() const {
    Commands::CommandHandler::get_instance().setup_handler(*this->cluster);
}

void Bot::run() const {
    this->cluster->start(dpp::st_wait);
}
