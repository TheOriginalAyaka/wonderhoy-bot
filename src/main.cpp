//
// Created by Ayaka on 9/30/25.
//

#include "bot.h"

int main() {
    const char* token = std::getenv("DISCORD_TOKEN");
    if (!token) {
        std::cerr << "DISCORD_TOKEN environment variable not set" << std::endl;
        return 1;
    }

    const Bot bot(token);
    bot.setup_commands();
    bot.run();

    return 0;
}