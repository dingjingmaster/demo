## QGraphicsView

QGraphicsView在可滚动的视口中可视化QGraphicsScene的内容。要创建带有几何项的场景，请参阅QGraphicsScene的文档。QGraphicsView是图形视图框架的一部分。

为了可视化一个场景，你首先构造一个QGraphicsView对象，将你想要可视化的场景的地址传递给QGraphicsView的构造函数。或者，您可以调用setScene()在稍后设置场景。调用show()后，视图将默认滚动到场景的中心，并显示此时可见的任何项。