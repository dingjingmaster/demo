# GNOME JavaScript(gjs)

## gjs 与 其它 JavaScript 框架的区别

### 导入和模块

在 gjs 中每个文件都被视为一个“模块”。任何使用“var”(或全局作用域)声明的变量都会被导出。

每个“模块”都可以通过“import”被导入其它模块导入。

假设有文件fileName.js，你可以使用 object['fileName']来访问它。

a.js:
```js
var A = class A {};
```

b.js:
```js
// 标准语法
const A = imports.a;
let a = new A.A();
```
或者
```js
// 快速语法
const {A} = imports.a;
let a = new A();
```

模块的查找路径定义在数据 `imports.searchPath` 中。

### 导入版本

在 GTK+ 程序中使用`pkg.require({})`，其它地方使用 `imports.gi.Versions.Gtk = X;`

### 日志输出

gjs 中有很多日志工具，有些是 gjs 特有的，有些是从 JavaScript 继承来的，还有一些是由GLib提供的

#### console 函数

console 函数从 gjs 1.70 (GNOME 41) 开始引入，此对象全局可用。

> 注意：console 函数不能与 `GNOME_SHELL_EXTENSION_UUID` 特性一起使用，这时候可以使用内置函数

#### 内置函数

```js
// Log a string, usually to `journalctl`
log('a message');

// Log an Error() with a stack trace and optional prefix
try {
    throw new Error('An error occurred');
} catch (e) {
    logError(e, 'ExtensionError');
}

// Print a message to stdout
print('a message');

// Print a message to stderr
printerr('An error occured');
```

在编写扩展时，print()和printer()不是特别有用，因为我们不容易访问gnome-shell的stdin和stderr管道。相反，您需要使用log()和logError()，并使用journalctl在新终端中观察日志:

```gjs
journalctl -f -o cat /usr/bin/gnome-shell
```

### GObject 类

gjs 支持原生ES6类，但需要对任何从 GObject 类(例如GTK+或GLib类的子类)扩展的类进行一些更改。

#### 标准的 ES6 类

```js
var A = class A extends B {
    constructor(x, y){
        super(x);
        this.y = y;
    }
}
```

#### GObject子类

```js
var A = GObject.registerClass(
    {
        GTypeName: 'A',
    },
    class A extends GObject.Object {
        constructor(x, y) {
            super(x);
            this.y = y;
        }
    }
);
```

## 异步编程

JavaScript 是一种单线程并发编程语言。并发是通过使用事件循环来实现的，该事件循环在处理一个事件时候停止，然后返回处理更多事件。

Promise API 是一个强大的框架，用于控制事件循环中的执行流，同时保持代码简单和可维护。

虽然 JavaScript 只能在单线程环境中运行，但是 GNOME api 包含许多使用任务线程执行阻塞操作的函数。

同时使用这三种工具，gjs 程序员可以根据有限级别来安排事件，保持代码的干净和可理解，并间接使用线程来编写响应性和高性能的代码

### Main Loop

为了以并发的方式处理事件，JavaScript引擎需要一个事件循环。虽然 gsj 是由 Firefox 和 SpiderMonkey JavaScript引擎驱动的，但是它使用了GLib事件循环。事件循环是GLib中并发和异步编程的基础，因此我们将详细介绍它。

如果您使用gio编写应用程序，应用程序或者它的子类，比如Gtk或者Adw，当调用 Gio.Application.run()时候，将为您启动一个主循环。如果您正在编写 GNOME-SHELL 扩展，您将使用已经在 GNOME shell 中运行的主循环。

了解如何为简单脚本创建主循环仍然很有用，因此让我们从创建GLib的示例开始，MainLoop和添加超时的源码：
```gjs
import GLib from 'gi://GLib';


// Here we're creating an event loop, to iterate the main context
const loop = new GLib.MainLoop(null, false);

// Here we're adding a timeout source to the main context that executes a
// callback after one second. The returned ID can be used to remove the source.
const sourceId = GLib.timeout_add_seconds(
    GLib.PRIORITY_DEFAULT,           // priority of the source
    1,                               // seconds to wait
    () => {                          // the callback to invoke
        return GLib.SOURCE_CONTINUE; // the return value; to recurse or not?
    }
);

// Here we're starting the loop, instructing it to process sources (events)
loop.run();
```
如果将上边例子当作脚本执行，它将永远不会退出，因为循环没有提供退出操作，看下面的例子将会在执行一次后退出：
```gjs
import GLib from 'gi://GLib';


const loop = new GLib.MainLoop(null, false);

const sourceId = GLib.timeout_add_seconds(GLib.PRIORITY_DEFAULT, 1, () => {
    loop.quit();

    return GLib.SOURCE_REMOVE;
});

log('Starting the main loop');

// This function will return when GLib.MainLoop.quit() is called
loop.run();

log('The main loop stopped');
```

