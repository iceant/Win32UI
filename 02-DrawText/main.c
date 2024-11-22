#include <main.h>

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#define DRAW_TEXT_CLASSNAME  "Win32UI.DrawText"

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
/* -------------------------------------------------------------------------------------------------------------- */
/*  */

static char Directions[128];

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

    HDC PaintDC = BeginPaint(hwnd, &PaintStruct);

    SetBkMode(PaintDC, TRANSPARENT);
    TextOut(PaintDC, 10, 10, Directions, lstrlen(Directions));
    GetClientRect(hwnd, &Rect);
    DrawText(PaintDC, "The middle of the road.", -1, &Rect, DT_SINGLELINE|DT_CENTER|DT_VCENTER);

    EndPaint(hwnd, &PaintStruct);
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam){
    switch (Message) {
        HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
        HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
        HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
        default:
            return DefWindowProc(hwnd, Message, wParam, lParam);
    }
}

static BOOL Register(HINSTANCE hInstance){
    WNDCLASS WndClass;
    memset(&WndClass, 0, sizeof(WndClass));
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    WndClass.lpszClassName = DRAW_TEXT_CLASSNAME;
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

    HWND Main_HWND = CreateWindow(DRAW_TEXT_CLASSNAME, "DrawText"
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