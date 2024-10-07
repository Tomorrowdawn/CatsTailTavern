## 核心类

整个Event System由以下几部分组成:

```cpp

enum ETYPE;

class Event{
    public:
        int eid;int source_id;int player_id;
        virtual ETYPE getType(void);
    virtual vector<shared_ptr<Event> > handle(GameInstance* g){
        return g->handle(this);
    }
};

using Eptr = std::variant<allevent here>


class GameInstance{
 public:
    //other code
    vector<Eptr> execute(Eptr e){
        return e->handle(this);
    }

    vector<Eptr> handle(Event* e){
        //do something
    }
    vector<Eptr> handle(EventSubclass* e){
        //do something.
    }
};
```

### 