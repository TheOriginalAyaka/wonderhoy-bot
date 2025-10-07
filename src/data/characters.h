//
// Created by Ayaka on 9/30/25.
//

#ifndef WONDERHOYCXX_CHARACTERS_H
#define WONDERHOYCXX_CHARACTERS_H
#include <string>
#include <vector>


struct Character {
    int id;
    std::string name;
    std::string character;
    std::string img;
    std::string color;
};

struct CharacterPreview {
    std::string id;
    std::string name;
    std::string name_jp;
    std::string color;
    std::string img;
    std::string emoji;
};

const std::vector<Character> &get_all_characters();
const std::vector<CharacterPreview> &get_all_character_previews();

Character *get_character(int id);
CharacterPreview *get_character_preview(const std::string &id);

static void initialize_characters();
static void initialize_character_previews();



#endif //WONDERHOYCXX_CHARACTERS_H
