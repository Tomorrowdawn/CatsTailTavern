#pragma once
#include "base.h"
#include "Event.h"
#include <string>
#include <vector>
namespace cgisim{

class GameInstance;

struct Listener{
    Address addr;
    GameInstance* g = nullptr;/// we store a pointer to global gameinstance.
    ///that might cause some trouble when clone a gameinstance. However, if we donot, it's quite difficult to write an operator() with two arguments(work with std::visit)
    /// should never free this pointer.
    int usage = 1;
    bool alive = true;


    virtual cgisim::Elist listen(EventType e) = default;
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

enum Fraction{
    nowhere = 0, Mondstadt, Liyue, Sumeru, Fontaine
};

enum WeaponType{
    noweapon = 0, sword, claymore, catalyst, bow, polearm
};

enum KitOption{
    na = 0, skill, burst, sp1, sp2, count
};


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

struct CharProfile{
    int maxhp = 10;
    int maxenergy;
    int element;
    Fraction fraction = Fraction::nowhere;
    WeaponType weapontype = WeaponType::noweapon;
    DicePattern kitpatterns[static_cast<int>(KitOption::count)];/// @brief an array stores dice pattern of each kit. indexed by KitOption. 
    KitOption kittypes[static_cast<int>(KitOption::count)];
};

struct Character:Listener{
    CharProfile profile;
    CharHistory history;//we split data separate from listeners to make copy easier.

    std::vector<Lptr> buff;
    Lptr equips[3] = {nullptr, nullptr, nullptr};///weapon, artifact, talent.
    ///cloning a character would require cloning these pointers first


    std::vector<Lptr> listeners;/// a view which only works for listener architecture.
    //do not need to copy this.
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
};
};
//TODO: 编写自动注册工厂. 这样的工厂应当有三个(CardFactory, CharacterFactory, TokenFactory).
///Token泛指卡牌打出后的衍生物.
//这样, 如Paimon就可以在CardF和TokenF里保持同一个名字但不会引起冲突了(这也有助于反向查找, 以实现藏锋等卡牌).