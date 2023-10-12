#include"Event.h"
#include"Listener.h"
#include"base.h"
#include"Zone.h"
#include <iostream>

template<typename... T>
struct overload:T...{
    using T::operator()...;
};

template<typename... T>
overload(T...) -> overload<T...>;

namespace cgisim{
    namespace GamePhase{
    enum GamePhase{
        GameStart = 0,RoundStart,Combat,RoundEnd, count
    };}
    namespace ActionOption{
        enum ActionOption{
            Combat = 0, Reroll, SwitchHand, DeathSwap
        };
    };

    struct PlayerHistory{
        int player_id;
        int active_char = 0;
        int reroll_times = 1;
        bool roundend = false;
        bool isplunge = true;
        bool died_in_round = false;
        std::vector<std::string> discarded_cards;//爆牌
    };

    struct PlayerIntance{
        PlayerHistory history;
        Zptr Zones[int(ZoneID::Zone)];
        PlayerIntance(){
            Zones[ZoneID::CharZone] = nullptr;
            //TODO: for each entry in Zones, use make_shared to initialize.
        }
        Llist get_listeners();
    };

    struct GameHistory{
        int mover = -1;
        int round = 0;
        int winner = -1;
        GamePhase::GamePhase phase = GamePhase::GameStart;
        ActionOption::ActionOption action = ActionOption::Combat;
    };
    
    enum INTERRUPT{
        DEATH_SWAP, SWITCH_HAND, NO_DRAW, SET_DICE,  RECOVER, NOTHING/// no interrupt
    };
    struct GameConfig{
        bool nodraw = false;
        bool noroll = false;
    };
    struct GameInstance{
        GameHistory history;
        PlayerIntance player[2];


        GameConfig config;
        INTERRUPT signal = NOTHING;
        Elist q;//be careful when copy!
        Elist invoked;
        EventType ir_event;/// interrupt_event record, already executed.
        ///only valid when signal is not NOTHING.

        int maxeid = 1;

        void start(int active_chars[2]){
            //TODO: 设置出战角色, 发送一条GameStart事件, 然后checkout.
            this->player[0].history.active_char = active_chars[0];
            this->player[1].history.active_char = active_chars[1];
            this->issue(GameStart);
        }

        Elist execute(EventType event){
            auto valid = std::visit(
                [&](Eptr e){
                    if(e->eid == -1){return false;}
                    else return true;
                }, event
            );
            if(valid) return std::visit([&](auto&& e){
                return this->_execute(std::forward<decltype(e)>(e));
            }, event);
            else return Elist();
        }
        int getnewid(){
            return maxeid++;
        }
        Elist issue(EventType e){
            this->q.push_back(e);
        }
        /**
         * @brief checkout() will process all events in this->q. A standard workflow is 
         * 
         * for e in events:
         *    g.issue(e)
         * g.checkout(). That would invoke event system process. and this->q will be reset.
         * 
         * @return An interrupt signal indicates if checkout() exits normally. Default value is NOTHING.
         */
        INTERRUPT checkout(){
            if(this->signal!=INTERRUPT::NOTHING){
                if(this->signal == INTERRUPT::RECOVER){
                    this->q.insert(this->q.end(), this->invoked.rbegin(), this->invoked.rend());
                    this->signal = INTERRUPT::NOTHING;
                }
                else{
                    throw "now system is under interrupt!";
                }
            }
            auto deq = [](decltype(this->q) q){auto b = q.back();q.pop_back();return b;};
            while(not this->q.empty()){
                auto event = deq(this->q);
                auto eid = std::visit([](Eptr e){
                    return e->eid;
                }, event);
                if(eid == -1){
                    continue;
                }

                int first_player = std::visit([](Eptr e){
                    return e->player_id;
                }, event);
                int second = 1 - first_player;
                Llist listeners = this->player[first_player].get_listeners();
                auto temp = this->player[second].get_listeners();
                listeners.insert(listeners.end(), temp.begin(), temp.end());
                Elist invoked{};
                for(auto l:listeners){
                    auto ret = l->listen(event);
                    invoked.insert(invoked.end(), ret.begin(), ret.end());
                }
                auto ret = this->execute(event);
                
                invoked.insert(invoked.end(), ret.begin(), ret.end());
                bool is_over = std::visit(overload{
                    [](Eptr e){return false;},
                    [](Overptr e){return true;}
                }, event);
                if(!is_over){
                    this->q.push_back(EventType(std::make_shared<Over>(first_player, this->getnewid(), eid, event)));
                }
                /// @brief interrupt check.
                if(this->signal != INTERRUPT::NOTHING){
                    this->ir_event = event;//save ir_event.
                    this->invoked = invoked;
                    break;
                    /// codes above make such situation valid: some card raise an interrupt when card.play() be called.
                    /// that card only needs to modify this->signal. 
                }
                //reversely insert. because invoked is temporally ordered but this->q only supports pop_back.
                this->q.insert(this->q.end(), invoked.rbegin(), invoked.rend());
            }
            return this->signal;
        }

        /// @brief forward declarations below. They would be implemented in Cpp files.
        /// all events should be declared here.
        Elist _execute(Eptr e){
            #ifndef NDEBUG
            printf("You are executing an Event, which is not supposed to happen.");
            #endif
        
            return Elist();
        }
        Elist _execute(Overptr o){
            //here would be a dispatch in future.
            //see py-version gisim for details.
            #ifndef NDEBUG
            std::cerr << "Over with eid = " << o->eid  << " executed" << std::endl;
            #endif
            return Elist();
        }
        Elist _execute(GameStartptr gs){
            #ifndef NDEBUG
            std::cerr << "Game starts" << std::endl;
            #endif
            this->history.phase = GamePhase::GameStart;
            this->history.mover = 0;
            Elist events{};
            //TODO: Draw 5 cards
            this->history.action = ActionOption::SwitchHand;
            return events;
        }
    };
};