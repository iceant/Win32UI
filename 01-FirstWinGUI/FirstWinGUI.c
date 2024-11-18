#include <FirstWinGUI.h>
#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <MainFrame.h>


/* -------------------------------------------------------------------------------------------------------------- */
/*  */
static HWND MainFrame_HWND;

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

static BOOL Register(HINSTANCE hInstance){
    return MainFrame_Register(hInstance);
}
/* -------------------------------------------------------------------------------------------------------------- */
/*  */

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow){
    MSG Msg;
    
    if(!hPrevInstance){
        if(!Register(hInstance)){
            return FALSE;
        }
    }
    GetCommandLineW();
    CommandLineToArgvW()
    
    printf("hInstance: %p\n", hInstance);
    printf("hPrevInstance: %p\n", hPrevInstance);
    printf("lpszCmdShow: %s\n", lpszCmdParam);
    printf("nCmdShow: %d\n", nCmdShow);
    
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
    SW_HIDE;
    ShowWindow(MainFrame_HWND, nCmdShow);
    UpdateWindow(MainFrame_HWND);
    
    while(GetMessage(&Msg, NULL, 0, 0)){
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    
    return Msg.wParam;
}