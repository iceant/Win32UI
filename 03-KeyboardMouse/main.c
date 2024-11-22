#include <main.h>
#include <stdio.h>
/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#define KEYBOARD_MOUSE_CLASSNAME  "Win32UI.KeyBoardMouse"

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
/* -------------------------------------------------------------------------------------------------------------- */
/*  */

static char Directions[128];
static int XVal = 10;
static int YVal = 30;

static void OnDestroy(HWND hwnd){
    PostQuitMessage(0);
}

static BOOL OnCreate(HWND hwnd, CREATESTRUCT FAR* lpCreateStruct){
    strncpy(Directions, "Try resizing this window.", ARRAY_SIZE(Directions));
    return TRUE;
}

static void OnPaint(HWND hwnd){
    PAINTSTRUCT PaintStruct;
    RECT Rect;

    static char* Message[]={
            "WM_CHAR",
            "WM_KEY",
            "WM_SYSKEY",
            "WM_MOUSEMOVE",
            "WM_MOUSEDOWN",
            "WM_MOUSEUP",
            "WM_MOUSEWHEEL",
    };

    HDC PaintDC = BeginPaint(hwnd, &PaintStruct);
    SetBkMode(PaintDC, GetSysColor(COLOR_WINDOW));
    HFONT OldFont = SelectFont(PaintDC, GetStockObject(OEM_FIXED_FONT));
    GetClientRect(hwnd, &Rect);
    Rect.top = 5;

    DrawText(PaintDC, "MOUSE AND KEYBOARD DEMONSTRATION", -1, &Rect, DT_CENTER);

    Rect.top = 20;
    Rect.bottom = 40;

    DrawText(PaintDC, "(Try experimenting with the mouse and keyboard)", -1, &Rect, DT_CENTER);

    SelectFont(PaintDC, OldFont);

    for(int i=0; i< ARRAY_SIZE(Message); i++){
        TextOut(PaintDC, XVal, YVal + (20 * (i+1)), Message[i], strlen(Message[i]));
    }

    EndPaint(hwnd, &PaintStruct);
}

static void OnChar(HWND hwnd, UINT ch, int cRepeat){
    char s[100];
    HDC hdc = GetDC(hwnd);
    size_t sz = snprintf(s, ARRAY_SIZE(s), "WM_CHAR ==> Ch = %c, cRepeat = %d ", ch, cRepeat);
    SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
    TextOut(hdc, XVal, YVal+20, s, sz);
    ReleaseDC(hwnd, hdc);
}

static void OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags) {
    char s[100];
    HDC hdc = GetDC(hwnd);
    size_t sz = 0;
    if(fDown){
        sz = snprintf(s, ARRAY_SIZE(s), "WM_KEYDOWN ==> vk = %d, fDown = %d, cRepeat = %d, flags = %d        "
            , vk, fDown, cRepeat, flags);
    }else{
        sz = snprintf(s, ARRAY_SIZE(s), "WM_KEYUP ==> vk = %d, fDown = %d, cRepeat = %d, flags = %d        "
                , vk, fDown, cRepeat, flags);
    }
    SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
    TextOut(hdc, XVal, YVal + 40, s, sz);

    ReleaseDC(hwnd, hdc);
}

static void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags) {
    char s[100];
    HDC hdc = GetDC(hwnd);
    size_t sz = 0;

    sz = snprintf(s, ARRAYSIZE(s), "WM_MOUSEMOVE ==> x = %d, y = %d, keyFlags = %d   ", x, y, keyFlags);

    if((keyFlags & MK_CONTROL) == MK_CONTROL){
        SetTextColor(hdc, RGB(0, 0, 255));
    }
    if((keyFlags & MK_LBUTTON) == MK_LBUTTON){
        SetTextColor(hdc, RGB(0, 255, 0 ));
    }
    if((keyFlags & MK_RBUTTON) == MK_RBUTTON){
        SetTextColor(hdc, RGB(255, 0, 0));
    }
    if((keyFlags & MK_SHIFT) == MK_SHIFT){
        SetTextColor(hdc, RGB(255, 0, 255));
    }
    SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
    TextOut(hdc, XVal, YVal+80, s, sz);

    ReleaseDC(hwnd, hdc);
}

static void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags){
    char s[100];
    HDC hdc = GetDC(hwnd);
    size_t sz = 0;

    if(fDoubleClick){
        sz = snprintf(s, ARRAY_SIZE(s), "WM_LBUTTONDBLCLK ==> Db = %d, x = %d, y = %d, flags = %d    "
            , fDoubleClick, x, y, keyFlags);
    }else{
        sz = snprintf(s, ARRAY_SIZE(s), "WM_LBUTTONDOWN ==> Db = %d, x = %d, y = %d, flags = %d    "
                , fDoubleClick, x, y, keyFlags);
    }

    SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
    TextOut(hdc, XVal, YVal+100, s, sz);

    ReleaseDC(hwnd, hdc);
}

