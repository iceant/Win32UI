#include <main.h>
#include <stdio.h>
#include <strsafe.h>
/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#define RESOURCE_DEMO_CLASSNAME  "Win32UI.ResourceDemo"

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
/* -------------------------------------------------------------------------------------------------------------- */
/*  */

static HINSTANCE g_hInstance;
static HANDLE g_hResource;
// static HANDLE g_hFindIcon;

static int ScrollWidth;
static int MaxLines = 21;
static int Start = 33;
static int TextHeight;
static int PageSize;
static int nPosition = 0;

/* -------------------------------------------------------------------------------------------------------------- */
/*  */
static void ShowErrorMessage(LPCTSTR lpszFunction)
{
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}

static BOOL OnCreate(HWND hwnd, CREATESTRUCT FAR* lpCreateStruct){
    g_hInstance = lpCreateStruct->hInstance;
    g_hResource = LoadResource(g_hInstance, FindResource(g_hInstance, "Brahma", "CUSTOM"));
    return TRUE;
}

static void OnDestroy(HWND hwnd){
    FreeResource(g_hResource);
    PostQuitMessage(0);
}


static void OnPaint(HWND hwnd){
    PAINTSTRUCT PaintStruct;
    RECT Rect;
    char* Poem;
    HBITMAP hBitMap;


    HDC hdc = BeginPaint(hwnd, &PaintStruct);

    SetBkMode(hdc, TRANSPARENT);
    Poem = (char*) LockResource(g_hResource);
    GetClientRect(hwnd, &Rect);
    Rect.left += 10;
    Rect.top += 10;
    DrawText(hdc, Poem, -1, &Rect, DT_EXTERNALLEADING);
    GlobalUnlock(g_hResource);

    hBitMap = LoadBitmap(g_hInstance, "find_icon");
    // hBitMap = (HBITMAP)LoadImage(g_hInstance, "18655_find_icon.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (!hBitMap) {
        // MessageBox(hwnd, "Failed to load bitmap", "Error", MB_OK|MB_ICONERROR);
        ShowErrorMessage(__FUNCTION__);
    }else {
        HDC BitmapDC = CreateCompatibleDC(hdc);
        HBITMAP BitmapOld = SelectObject(BitmapDC, hBitMap);
        // for (int i=1; i < MaxLines; i++) {
        //     for (int j=1; j < MaxLines; j++) {
        //         BitBlt(hdc, i*258, j*258, 256, 256, BitmapDC, 200, 100, SRCCOPY);
        //     }
        // }
        BitBlt(hdc, 300, 0, 256, 256, BitmapDC, 0, 0, SRCCOPY);
        SelectBitmap(BitmapDC, BitmapOld);
        DeleteDC(BitmapDC);
        DeleteBitmap(hBitMap);
    }


    EndPaint(hwnd, &PaintStruct);
}

static void NotYetAvailable(HWND hwnd){
    MessageBoxA(hwnd, "Not Yet Available", "Notify", MB_OK);
}

static BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam){
    switch (Message) {
        case WM_INITDIALOG:{
            return TRUE;
        }
        case WM_COMMAND:{
            if(LOWORD(wParam)==IDOK || LOWORD(wParam)==IDCANCEL){
                EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            }
            break;
        }
    }
    return FALSE;
}

static void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
    switch (id) {
        case CM_ABOUT:{
            FARPROC AboutBox = MakeProcInstance((FARPROC)AboutDlgProc, g_hInstance);
            DialogBox(g_hInstance, "ABOUT", hwnd, (DLGPROC)AboutBox);
            FreeProcInstance(AboutBox);
            break;
        }
        case CM_BITMAP:{
            NotYetAvailable(hwnd);
            break;
        }
        case CM_BRAHMIN:{
            NotYetAvailable(hwnd);
            break;
        }
        case CM_WOODNOTES:{
            NotYetAvailable(hwnd);
            break;
        }
        case CM_SEASHORE:{
            NotYetAvailable(hwnd);
            break;
        }
        default:
            FORWARD_WM_COMMAND(hwnd, id, hwndCtl, codeNotify, DefWindowProc);
    }
}
/* -------------------------------------------------------------------------------------------------------------- */
/*  */

static LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam){
    switch (Message) {
        HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
        HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
        HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
        HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
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
    WndClass.hIcon = LoadIcon(hInstance, "Icon");
    WndClass.hCursor = LoadCursor(hInstance, "Cursor");
    WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    WndClass.lpszClassName = RESOURCE_DEMO_CLASSNAME;
    WndClass.lpszMenuName = "Menu";

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

    HWND Main_HWND = CreateWindow(RESOURCE_DEMO_CLASSNAME, "ResourceDemo"
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