#pragma once
#include <memory>
#include <vector>
#include <variant>
#include "base.h"
namespace cgisim{

    namespace Instruction{
    
        struct Instruction{
            int player_id;
            DicePattern origin_dp;
            DicePattern real_dp;
            DiceInstance dicecost;
        };
        struct EndRound:Instruction{

        };
        struct Switch:Instruction{
            int direction = 1;
        };
        struct UseKit:Instruction{
            KitOption kit = KitOption::skill;
        };
        struct PlayCard:Instruction{
            int offset = 0;
            Address targets[4];
        };
        struct Tuning:Instruction{
            int offset = 0;/**< which card should be used for tuning.*/
            int dice_element;
        };

        //TODO: SwitchHand & Reroll. 
    };


    /// @brief forward decl area.
    /// in this area, all event types would be declared
    /// and we will define several *using* phrase to wrap structs.
    struct Event;
    struct Over;
    struct GameStart;
    struct Death;

    using Eptr = std::shared_ptr<Event>;
    using Overptr = std::shared_ptr<Over>;
    using GameStartptr = std::shared_ptr<GameStart>;
    using Deathptr = std::shared_ptr<Death>;

    using EventType = std::variant<Eptr, Overptr, GameStartptr, Deathptr>;

    struct Event{
        int player_id, eid, source_id;
    };

    struct Over:Event{
        EventType overed;
    };
    struct GameStart:Event{};
    struct Death:Event{
        /// @brief who dies.
        Address char_loc;
    };
    using Elist = std::vector<EventType>;

};
