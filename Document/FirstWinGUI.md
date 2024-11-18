# Windows GUI 程序的主要框架

## `WinMain`

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

