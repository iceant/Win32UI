# Windows GUI 程序的主要框架

## `WinMain`应用入口

Windows GUI 程序入口是 `WinMain`, 代码如下:

```c

int WINAPI WinMain(HINSTANCE hInstance
                   , HINSTANCE hPrevInstance
                   , LPSTR lpszCmdParam
                   , int nCmdShow){
    MSG Msg;
    
    if(!hPrevInstance){
        if(!Register(hInstance)){
            return FALSE;
        }
    }
    
    MainFrame_HWND = CreateWindow(MAINFRAME_CLASSNAME, "MainFrame"
                                  , WS_OVERLAPPEDWINDOW
                                  , CW_USEDEFAULT, CW_USEDEFAULT
                                  , CW_USEDEFAULT, CW_USEDEFAULT
                                  , NULL
                                  , NULL
                                  , hInstance
                                  , NULL
                                  );
    
    if(!MainFrame_HWND){
        return FALSE;
    }
    
    ShowWindow(MainFrame_HWND, nCmdShow);
    UpdateWindow(MainFrame_HWND);
    
    while(GetMessage(&Msg, NULL, 0, 0)){
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    
    return Msg.wParam;
}
```

**参数解释:**

- `hInstance`:  代表进程的地址空间，.EXE文件从此处开始加载(在Windows95中，这个数是4MB或者 `0x00400000`，在我的64位Windows10中是`00007ff64e440000`)
- `hPrevInstance`: 表示本程序的前一个实例，WIN32中当前地址空间不会有应用的其它实例运行，因此这个值一般都是`NULL`, 在我的64位Windows10中是`0000000000000000`
- `lpszCmdParam`: 是一个字符串，包含从命令行传来的任何信息，这时一个 ANSI 字符串，如果需要Unicode字符串，可以使用以下方法 :
  - `LPWSTR GetCommandLineW(void)`
  - `LPWSTR* CommandLineToArgvW(LPCWSTR lpCmdLine, int* pArgc)`获取
- `nCmdShow`: 定义第一次打开主窗口的状态，比如：按最大化、最小化或者简单的以默认大小和外观出现。

**运行示例:**

运行 `FirstWinGUI.exe arg1 arg2`，会得到类似以下结果

- hInstance: 00007ff7d4c30000
- hPrevInstance: 0000000000000000
- lpszCmdShow: arg1 arg2
- nCmdShow: 10 `SW_SHOWDEFAULT`

**nCmdShow可选值:**

```
#define SW_HIDE 0
#define SW_SHOWNORMAL 1
#define SW_NORMAL 1
#define SW_SHOWMINIMIZED 2
#define SW_SHOWMAXIMIZED 3
#define SW_MAXIMIZE 3
#define SW_SHOWNOACTIVATE 4
#define SW_SHOW 5
#define SW_MINIMIZE 6
#define SW_SHOWMINNOACTIVE 7
#define SW_SHOWNA 8
#define SW_RESTORE 9
#define SW_SHOWDEFAULT 10
#define SW_FORCEMINIMIZE 11
#define SW_MAX 11
```

## 注册窗口类

```c
BOOL MainFrame_Register(HINSTANCE hInstance){
    WNDCLASS WndClass;
    memset(&WndClass, 0, sizeof(WndClass));
    
    WndClass.style = CS_HREDRAW|CS_VREDRAW;
    WndClass.lpfnWndProc = WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = MAINFRAME_CLASSNAME;
    
    return (RegisterClass(&WndClass)!=0)?TRUE:FALSE;
    
}
```

- `style` 窗口类的样式

  ```c
  #define CS_VREDRAW 0x0001
  #define CS_HREDRAW 0x0002
  #define CS_DBLCLKS 0x0008
  #define CS_OWNDC 0x0020
  #define CS_CLASSDC 0x0040
  #define CS_PARENTDC 0x0080
  #define CS_NOCLOSE 0x0200
  #define CS_SAVEBITS 0x0800
  #define CS_BYTEALIGNCLIENT 0x1000
  #define CS_BYTEALIGNWINDOW 0x2000
  #define CS_GLOBALCLASS 0x4000
  #define CS_IME 0x00010000
  #define CS_DROPSHADOW 0x00020000
  ```

  

- `lpfnWndProc`: 窗口消息接收入口

  在Windows中，GUI 窗口创建后等待消息进行响应。该入口针对不同的消息进行处理。

  ```c
  static LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam){
      switch (Message) {
          HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
          default:
              return DefWindowProc(hwnd, Message, wParam, lParam);
      }
  }
  ```

  - `WM_DESTROY`是窗口销毁消息
  - `DefWindowProc`是默认的处理方法

- `hInstance`: 表示窗口应用的实例

- `hIcon`: 表示窗口的图标

- `hCursor`: 表示窗口的鼠标样式, `IDC_ARROW` 表示箭头样式的鼠标

- `hbrBackground`: 表示背景样式

- `lpszMenuName`: 菜单资源的名称

- `lpszClassName`: 窗口的类名称，一个窗口有一个唯一的类名

## 处理窗口消息

```c
static LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam){
    switch (Message) {
        HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
        default:
            return DefWindowProc(hwnd, Message, wParam, lParam);
    }
}
```

在 `#include <windowsx.h>` 头文件中包含 `HANDLE_MSG` 和 `FORWARD_WM_xxx` 的简便方法，

- `HANDLE_MSG`用来注册登记消息的处理方法
- `FORWARD_WM_XXX(hwnd, ..., fn)`表示将处理方法转移给指定的`fn`

## 创建窗口

```c
HWND MainFrame_HWND = CreateWindow(MAINFRAME_CLASSNAME, "MainFrame"
                              , WS_OVERLAPPEDWINDOW /* 窗口样式 */
                              , CW_USEDEFAULT, CW_USEDEFAULT /* x, y */
                              , CW_USEDEFAULT, CW_USEDEFAULT /* width, height */
                              , NULL /* parent */
                              , NULL /* menu */
                              , hInstance
                              , NULL /* lParam */
                              );
```

- 创建窗口时指定要创建的窗口类名: `MAINFRAME_CLASSNAME`
- `MainFrame` 为窗口实例的名称
- `WS_OVERLAPPEDWINDOW`: 为窗口样式，可以指定是否要支持鼠标双击，是否要显示边框，是否需要滚动条等等
- `x, y, width, height`:  设置窗口显示的位置，以及宽高尺寸信息
- `parent`: 父窗口的句柄
- `menu`: 菜单句柄
- `hInstance`: 窗口所属的实例
- `lParam`: `void*`类型数据

## 显示窗口

显示窗口时，可以指定 `nCmdShow`参数让窗口显示最大、最小或者隐藏等

```c
ShowWindow(MainFrame_HWND, nCmdShow);
UpdateWindow(MainFrame_HWND);
```

## 消息循环

窗口创建后，需要通过循环一直等待消息，并将消息传递给窗口，以下代码实现这个目标:

```c
 while(GetMessage(&Msg, NULL, 0, 0)){
    TranslateMessage(&Msg);
    DispatchMessage(&Msg);
}
```

