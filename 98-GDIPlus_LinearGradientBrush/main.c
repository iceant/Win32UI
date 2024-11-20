#include <main.h>
#include <string.h>
#include <gdiplus.h>
#include <stdio.h>
#include <uiToolkit.h>
/* -------------------------------------------------------------------------------------------------------------- */
/*  */
#define MAIN_CLASSNAME "MainWindow"

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

static ULONG_PTR GdiplusToken;

static void OnDestroy(HWND hwnd){
    PostQuitMessage(0);
}

static void OnPaint(HWND hwnd){

    RECT Rect;
    POINT Point;
    GpPen* pen = NULL;
    GpGraphics* graphics = NULL;
    ARGB color = uiToolkit_ARGB(0xFF, 0xef, 0xef, 0xef);
    GpUnit unit = UnitPixel;
    REAL width = 5.0f;
    PAINTSTRUCT PaintStruct;
    GetClientRect(hwnd, &Rect);
    int nWidth = Rect.right-Rect.left;
    int nHeight = Rect.bottom - Rect.top;
    GetCursorPos(&Point);

//    MoveToEx(hdc, nWidth/2, nHeight/2, &Point);
//    LineTo(hdc, 100, 50);

    HDC hdc = BeginPaint(hwnd, &PaintStruct);

    GdiplusStartupInput gdiplusStartupInput;
    gdiplusStartupInput.GdiplusVersion = 1;
    gdiplusStartupInput.DebugEventCallback = NULL;
    gdiplusStartupInput.SuppressBackgroundThread = FALSE;
    gdiplusStartupInput.SuppressExternalCodecs = FALSE;
    GdiplusStartup(&GdiplusToken, &gdiplusStartupInput, NULL);

//    GdipCreateFromHWND(hwnd, &graphics);
    GpStatus status = GdipCreateFromHDC(hdc, &graphics);
    status = GdipCreatePen1(color, width, unit, &pen);


    PointF Point1 = {.X=0, .Y=10};
    PointF Point2 = {.X=200, .Y=10};
    GpLineGradient * lineGrBrush;
    GdipCreateLineBrush(&Point1, &Point2
            , uiToolkit_ARGB(255, 255, 0, 0)
            , uiToolkit_ARGB(255, 0, 0, 255)
            , WrapModeTile
            , &lineGrBrush
    );
    GpPen * lineGrPen = NULL;
    GdipCreatePen2(lineGrBrush, 1.0f, unit, &lineGrPen);
    GdipDrawLine(graphics, lineGrPen, 0, 10, 200, 10);

    GdipFillEllipse(graphics, lineGrBrush, 0, 30, 200, 100);
    GdipFillRectangle(graphics, lineGrBrush, 0, 155, 500, 30);

    GdipDeletePen(lineGrPen);

    GdipDeletePen(pen);
    GdipDeleteGraphics(graphics);
    GdiplusShutdown(GdiplusToken);

    EndPaint(hwnd, &PaintStruct);
}

void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags){
    RECT Rect;
    POINT Point;
    GpPen* pen = NULL;
    GpGraphics* graphics = NULL;
    GpLineGradient *gpLineGradient = NULL;

    ARGB color = uiToolkit_ARGB(0xFF, 0x00, 0xff, 0x00);
    GpUnit unit = UnitPixel;
    REAL width = 1.0f;

    GetClientRect(hwnd, &Rect);
    int nWidth = Rect.right-Rect.left;
    int nHeight = Rect.bottom - Rect.top;

    HDC hdc = GetDC(hwnd);
    GdiplusStartupInput gdiplusStartupInput;
    gdiplusStartupInput.GdiplusVersion = 1;
    gdiplusStartupInput.DebugEventCallback = NULL;
    gdiplusStartupInput.SuppressBackgroundThread = FALSE;
    gdiplusStartupInput.SuppressExternalCodecs = FALSE;
    GdiplusStartup(&GdiplusToken, &gdiplusStartupInput, NULL);

//    GdipCreateFromHWND(hwnd, &graphics);
    GpStatus status = GdipCreateFromHDC(hdc, &graphics);


    PointF Point1 = {.X=0, .Y=10};
    PointF Point2 = {.X=200, .Y=10};
    GpLineGradient * lineGrBrush;
    GdipCreateLineBrush(&Point1, &Point2
            , uiToolkit_ARGB(255, 255, 0, 0)
            , uiToolkit_ARGB(255, 0, 0, 255)
            , WrapModeTile
            , &lineGrBrush
    );
    GpPen * lineGrPen = NULL;
    GdipCreatePen2(lineGrBrush, 1.0f, unit, &lineGrPen);
    GdipDrawLine(graphics, lineGrPen, nWidth/2, nHeight/2, x, y);


    GdipDeletePen(pen);
    GdipDeleteGraphics(graphics);
    GdiplusShutdown(GdiplusToken);

    ReleaseDC(hwnd, hdc);
}

/* -------------------------------------------------------------------------------------------------------------- */
/*  */
static LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam){
    switch (Message) {
        HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
        HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
        HANDLE_MSG(hwnd, WM_MOUSEMOVE, OnMouseMove);
        default:
            return DefWindowProc(hwnd, Message, wParam, lParam);
    }
}


static BOOL Register(HINSTANCE hInstance){
    WNDCLASS WndClass;
    memset(&WndClass, 0, sizeof(WndClass));
    WndClass.style = CS_VREDRAW|CS_HREDRAW;
    WndClass.hInstance = hInstance;
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = MAIN_CLASSNAME;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    WndClass.cbWndExtra = 0;
    WndClass.cbClsExtra = 0;
    WndClass.lpfnWndProc = WndProc;

    return RegisterClass(&WndClass)!=0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow){
    MSG Msg;
    if(!hPrevInstance){
        if(!Register(hInstance)){
            return FALSE;
        }
    }

    int nScreenX = GetSystemMetrics(SM_CXSCREEN);
    int nScreenY = GetSystemMetrics(SM_CYSCREEN);
    int nWidth = 600;
    int nHeight = 600;

    int X = (nScreenX-nWidth)/2;
    int Y = (nScreenY-nHeight)/2;

    HWND Main_HWND = CreateWindowA(MAIN_CLASSNAME, "Main", WS_OVERLAPPEDWINDOW
                                   , X, Y, nWidth, nHeight
                                   , NULL   /* hWndParent */
                                   , NULL   /* hMenu */
                                   , hInstance
                                   , NULL   /* lParam */
                                   );

    ShowWindow(Main_HWND, nCmdShow);
    UpdateWindow(Main_HWND);

    while(GetMessage(&Msg, NULL, 0, 0)){
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    return Msg.wParam;
}