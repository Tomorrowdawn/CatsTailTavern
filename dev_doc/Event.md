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

在框架中, 我们使用`shared_ptr`管理. 但是, `handle`中, 因为双分派语法的关系, 我们使用裸指针. 注意, 这里没有新开辟任何内存, 因此它是安全的.

`GI::handle`中可能会创建一些事件, 它们使用shared_ptr管理, 并通过一个vector容器传回.

由于双分派语法, 在`Event.h`中需要前向声明`GameInstance`, 同时实现得放到cpp文件夹下.

