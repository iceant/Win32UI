#include <MainFrame.h>
#include <windowsx.h>

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

static void OnDestroy(HWND hwnd){
    PostQuitMessage(0);
}

/* -------------------------------------------------------------------------------------------------------------- */
/*  */



static LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam){
    switch (Message) {
        HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
        default:
            return DefWindowProc(hwnd, Message, wParam, lParam);
    }
}

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

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