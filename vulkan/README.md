# Vulkan

与Vulkan打交道，通常的步骤是创建一个intance去初始化Vulkan library。这个instance是您的应用程序与Vulkan库之间的连接桥梁,通常创建过程中，需要向驱动程序提供一些应用层的信息。

## 创建一个实例

Vulkan API使用vkInstance对象来存储所有每个应用的状态。应用程序必须在执行任何其他Vulkan操作之前创建一个Vulkan实例，基本的Vulkan架构看起来是这样的：

```
Application --> Loader --> Layer1 --> LayerN --> Driver
```

请注意，图中有一些层（Layer)，这些层也被加载器加载。层通常用于验证，通常是由驱动执行的错误检查。在Vulkan中，驱动程序比OpenGL等其他API要轻量得多，部分原因是它将功能验证委托给验证层。层是可选的，每次应用程序创建一个实例时都可以选择性地装载。

## Vulkan 物理设备与队列

Vulkan 物理设备与队列，通过VkInstance初始化Vulkan后，我们需要在系统中查找并选择一个支持我们所需功能的显卡。实际上，我们可以选择任意数量的显卡并同时使用他们，但在本小节中，我们简单的设定选择规则，即将查找到的第一个图形卡作为我们适合的物理设备。

通过VkInstance初始化Vulkan后，我们需要在系统中查找并选择一个支持我们所需功能的显卡。实际上，我们可以选择任意数量的显卡并同时使用他们，但在本小节中，我们简单的设定选择规则，即将查找到的第一个图形卡作为我们适合的物理设备。

我们添加函数pickPhysicalDevice并在initVulkan函数中调用。

最终我们选择的图形显卡存储在类成员VkPhysicalDevice句柄中。当VkInstance销毁时，这个对象将会被隐式销毁，所以我们并不需要在cleanup函数中做任何操作。


## 队列族

几乎所有的Vulkan操作，从绘图到上传纹理，都需要将命令提交到队列中。有不同类型的队列来源于不同的队列簇，每个队列簇只允许部分commands。例如，可以有一个队列簇，只允许处理计算commands或者只允许内存传输commands:

我们需要检测设备中支持的队列簇，其中哪一个队列簇支持我们想要的commands。为此我们添加一个新的函数findQueueFamilies来查找我们需要的队列簇。现在我们只会寻找一个支持图形commands队列簇，但是我们可以在稍后的小节中扩展更多的内容。

```
VkInstance --> VkPhysicalDevice --> VkQueueFamilyProperties
```