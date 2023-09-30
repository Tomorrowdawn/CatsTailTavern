## 绑定范围

我们只将GameInstance向Python公开, 并且这些数据都将是只读的. 必须使用绑定的接口来修改数据(我们不推荐任何除了step以外的修改方式, 但开放部分修改接口有利于AI做数据增强). 我们也将在Python端包装一些操作来方便接口调用.

> 由于Pybind绑定原生array比较麻烦, 我们要尽量使用STL容器(std::array). 尽管这里最常见的容器是vector.

## 绑定细节

大体上, Py端的GameInstance只包含Data Manage部分. 换句话说, 它基本上是一个复杂的纯数据结构(而没有什么成员函数).

不过, 绑定的顺序比较重要. 我们得先将基本类绑定完毕后才能绑定GameInstance. 

宗旨: 游戏状态变量必定绑定(AI必需), 其他能省则省. 

### Player

绑定History和一个方法, zones_()

zones_()方法会避免绑定Zone, 而只调用它们的`get_listeners()`方法. 这个方法在python端会被包装成易于调用的格式. 

Listener的绑定如下

### Pure Listener

只绑定name和usage两个变量. 

### Character

绑定武器圣遗物天赋和一个Listener列表. 内部Listener绑定同上. 此外, Character还会绑定hp, energy等常量. 

### DicePool

这个区域比较特殊. 它会被原样传递. 在py端, 它就是一条向量. 

### GameInstance

绑定GameInstance时将绑定一个方法`players_()`. 这个方法将在python端被包装:

```py
class GI:
    @property
    def p1:
        return self.players_()[0]
    @property
    def p2:
        pass
```

