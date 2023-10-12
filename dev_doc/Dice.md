## 费用计算

这是一个看上去简单, 但其实非常麻烦的问题. 最关键的是, "消耗费用"(命名为DiceCost)这个事件该发生在什么时候.

从一般的观察上来说, DiceCost需要保证以下几个性质:

1. DiceCost的执行不会影响到依赖于DicePool的监听器. 即, 对于其他监听器而言, 他们看到的DicePool是未执行DiceCost前的. 这一点要求DiceCost的执行发生在非常靠后.
2. 当一个消耗DiceCost的动作执行之后, 其他事件应当看到的是执行Cost之后的DicePool, 比如说赌徒响应一个死亡事件.

一个很自然的想法是, 哪个Event可能会消耗Dice就内置一个DiceCost, 在执行该事件的时候同时执行DiceCost. 

## 费用预计算

现在费用预计算使用以下模式: 先传递一个DicePreCal事件, 内部有一个目标指令(dicecost字段为空), 然后收集该事件执行完毕后的结果(一个pattern), 再填入符合pattern的instance, 执行指令.

所有费用相关监听器都监听DicePreCal, 并在监听到该事件的时候就改变自身状态. 用python代码, 流程大概如下:

```py
for ins in instrucions:
    dpc = DicePreCal(ins)
    newg = g.clone()
    newg.issue(dpc)
    next_g = newg.step(ins.fill(dpc))##默认inplace
```

换句话说, DicePreCal会修改游戏状态, 因此需要先复制一份. 另外, 如果没有符合dpc.pattern的骰子, 就说明该行动不合法. 

> 这和之前的py项目中的方法不太一样. 这种方法的好处是监听器编码简单(只监听一种事件). 