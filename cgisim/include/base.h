#pragma once

namespace cgisim{
    enum Faction{
    nowhere = 0, Mondstadt, Liyue, Inazuma, Sumeru, Fontaine, Natlan, Snezhnaya, Hilichurl, Fatui, Abyss, Khaenri_ah, fac_count
};

enum WeaponType{
    noweapon = 0, sword, claymore, catalyst, bow, polearm
};

enum KitOption{
    na = 0, skill, burst, sp1, sp2, count
};


enum CardTag{
    notag = 0, event, food, partner, weapon, artifact
};


namespace element{

    //aura
    const int pyro = 0;
    const int cryo = 1;
    const int electro = 2;
    const int hydro = 3;
    const int dendro = 4;
    const int anemo = 5;
    const int geo = 6;
    const int empty = 7;
    const int cryodendro = 8;
    const int died = 9;

    /**
     * @brief dice type
     * 
     */
    const int omni = 7;
    const int black = 8;
    const int white = 9;


    /**
     * @brief damage type
     * 
     */
    const int physical = 7;
    const int pierce = 8;
};

struct CharAddr{
    int subzone, offset;
};

struct Address{
    int player_id = 0, zone_id = 0, index = 0;
    CharAddr* ca = nullptr;
};


/**
 * @brief actual dices
 * 
 * dice[element] shows how many dices are selected whose element type is element.
 * 
 */
struct DiceInstance{
    int dice[8] = {};
};


/**
 * @brief dice pattern, such as 1 hydro die and 2 unaligned.
 * 
 * pattern: an array mapping element::ele to number, which is zero by default.
 * you can access actual number by pattern[element::hydro] for example.
 * 
 */
struct DicePattern{
    int pattern[10] = {};
};
};