### 事件源

GLib 中有很多类型的事件源，甚至可以创建自己的事件源，但最常见的两种事件源是：显示创建的超时源(定时器源)和空闲源：
```gjs
import GLib from 'gi://GLib';


const loop = new GLib.MainLoop(null, false);

// Timeout sources execute a callback when the interval is reached
const timeoutId = GLib.timeout_add_seconds(GLib.PRIORITY_DEFAULT, 1, () => {
    console.log('This callback was invoked because the timeout was reached');

    return GLib.SOURCE_REMOVE;
});


// Idle sources execute a callback when no other sources with a higher priority
// are ready.
const idleId = GLib.idle_add(GLib.PRIORITY_DEFAULT_IDLE, () => {
    console.log('This callback was invoked because no other sources were ready');

    return GLib.SOURCE_REMOVE;
});

loop.run();
```
源也可以隐式创建，就像调用gio中的异步方法一样。这些函数通常在后台线程中执行，然后在它们完成后添加GLib。源到调用者的主要上下文来调用回调
```gjs
import GLib from 'gi://GLib';
import Gio from 'gi://Gio';


const loop = new GLib.MainLoop(null, false);
const file = Gio.File.new_for_path('test-file.txt');

// GTask-based operations invoke a callback when the task completes
file.delete_async(GLib.PRIORITY_DEFAULT, null, (_file, result) => {
    console.log('This callback was invoked because the task completed');

    try {
        file.delete_finish(result);
    } catch (e) {
        logError(e);
    }
});

loop.run();
```
接下来展示一个稍微高级点的例子，在这种情况下，我们将为stdin创建一个输入流，然后当用户按Enter时候触发事件：
```gjs
import GLib from 'gi://GLib';
import Gio from 'gi://Gio';


const loop = new GLib.MainLoop(null, false);

const stdinDecoder = new TextDecoder('utf-8');
const stdinStream = new Gio.UnixInputStream({fd: 0});

// Here we create a GLib.Source using Gio.PollableInputStream.create_source(),
// set the priority and callback, then add it to main context
const stdinSource = stdinStream.create_source(null);
stdinSource.set_priority(GLib.PRIORITY_DEFAULT);
stdinSource.set_callback(() => {
    try {
        const data = stdinStream.read_bytes(4096, null).toArray();
        const text = stdinDecoder.decode(data).trim();

        print(`You typed: ${text}`);

        return GLib.SOURCE_CONTINUE;
    } catch (e) {
        logError(e);

        return GLib.SOURCE_REMOVE;
    }
});
const sourceId = stdinSource.attach(null);

// Start processing input
loop.run();
```

### 事件优先级

每个事件源都有一个优先级，以确定如果同时有多个事件源准备就绪，哪个事件源将被先“分流”。优先级只是整数和负数；数字越小优先级越高，以下是优先级表：
|变量|值|
|:---|:---|
|`GLib.PRIORITY_LOW`|300|
|`GLib.PRIORITY_DEFAULT_IDLE`|200|
|`Gdk.PRIORITY_REDRAW`|120|
|`Gtk.PRIORITY_RESIZE`|110|
|`GLib.PRIORITY_GIGH_IDLE`|100|
|`GLib.PRIORITY_DEFAULT`|0|
|`GLib.PRIORITY_GIGH`|-100|

在下面的例子中，我们将添加两个同时解析的超时源，但具有不同的优先级，以便按序调度：
```gjs
import GLib from 'gi://GLib';


const loop = new GLib.MainLoop(null, false);

const idleId = GLib.timeout_add_seconds(GLib.PRIORITY_DEFAULT_IDLE, 1, () => {
    console.log('idle source');

    return GLib.SOURCE_REMOVE;
});

const defaultId = GLib.timeout_add_seconds(GLib.PRIORITY_DEFAULT, 1, () => {
    console.log('default source');

    return GLib.SOURCE_REMOVE;
});

loop.run();
```

### 移出事件源

有两种方法可以从循环中删除源：
1. 如果它是用 `GLib.timeout_add()`或`GLib.idle_add()`添加的，这两个函数的返回值可以手动传递给 `GLib.Source.remove()`，从上下文中删除事件源
2. 在事件源对应的回调函数里通过回调函数的返回值来删除源

