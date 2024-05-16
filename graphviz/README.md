# dot

dot 接受 DOT 语言的输入。

此语言描述了三种主要对象类型：图形(graphs)、节点(nodes)和边(edges)。

主（最外层）图形可以是有向图或无方向图形

## 例子1

- 第 1 行提供图形名称和类型
- 后续的行创建节点、边或子图，并设置属性。所有这些对象的名称可以是 C 标识符、数字或引用的 C 字符串。引号保护标点符号和空白
- 当节点的名称首次出现在文件中时，将创建节点
- 当边运算符 -> 加入节点时，将创建边

```dot
digraph G {
    size = "4,4";                       # 图形大小设置为4x4（英寸）
    main [shape=box];                   # 节点或边的属性以方框控制
    main -> parse [weight=8];
    parse -> execute;
    main -> init [style=dotted];        # 边是虚线
    main -> cleanup;
    execute -> {make_string; printf};
    init -> make_string;
    edge [color=red];                   # 边是红线
    main -> prinf [style=bold, label= "100 times"];         # 标记边的 标签
    make_string [label = "make a \nstring"];
    node [shape=box,style=filled,color=".7 .3 1.0"];
    execute -> compare;
}
```

