//
// Created by Ayaka on 9/30/25.
//

#ifndef WONDERHOYCXX_BOT_H
#define WONDERHOYCXX_BOT_H

#include <dpp/dpp.h>

class Bot {
public:
    explicit Bot(const std::string &token);

    void setup_commands() const;

    void run() const;

private:
    std::unique_ptr<dpp::cluster> cluster;
};

#endif //WONDERHOYCXX_BOT_H
