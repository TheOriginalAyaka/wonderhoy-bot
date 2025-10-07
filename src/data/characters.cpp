//
// Created by Ayaka on 9/30/25.
//

#include "characters.h"
#include <dpp/json.h>
#include <mutex>
#include <fstream>

using json = nlohmann::json;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Character, id, name, character, img, color)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CharacterPreview, id, name, name_jp, color, img, emoji)

static std::vector<Character> characters;
static std::vector<CharacterPreview> character_previews;

static std::once_flag characters_initialized;
static std::once_flag character_previews_initialized;

static void initialize_characters() {
    std::call_once(characters_initialized, []() {
        std::ifstream file("../assets/characters.json");

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open characters.json");
        }

        try {
            json j;
            file >> j;

            if (!j.is_array()) {
                throw std::runtime_error("JSON file must contain an array");
            }

            characters = j.get<std::vector<Character> >();

            if (characters.empty()) {
                throw std::runtime_error("No characters loaded from JSON");
            }
        } catch (const json::parse_error &e) {
            throw std::runtime_error(std::string("JSON parse error: ") + e.what());
        } catch (const json::type_error &e) {
            throw std::runtime_error(std::string("JSON type error: ") + e.what());
        }
    });
}

static void initialize_character_previews() {
    std::call_once(character_previews_initialized, []() {
        std::ifstream file("../assets/characters_prev.json");

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open characters_prev.json");
        }

        try {
            json j;
            file >> j;

            if (!j.is_array()) {
                throw std::runtime_error("JSON file must contain an array");
            }

            character_previews = j.get<std::vector<CharacterPreview> >();

            if (character_previews.empty()) {
                throw std::runtime_error("No character previews loaded from JSON");
            }
        } catch (const json::parse_error &e) {
            throw std::runtime_error(std::string("JSON parse error: ") + e.what());
        } catch (const json::type_error &e) {
            throw std::runtime_error(std::string("JSON type error: ") + e.what());
        }
    });
}

const std::vector<Character> &get_all_characters() {
    initialize_characters();
    return characters;
}

const std::vector<CharacterPreview> &get_all_character_previews() {
    initialize_character_previews();
    return character_previews;
}


Character *get_character(const int id) {
    initialize_characters();
    for (auto &character: characters) {
        if (character.id == id) {
            return &character;
        }
    }
    return nullptr;
}

CharacterPreview *get_character_preview(const std::string &id) {
    initialize_character_previews();
    for (auto &character: character_previews) {
        if (character.id == id) {
            return &character;
        }
    }
    return nullptr;
}