例子：
```gjs
import GLib from 'gi://GLib';


const loop = new GLib.MainLoop(null, false);

const idleId = GLib.idle_add(GLib.PRIORITY_DEFAULT, () => {
    console.log('This callback will only be invoked once.');

    return GLib.SOURCE_REMOVE;
});

const timeoutId = GLib.timeout_add_seconds(GLib.PRIORITY_DEFAULT, 1, () => {
    console.log('This callback will be invoked once per second, until removed');

    return GLib.SOURCE_CONTINUE;
});

const removeId = GLib.timeout_add_seconds(GLib.PRIORITY_DEFAULT, 5, () => {
    console.log('This callback will be invoked once after 5 seconds');

    GLib.Source.remove(timeoutId);

    return GLib.SOURCE_REMOVE;
});

loop.run();
```
> 在GIO中由异步方法创建的事件源不能直接删除，然而可以通过传递一个 Gio.Cancellable 参数来取消当前操作，进而执行源的删除。

### Promises

在 gjs 中，Promises 基本上是一个事件源，当 reslove() 或 reject() 函数被调用时候触发。如果你是 JavaScript 中 Promise 和 async 的新手，应该查找相关资料做一个了解。
- Using promises
- async/await

传统用法

```gjs
import GLib from 'gi://GLib';

const loop = new GLib.MainLoop(null, false);

// Returns a Promise that randonly fails or succeeds after one second
function unreliablePromise() {
    return new Promise((resolve, reject) => {
        GLib.timeout_add_seconds(GLib.PRIORITY_DEFAULT, 1, () => {
            if (Math.random() >= 0.5)
                resolve('success');
            else
                reject(Error('failure'));

            return GLib.SOURCE_REMOVE;
        });
    });
}


// When using a Promise in the traditional manner, you must chain to it with
// `then()` to get the result and `catch()` to trap errors.
unreliablePromise().then(result => {
    // Logs "success"
    console.log(result);
}).catch(e => {
    // Logs "Error: failure"
    logError(e);
});

// A convenient short-hand in GJS is just passing `logError` to `catch()`
unreliablePromise().catch(logError);


loop.run();
```

async/await
尽管 promise 对象为异步操作提供了简单的API，但是它仍然有回调的负担，使用 async/await 范式，程序员可以重新获得同步编程的简单性，并获得异步执行的好处
```gjs
import GLib from 'gi://GLib';


const loop = new GLib.MainLoop(null, false);

// Returns a Promise that randomly fails or succeeds after one second
function unreliablePromise() {
    return new Promise((resolve, reject) => {
        GLib.timeout_add_seconds(GLib.PRIORITY_DEFAULT, 1, () => {
            if (Math.random() >= 0.5)
                resolve('success');
            else
                reject(Error('failure'));

            return GLib.SOURCE_REMOVE;
        });
    });
}


// An example async function, demonstrating how Promises can be resolved
// sequentially while catching errors in a try..catch block.
async function exampleAsyncFunction() {
    try {
        let count = 0;

        while (true) {
            await unreliablePromise();
            console.log(`Promises resolved: ${++count}`);
        }
    } catch (e) {
        logError(e);
        loop.quit();
    }
}

// Run the async function
exampleAsyncFunction();

loop.run();
```

### 异步操作

虽然不能在多个线程上运行JavaScript，但几乎所有GIO操作都有异步变体。这些工作通过收集函数参数中必要的输入，将工作发送到另一个线程上完成，然后在完成时在主线程中调用回调

传统用法
尽管是用JavaScript编写的，下面的例子是C程序员通常如何使用这些函数的。
虽然这不会在操作期间阻塞主线程，但与JavaScript等高级语言中常用的现代编程风格相比，这是很尴尬的。

```gjs
import GLib from 'gi://GLib';
import Gio from 'gi://Gio';


/**
 * This callback will be invoked once the operation has completed.
 *
 * @param {Gio.File} file - the file object
 * @param {Gio.AsyncResult} result - the result of the operation
 */
function loadContentsCb(file, result) {
    try {
        const [length, contents] = file.load_contents_finish(result);

        console.log(`Read ${length} bytes from ${file.get_basename()}`);
    } catch (e) {
        logError(e, `Reading ${file.get_basename()}`);
    }
}

const file = Gio.File.new_for_path('test-file.txt');

// This method passes the file object to a task thread, reads the contents in
// that thread, then invokes loadContentsCb() in the main thread.
file.load_contents_async(GLib.PRIORITY_DEFAULT, null, loadContentsCb);
```

async/await

对于GJS程序员来说，Promise最方便的用法之一就是包装这些异步函数，并使用async/await模式来重新获得同步流:

```gjs
import GLib from 'gi://GLib';
import Gio from 'gi://Gio';


const file = Gio.File.new_for_path('test-file.txt');

// Here is the synchronous, blocking form of this operation
try {
    const [, contents] = file.load_contents(null);

    console.log(`Read ${contents.length} bytes from ${file.get_basename()}`);
} catch (e) {
    logError(e, `Reading ${file.get_basename()}`);
}

// Here is an asynchronous, non-blocking wrapper in use
try {
    const [, contents] = await new Promise((resolve, reject) => {
        file.load_contents_async(GLib.PRIORITY_DEFAULT, null, (_file, res) => {
            try {
                // If the task succeeds, we can return the result with resolve()
                resolve(file.load_contents_finish(res));
            } catch (e) {
                // If an error occurred, we can report it using reject()
                reject(e);
            }
        });
    });

    console.log(`Read ${contents.length} bytes from ${file.get_basename()}`);
} catch (e) {
    logError(e, `Reading ${file.get_basename()}`);
}
```