static void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags){
    char s[100];
    HDC hdc = GetDC(hwnd);
    size_t sz = 0;

    sz = snprintf(s, ARRAY_SIZE(s), "WM_LBUTTONUP ==> x = %d, y = %d, keyFlags = %d    "
        ,x, y, keyFlags);

    SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
    TextOut(hdc, XVal, YVal + 120, s, sz);

    ReleaseDC(hwnd, hdc);
}

static void OnSysKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags){
    char s[100];
    HDC hdc = GetDC(hwnd);
    size_t sz = 0;

    SetBkColor(hdc, GetSysColor(COLOR_WINDOW));

    if(fDown){
        sz = snprintf(s, ARRAY_SIZE(s), "WM_SYSKEYDOWN ==> vk = %d, fDown = %d, cRepeat = %d, flags = %d      "
            , vk, fDown, cRepeat, flags);
        TextOut(hdc, XVal, YVal + 60, s, sz);
        FORWARD_WM_SYSKEYDOWN(hwnd, vk, cRepeat, flags, DefWindowProc);
    }else{
        sz = snprintf(s, ARRAY_SIZE(s), "WM_SYSKEYUP ==> vk = %d, fDown = %d, cRepeat = %d, flags = %d        "
                , vk, fDown, cRepeat, flags);
        TextOut(hdc, XVal, YVal + 60, s, sz);
        FORWARD_WM_SYSKEYUP(hwnd, vk, cRepeat, flags, DefWindowProc);
    }

    ReleaseDC(hwnd, hdc);
}

static void OnMouseWheel(HWND hwnd, int x, int y, int z, UINT fwKeys){
    char s[100];
    HDC hdc = GetDC(hwnd);
    size_t sz = 0;

    SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
    sz = snprintf(s, ARRAY_SIZE(s), "WM_MOUSEWHEEL ==> xPos = %d, yPos = %d, zDelta = %d, fwKeys = %d        "
            , x, y, z, fwKeys);
    TextOut(hdc, XVal, YVal + 140, s, sz);
    FORWARD_WM_MOUSEWHEEL(hwnd, x, y, z, fwKeys, DefWindowProc);
    ReleaseDC(hwnd, hdc);
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam){
    switch (Message) {
        HANDLE_MSG(hwnd, WM_CHAR, OnChar);
        HANDLE_MSG(hwnd, WM_KEYDOWN, OnKey);
        HANDLE_MSG(hwnd, WM_KEYUP, OnKey);
        HANDLE_MSG(hwnd, WM_MOUSEMOVE, OnMouseMove);
        HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
        HANDLE_MSG(hwnd, WM_LBUTTONDBLCLK, OnLButtonDown);
        HANDLE_MSG(hwnd, WM_LBUTTONDOWN, OnLButtonDown);
        HANDLE_MSG(hwnd, WM_LBUTTONUP, OnLButtonUp);
        HANDLE_MSG(hwnd, WM_MOUSEWHEEL, OnMouseWheel);
        HANDLE_MSG(hwnd, WM_SYSKEYDOWN, OnSysKey);
        HANDLE_MSG(hwnd, WM_SYSKEYUP, OnSysKey);
        HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
        HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
        default:
            return DefWindowProc(hwnd, Message, wParam, lParam);
    }
}

static BOOL Register(HINSTANCE hInstance){
    WNDCLASS WndClass;
    memset(&WndClass, 0, sizeof(WndClass));
    WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    WndClass.lpfnWndProc = WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    WndClass.lpszClassName = KEYBOARD_MOUSE_CLASSNAME;
    WndClass.lpszMenuName = 0;

    return RegisterClass(&WndClass)!=0?TRUE:FALSE;
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

    HWND Main_HWND = CreateWindow(KEYBOARD_MOUSE_CLASSNAME, "KeyBoardMouse"
        , WS_OVERLAPPEDWINDOW
        , CW_USEDEFAULT, CW_USEDEFAULT
        , CW_USEDEFAULT, CW_USEDEFAULT
        , NULL          /* Parent */
        , NULL          /* Menu */
        , hInstance
        , NULL
        );

    if(!Main_HWND){
        return FALSE;
    }

    ShowWindow(Main_HWND, nCmdShow);
    UpdateWindow(Main_HWND);

    while(GetMessage(&Msg, NULL, 0, 0)){
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    return Msg.wParam;
}