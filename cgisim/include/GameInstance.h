#include"Event.h"
#include"Listener.h"
#include"base.h"
#include"Zone.h"


namespace cgisim{
    enum GamePhase{
        GameStart = 0,RollPhase,HandSwitch,RoundStart,Combat,RoundEnd, \
        DeathSwap, count
    };

    struct PlayerHistory{
        int player_id;
        int active_char;
        bool roundend = false;
        bool isplunge = true;
        bool died_in_round = false;
    };
    struct PlayerIntance{
        PlayerHistory history;
        Zptr Zones[int(ZoneID::Zone)];
        PlayerIntance(){
            Zones[ZoneID::CharZone] = nullptr;
            //TODO: for each element in Zones, use make_shared to initialize.
        }
    };

    struct GameHistory{
        int mover = -1;
        int round = 0;
        GamePhase phase = GamePhase::GameStart;
        int winner = -1;
    };
    
    struct GameInstance{
        GameHistory history;
        PlayerIntance player[2];
    };
};