需要注意的一点是，通过使用 async/await 模式，您可以在利用异步执行的同时保持一种简单的、类似同步的编程风格。

有了准备好的包装其函数，你甚至可以并行的运行这些操作；每个操作都使用一个线程
```gjs
import GLib from 'gi://GLib';
import Gio from 'gi://Gio';


/**
 * A simple Promise wrapper that elides the number of bytes read.
 *
 * @param {Gio.File} file - a file object
 * @returns {Promise<Uint8Array>} - the file contents
 */
function loadContents(file) {
    return new Promise((resolve, reject) => {
        file.load_contents_async(GLib.PRIORITY_DEFAULT, null, (_file, res) => {
            try {
                resolve(file.load_contents_finish(res)[1]);
            } catch (e) {
                reject(e);
            }
        });
    });
}

try {
    // A list of files to read
    const files = [
        Gio.File.new_for_path('test-file1.txt'),
        Gio.File.new_for_path('test-file2.txt'),
        Gio.File.new_for_path('test-file3.txt'),
    ];

    // Creating a Promise for each operation
    const operations = files.map(file => loadContents(file));

    // Run them all in parallel
    const results = await Promise.all(operations);

    results.forEach((result, i) => {
        console.log(`Read ${result.length} bytes from "${files[i].get_basename()}"`);
    });
} catch (e) {
    logError(e);
}
```

promisify Helper

在 gjs 1.54 及其之后的版本，可以无缝支持异步函数，但是在那之前，必须使用 promisify Helper 来自动创建 Promise 包装器。

`Gio._promisify()`实用程序替换了类原型上的原始函数，以便可以在类的任何实例（包括子类）上调用它。只需传递类原型"async"函数名和"finish"函数名作为参数：
```gjs
Gio._promisify(Gio.InputStream.prototype, 'read_bytes_async',
    'read_bytes_finish');
```
然后可以像使用任何其它 promise 一样使用该函数，而不需要自定义包装器，只需省略 callback 函数：
```gjs
const inputStream = new Gio.UnixInputStream({fd: 0});

try {
    const bytes = await inputStream.read_bytes_async(4096,
        GLib.PRIORITY_DEFAULT, null);
} catch (e) {
    logError(e, 'Failed to read bytes');
}
```
原始函数仍然可用，并且可以通过简单的传递回调来使用：
```gjs
inputStream.read_bytes_async(
    4096,
    GLib.PRIORITY_DEFAULT,
    null,
    (stream_, result) => {
        try {
            const bytes = inputStream.read_bytes_finish(result);
        } catch (e) {
            logError(e, 'Failed to read bytes');
        }
    }
);
```

### 取消操作

Gio的异步方法的一个好处是能够在函数启动后取消他们，这时通过 Gio.Cancellable对象实现的，此操作可以在任何线程触发。

```gjs
import GLib from 'gi://GLib';
import Gio from 'gi://Gio';


/**
 * This callback will be invoked once the operation has completed, even if
 * it was cancelled.
 *
 * @param {Gio.File} file - the file object
 * @param {Gio.AsyncResult} result - the result of the operation
 */
function loadContentsCb(file, result) {
    try {
        const [length, contents] = file.load_contents_finish(result);

        console.log(`Read ${length} bytes from ${file.get_basename()}`);
    } catch (e) {
        // If the operation was cancelled we probably did it on purpose, in
        // which case we may just want to mute the error
        if (!e.matches(Gio.IOErrorEnum, Gio.IOErrorEnum.CANCELLED))
            logError(e, `Reading ${file.get_basename()}`);
    }
}

const file = Gio.File.new_for_path('test-file.txt');

// This is the cancellable we will pass to the asynchronous method. We need to
// hold a reference to this somewhere if we want to cancel it.
const cancellable = new Gio.Cancellable();

// This method passes the file object to a task thread, reads the contents in
// that thread, then invokes loadContentsCb() in the main thread.
file.load_contents_async(GLib.PRIORITY_DEFAULT, cancellable, loadContentsCb);

// Cancel the operation by triggering the cancellable
cancellable.cancel();
```
同一个 Gio.Cancellable 对象可以用于多个操作中，但每个调用Gio.Cancellable.cancel()时候将所有关联操作都取消。

一旦执行了取消操作，您应该释放对它的引用，并为以后的操作重新创建一个 Gio.Cancellable 对象



