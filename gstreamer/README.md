# GStreamer

> GStreamer 是一个非常强大和通用的框架，用于创建流媒体模块

## GStreamer 相关功能

- GNonLin 插件：1. 对各种格式进行编解码；2. 选择流片段，并编辑。所有这些模块提供流级别的访问(编辑困难，需要专业知识)。
- GStreamer Editing Services(简称GES)，通过一系列类来简化编辑
  ![Layer Track](img/layer_track_overview.png)

## 相关类介绍

- Initialization - GStreamer 编辑服务初始化函数
- GESTimeline - 多媒体时间线
- GESLayer - 
- GESClip - 复制一个 GESLayer 的元素的基类，且他们子元素保持相同的时间
- GESPipeline - 方便的 GstPipeline 编辑
- GESProject - 用于管理项目的 GESAsset
- GESTrack - GESTimeline 的输出源
- GESAsset - 表示 GStreamer 编辑服务中可用的资源
- GESEffect - 在 GESSourceClip 或 GESLayer 的流中添加从解析启动央视的pin描述生成的效果
- GESExtractable Interface - 可以从 GESAssert 中提取得到对象的接口
- GESGroup - GESContainers 的集合
- GESMetaContainer Interface - 存储元素据的接口
- GESFormatter - 时间轴保存和加载
- GESTrackElement - GESTrack 元素的基类
- GESErrors - 错误信息
- GES Types - GStreamer Editing 服务的数据类型
- GES Enumerations - Gstreamer 编辑服务的各种枚举
- GES utilities - 常用方法
- ---
- GESAudioTransition - 实现音频交叉淡入过度
- GESBaseEffectClip - GESLayer 的特效
- GESBaseEffect - 在GESSourceClip或GESLayer中添加一个效果到流
- GESBaseTeansitionClip - ...
- GESOperationClip - 用于GESLayer中的操作的基类
- GESOperation -
- GESOverlayClip - 
- GESSourceClip -
- GESSource -
- GESTextOverlayClip -
- GESTransition -
- GESVideoTransition -

# GStreamer OpenGL Library

> pkg-config --cflags -libs gstreamer-gl-1.0

## 相关 API

- GstGLAPI - OpenGL API 特定功能，提供一些帮助类
- GstGLBaseFilter - 用于转换 OpenGL 资源的 GstBaseTransform 子类
- GstGLBaseMemory - GL 缓冲区的内存子类
- GstGLBaseSrc - GstPushSrc 子类，用于在管道中注入 OpenGL 资源
- GstGLBuffer - GL 缓冲区的内存子类
- GstGLBufferPool - GstGL BaseMemory 对象的缓存池
- GstGLColorConvert - 在视频颜色空间和格式之间转换
- GstGLContext - OpenGL上下文抽象
- GstGLDisplay - 窗口系统显示连接抽象
- GstGLFeature - OpenGL 中功能检查
- GstGLFilter - 处理 RGBA 纹理的 GstBaseTransform 子类
- GstGLFormat - 用于处理 OpenGL 格式的工具
- GstGLFramebuffer - OpenGL framebuffer 抽象
- GstGLMemory - GL 纹理的内存子类
- GstGLMemoryPBO - GL 纹理的内存子类
- GstGLOverlayCompositor - 使用 OpenGL 组合多个 overlays
- GstGLQuery - openGL查询抽象
- GstGLRenderBuffer - GL 渲染缓冲区对象的内存子类
- GstGLShader - 对象表示一个 OpenGL 着色程序
- GstGLSL - 用于处理 OpenGL 着色器的助手
- GstGLSLStage - 对象处理 OpenGL 着色器阶段
- GstGLSyncMeta - 同步原语
- GstGLUpload - 一个上传 GL 纹理的对象
- GstGLUtils - OpenGL的一些杂项实用程序
- GstGLViewConvert - 转换之间的立体/多试图视频格式
- GstGLWindow - window/surface 抽象

# GStreamer OpenGL Library - EGL

> pkg-config --cflags --libs gstreamer-gl-egl-1.0

## 相关 API

- GstEGL - EGL 帮助类
- GstEGLImage - EGLImage 抽象类
- GstEGLDiaplayEGL - EGL 显示 连接
- GstEGLDisplayEGLDevice - EGL EGLDeviceEXT 类
- GstGLMemoryEGL - EGLImage 的内存子类


