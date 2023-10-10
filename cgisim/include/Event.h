#pragma once
#include <memory>
#include <vector>
#include <variant>
namespace cgisim{
    
    /// @brief forward decl area.
    /// in this area, all event types would be declared
    /// and we will define several *using* phrase to wrap structs.
    struct Event;
    struct Over;

    using Eptr = std::shared_ptr<Event>;
    using Overptr = std::shared_ptr<Over>;
    using EventType = std::variant<Eptr, Overptr>;

    struct Event{
        int player_id, eid, source_id;
    };
    struct Over:Event{
        EventType overed;
    };
    using Elist = std::vector<EventType>;

};
