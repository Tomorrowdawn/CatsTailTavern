#pragma once

#include "base.h"
#include <variant>
#include <vector>
#include "Listener.h"

namespace cgisim{
    struct Instruction{
        int player_id;
        DicePattern dice_pattern;
        DicePattern real_dice_pattern;
        DiceInstance dice_instance;
    };
    struct EndRound:Instruction{};
    struct Switch:Instruction{
        int direction;//-1 or 1
        //dice_pattern 1 black
        Switch(){
            direction = 1;
            dice_pattern = DicePattern();
            dice_pattern.pattern[element::black] = 1;
        }
    };
    struct UseKit:Instruction{
        KitOption kit = KitOption::skill;
    };
    struct PlayCard:Instruction{
        int hand_offset;
        std::vector<Address> targets;
    };
};