# GStreamer OpenGL Library - Wayland

> pkg-config --cflags --libs gstreamer-gl-wayland-1.0

## 相关 API

- GstGLDisplayWayland - Wayland 显示连接

# GStreamer OpenGL Library - X11

> pkg-config --cflags --libs gstreamer-gl-x11-1.0

## 相关 API

- GStGLDisplayX11 - X11 显示连接

# GStreamer Plugin-ins splitup

> GStreamer 插件拆分

# GStewamer RTSP 服务

## 相关 API

- GstRTSPAddressPool - 它是一个维护网络地址集合的对象，用于分配服务器端口和服务器组播地址，但也可用于保留客户端提供的目的地址
- GstRTSPAuth - 负责检查当前用户是否被允许执行请求的操作。默认实现有一些合理的检查，但子类可以实现自定义安全策略
- GstRTSPClient - 只要TCP连接打开，客户端对象就会处理与客户端的连接。 当一个新的连接被接受时候，GstRTSPServer 创建 GstRTSPClient，它从服务器继承 GstRTSPMountPoints GstRTSPSessionPool GstRTSPAuth 和 GstRTSPThreadPool
- GstRTSPContext - 传递的包含上下文的信息
- GstRTSPMedia - GstRTSPMedia 包含完整的 GStreamer 管道，用于管理发送到客户端的流。实际的数据传输是由 GstRTSPMedia 创建和公开的 GstRTSPStream 对象完成的
- GstRTSPMediaFactory - GstRTSPMediaFactory 负责基于传递的 URL 创建或回收 GstRTSPMedia 对象。改对象的默认实现可以创建包含管道的 GstRTSPMedia对象。
- GstRTSPMediaFactoryURI - 这个专用的 GstRTSPMediaFactory 从一个由 `gst_media_factory_uri_set_uri` 提供的URI构造媒体管道
- GstRTSPMountPoints - 对象维护路径和 GstRTSPMediaFactory 对象之间的关系。该对象通常提供给 GstRTSPClient，用于查找附加到路径的媒体
- GstRTSPOnvifClient - 
- GstRTSPOnvifMedia - 一个 GstRTSPOvifMedia 包含了完整的 GStreamer 管道来管理到客户端的流。实际的数据传输是由 GstRTSPMedia 创建和公开的 GStRTSPStream对象完成的
- GstRTSPOnvifMediaFactory - GstRTSPOnvifMediaFactory 负责基于传递的 URL 创建或回收GstRTSPMedia对象。与 GstRTSPMediaFactory不同，它支持特殊的ONVIF特性，并且可以在普通 GstRTSPMedia 之外创建 GstRTSPOnvifMedia。
- GstRTSPServer - 服务器对象是侦听端口连接并创建 GstRTSPOnvifClient 对象来处理这些连接的对象
- GstRTSPParams - 
- GstRTSPPermissions - GstRTSPPermissions 对象 包含角色和关联权限的数据。角色用字符串表示、权限用通用的 GstStructure 表示。
- GstSDPInfo - 
- GstRTSPClient -
- GstRTSPServer - 
- GstRSTPSession - 由id标识，在创建会话和管理器媒体及其配置的GstRTSPSessionPool中是唯一的。
- GstRSTPSessionMedia - 使用给定的路径管理 GstRTSPMedia。
- GstRTSPSessionPool - 管理 `GstRTSPSession`
- GstRTSPStream - 管理一个流的数据传输。它由payloader元素和pad创建，源pad为流生成 RTP 包
- GstRTSPStreamTransport - 配置 GstRTSPStream 使用的传输。它通常由 GstRTSPSessionMedia 对象管理
- GstRTSPThreadPool - 管理各种服务器任务的可重用线程。目前定义的线程类型可以在 GstRTSPThreadType中找到。
- GstRTSPToken - 包含执行当前请求的用户的权限和角色。令牌通常在用户通过 GstRTSPAuth 对象进行身份验证时候创建，然后作为对当前请求的当前令牌放置。

# GStreamer Validate

> GSValidate 是一个工具，它允许 GStreamer 开发人员检查他们编写的 GstElements 是否按照他们应该的方式运行。它最初是为插件开发人员提供的一个工具，以检查他们是否以正确的方式使用框架。
