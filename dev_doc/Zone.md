## 基类Zone

Zone是一个抽象类. 每个类都需要提供以下方法:

```cpp
vector<Listener*>& Zone::get_listeners()
```

它将按监听顺序返回一个监听器指针列表的引用. 注意, 所有监听器指针都需要是`new`创建的(因为我们要用`delete`删除它).

简单来说, Zone是一个Listener容器. 它的作用就是管理Listener. 具体某个的Zone中还包含了执行游戏逻辑所必需的其他参数.

```cpp
void Zone::Zone()
```

通常, Zone中包含一个Listener*指针列表. 拷贝这个列表时不仅需要复制指针, 还需要拷贝内部成员. 没有特殊说明的情况下, 我们均使用`shared_ptr`管理内存. 因为游戏中可能会复制大量局面, 对指针管理不当会造成严重的内存泄漏.

```cpp
void Zone::~Zone();
```

Zone提供一个默认的析构. 这个析构会调用`get_listeners`并逐个释放其中的指针.

```cpp
virtual AppendZone::append(Listener* L)
```

此方法用于描述增加一个区域成员时的行为. 各类需要实现该接口.

```cpp
AppendZone::pop(int index)
```

将index处的Listener弹出. 此方法有一个默认实现. 它将get_listeners()返回的vector中的对应index的监听器弹出, 之后对所有其他监听器重新修改位置Address.



## Character(CharZone)

### 成员

CharZone中记录了如下信息:

1. 所属player_id
2. 角色列表char_list
3. 出战角色下标active

char_list是一个`vector<Charcter*>`.

### 函数

CharZone内部有一些工具函数.

```cpp
vector<T> CharZone::collect(vector<char_index>, collect_func)
```

对char_index指定的Character施加collect_func, 将其返回值打包成一个vector. 这是一个模板函数, T被collect_func的返回值确定. 两者必须相同. collect_func的输入为Character.

```cpp
void CharZone::scatter(vector<char_index>, vector<T>(*scatter_func)(char_index, Character))
```

scatter_func将会修改前一个容器中指明的所有Character. 返回值为空. scatter接收(index, character). index是前一个vector中包含的index(而不是它的下标).

我们可以很轻松地使用collect来收集一个附着列表

```cpp
CZ = CharacterZone();
///....
auto aura_list = CZ.collect([0,1,2], [](Listener::Character c){
    return c.aura;
});
```

在反应结束后, 我们使用

```cpp

/// ..get result_aura;
CZ.scatter([0,1,2], [result_aura](int index,Character c){
    c.aura = result_aura[index];
});
```

```cpp
Listener* CharZone::get(CZAddr a);
```

该函数返回一个Listener指针, 使用CharZone特有的寻址系统.

```cpp
int CharZone::nextchar(int c);
int CharZone::prechar(int c);
```

这两个方法用于支持七圣召唤中的"下一个"和"前一个"定位方法. 他们会自动跳过已死亡角色. 输入为角色下标, 返回值为下(上)一个角色下标.

当仅剩一个角色时, 返回值与输入完全一致.
