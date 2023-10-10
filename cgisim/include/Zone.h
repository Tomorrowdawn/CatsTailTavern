#pragma once
#include "Listener.h"
#include <vector>
#include <memory>
#include "assert.h"
namespace cgisim{
    using namespace std;

namespace ZoneID{
enum ZoneID{
    CharZone = 0,
    TeamBuffZone,
    SummonZone,
    SupportZone,
    HandZone,
    PileZone,
    DicePool,
    Zone
};
};

struct Zone{
    virtual vector<Lptr>& get_listeners() = 0;
    virtual void append(Lptr L) = 0;
    virtual Lptr pop(int index){
        auto list = this->get_listeners();
        assert(index < list.size() && index > 0);
        auto listener = list[index];
        list.erase(list.begin() + index);
        return listener;
    }
};

using Zptr = shared_ptr<Zone>;

struct PileZone:Zone{
    vector<Cardptr>& pile;
};

};