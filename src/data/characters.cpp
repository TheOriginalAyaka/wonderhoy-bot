//
// Created by Ayaka on 9/30/25.
//

#include "characters.h"
#include <dpp/json.h>
#include <mutex>

using json = nlohmann::json;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Character, id, name, character, img, color)

static std::vector<Character> characters;
static std::once_flag initialized;

static void initialize_characters() {
    std::call_once(initialized, []() {
        const json j = json::parse(characters_json());
        characters = j.get<std::vector<Character> >();
    });
}

const std::vector<Character> &get_all_characters() {
    initialize_characters();
    return characters;
}

Character *get_character(const int id) {
    initialize_characters();

    for (auto &character: characters) {
        if (character.id == id) {
            return &character;
        }
    }
    return nullptr; // Not found
}
