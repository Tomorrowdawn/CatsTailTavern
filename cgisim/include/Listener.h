#pragma once
#include "base.h"
#include "Event.h"
#include <string>
#include <vector>
#include <map>
namespace cgisim{

class GameInstance;

struct Listener{
    Address addr;
    GameInstance* g = nullptr;/// we store a pointer to global gameinstance.
    ///that might cause some trouble when clone a gameinstance. However, if we donot, it's quite difficult to write an operator() with two arguments(work with std::visit)
    /// should never free this pointer.
    int usage = 1;
    bool alive = true;


    virtual cgisim::Elist listen(EventType e) = 0;
    virtual std::string getName() = 0;//usually it should be declared as a static function.
    Elist operator()(Eptr e){
        return Elist();//do nothing.
    }
};

using Lptr = std::shared_ptr<Listener>;

enum CardTag{
    notag = 0, event, food, partner, weapon, artifact
};

struct Card:Listener{
    virtual Elist play();
    virtual Elist enterpile(){}
    virtual Elist draw();
    DicePattern dicecost;
    virtual std::vector<CardTag> getTags() = 0;
};

using Cardptr = std::shared_ptr<Card>;

enum Faction{
    nowhere = 0, Mondstadt, Liyue, Inazuma, Sumeru, Fontaine, Natlan, Snezhnaya, Hilichurl, Fatui, Abyss, Khaenri_ah, fac_count
};

enum WeaponType{
    noweapon = 0, sword, claymore, catalyst, bow, polearm
};

enum KitOption{
    na = 0, skill, burst, sp1, sp2, count
};



/**
 * @brief This struct contains *real-time* properties of a character.
 * which means value in this struct is variable.
 * 
 */
struct CharHistory{
    int hp = 10;
    int energy = 0;
    int KitUsePerRound[static_cast<int>(KitOption::count)];
    int KitUsePerGame[static_cast<int>(KitOption::count)];
    bool frozen = false;
    bool petrified = false;
    bool saturated = false;
    int aura = element::empty;
};


/**
 * @brief Data stored in this charprofile should be constants.
 * 
 */
struct CharProfile{
    int maxhp = 10;
    int maxenergy;
    int element;
    Faction faction = Faction::nowhere;
    WeaponType weapontype = WeaponType::noweapon;
    DicePattern kitpatterns[static_cast<int>(KitOption::count)];/// @brief an array stores dice pattern of each kit. indexed by KitOption. 
    KitOption kittypes[static_cast<int>(KitOption::count)];
};

struct Character:Listener{
    //CharProfile profile;
    //CharHistory history;//we split data separate from listeners to make copy easier.
    virtual const CharProfile& getprofile() = 0;
    virtual CharHistory& gethistory() = 0;

    std::vector<Lptr> buff;
    Lptr equips[3] = {nullptr, nullptr, nullptr};///weapon, artifact, talent.
    ///cloning a character would require cloning these pointers first


    std::vector<Lptr> listeners;/// a view which only works for listener architecture.
    //do not copy this.
    std::vector<Lptr>& get_listeners(){
        listeners.clear();
        listeners.insert(listeners.end(), equips, equips + 3*sizeof(Lptr));
        listeners.insert(listeners.end(), buff.begin(), buff.end());
        return listeners;
    }
    virtual Elist na();
    virtual Elist skill();
    virtual Elist burst();
    virtual Elist sp1(){return Elist();}
    virtual Elist sp2(){return Elist();}
    auto cast_kit(KitOption kit){
        switch(kit){
            case KitOption::na : return this->na();break;
            case KitOption::skill : return this->skill();break;
            case KitOption::burst : return this->burst();break;
            case KitOption::sp1 :return this->sp1();break;
            case KitOption::sp2 : return this->sp2();break;
            default: throw "Unknown Kit Type!";
        }
    }
};

using TokenCreator = Listener* (*)();
using CharCreator = Character* (*)();
using CardCreator = Card* (*)();


template <typename T>
struct Factory{
    inline static Factory& get(){
        static Factory instance;
        return instance;
    }
    template<typename subT>
    struct registrar{
        registrar(const std::string& name){
            if(Factory::get()._map.find(name) != Factory::get()._map.end()){
                throw "this name " + name + " is already used!";
            }
            Factory::get()._map.emplace(name, [](){return std::shared_ptr<T>(new subT{});});
        }
    };

    static std::shared_ptr<T> produce(const std::string& name){
        auto F = get();
        if(F._map.find(name) == F._map.end()){
            throw "this subclass name " + name + " havn't been registered!";
        }
        return F._map[name]();
    }
    private:
    std::map<std::string, std::shared_ptr<T>(*)() > _map;
};


/**
 * @brief These macros should be used like REGISTER_TOKEN(Paimon, Paimon).
 * You shouldn't quote *name* argument because it would be parsed as simple text by compiler, not an expression.
 * 
 */
#define REGISTER_TOKEN(T, name) static Factory<Listener>::registrar<T> reg_for_token_##name (#name);
#define REGISTER_CHAR(C, name) static Factory<Character>::registrar<C> reg_for_char_##name (#name);
#define REGISTER_CARD(C, name) static Factory<Card>::registrar<C> reg_for_card_##name (#name);

//这样, 如Paimon就可以在CardF和TokenF里保持同一个名字但不会引起冲突了(这也有助于反向查找, 以实现藏锋等卡牌).


};


/**
template <typename... Ts>
struct overload : Ts... {
    using Ts::operator()...;
};

using namespace cgisim;
GameInstance g;

std::visit([&](auto &&v) {
    Listenter l;
    l(std::forward<decltype(v)>(v),g );
}, v);

std::visit(event, L) ;

**/