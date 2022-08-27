// ScreenShoter.cpp : Definuje vstupní bod pro aplikaci.
//

#include "framework.h"
#include "ScreenShoter.h"

#include <objidl.h>
#include <wingdi.h>
#include <assert.h>
#include <windows.h>
#include <gdiplus.h>
#include <stdio.h>
#include <ShlObj.h>

#define MAX_LOADSTRING 100

#define ID_WindowButton1 10001

// Globální proměnné:
HINSTANCE hInst;                                // aktuální instance
WCHAR szTitle[MAX_LOADSTRING];                  // Text záhlaví
WCHAR szWindowClass[MAX_LOADSTRING];            // název třídy hlavního okna

// Předat dál deklarace funkcí zahrnuté v tomto modulu kódu:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void ScreenShotDesktop();
PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp);
void CreateBMPFile(LPCWSTR pszFile, HBITMAP bitmap);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Sem umístěte kód.

    // Inicializovat globální řetězce
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SCREENSHOTER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Provést inicializaci aplikace:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SCREENSHOTER));

    MSG msg;

    // Hlavní smyčka zpráv:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNKCE: MyRegisterClass()
//
//  ÚČEL: Zaregistruje třídu okna.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SCREENSHOTER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SCREENSHOTER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNKCE: InitInstance(HINSTANCE, int)
//
//   ÚČEL: Uloží popisovač instance a vytvoří hlavní okno.
//
//   KOMENTÁŘE:
//
//        V této funkci uložíme popisovač instance do globální proměnné
//        a vytvoříme a zobrazíme hlavní okno programu.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Uloží popisovač instance do naší globální proměnné.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED, CW_USEDEFAULT, 0, 400, 240, 0, 0, hInstance, nullptr);   

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNKCE: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ÚČEL: Zpracuje zprávy pro hlavní okno.
//
//  WM_COMMAND  - zpracování aplikační nabídky
//  WM_PAINT    - Vykreslení hlavního okna
//  WM_DESTROY  - vystavení zprávy o ukončení a návrat
//
//
LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
    {
        ShowWindow(hWnd, SW_HIDE);
        Sleep(1000);
        ScreenShotDesktop();
        ShowWindow(hWnd, SW_SHOW);

    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analyzovat vybrané možnosti nabídky:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            RECT intro_rect;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Sem přidejte jakýkoliv kód vykreslování, který používá hdc...
            GetClientRect(hWnd, &intro_rect);
            SetTextColor(hdc, RGB(0x00, 0x00, 0x00));
            SetBkMode(hdc, TRANSPARENT);
            intro_rect.top = (intro_rect.bottom - intro_rect.top) / 2 - 10;
            DrawText(hdc, L"CLICK HERE TO DESKTOP SCREENSHOTING!", -1, &intro_rect, DT_CENTER);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Obslužná rutina zprávy pro pole O produktu
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void ScreenShotDesktop()
{
    HWND hwnd = GetDesktopWindow();
    //HWND hwnd = HWND_DESKTOP;
    HDC hdc[2];
    HBITMAP hbmp;
    RECT rect;
    GetClientRect(hwnd, &rect);

    hdc[0] = GetWindowDC(hwnd);
    hbmp = CreateCompatibleBitmap(hdc[0], rect.right, rect.bottom);
    hdc[1] = CreateCompatibleDC(hdc[0]);
    SelectObject(hdc[1], hbmp);

    BitBlt(hdc[1], 0, 0, rect.right, rect.bottom, hdc[0], 0, 0, SRCCOPY);

    CreateBMPFile(L"WinDesktop.bmp", hbmp);

}

PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp)
{
    //BITMAP *bmp = (BITMAP*)malloc(sizeof(BITMAP*));
    BITMAP bmp;
    PBITMAPINFO pbmi;
    WORD cClrBits;

    GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp);

    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);

    if (cClrBits == 1)
        cClrBits = 1;
    else if (cClrBits <= 4)
        cClrBits = 4;
    else if (cClrBits <= 8)
        cClrBits = 8;
    else if (cClrBits <= 16)
        cClrBits = 16;
    else if (cClrBits <= 24)
        cClrBits = 24;
    else
        cClrBits = 32;

    if (cClrBits < 24)
        pbmi = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << cClrBits));
    else
        pbmi = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER));

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth = bmp.bmWidth;
    pbmi->bmiHeader.biHeight = bmp.bmHeight;
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;

    if (cClrBits < 24)
        pbmi->bmiHeader.biClrUsed = (1 << cClrBits);

    pbmi->bmiHeader.biCompression = BI_RGB;
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 32) & ~31) / 8 * pbmi->bmiHeader.biHeight;
    pbmi->bmiHeader.biClrImportant = 0;

    return pbmi;
}

void CreateBMPFile(LPCWSTR pszFile, HBITMAP hBMP)
{
    HANDLE hf;
    BITMAPFILEHEADER hdr;
    PBITMAPINFOHEADER pbih;
    LPBYTE lpBits;
    DWORD dwTotal;
    DWORD cb;
    BYTE* hp;
    DWORD dwTmp;
    PBITMAPINFO pbi;
    HDC hdc;

    hdc = CreateCompatibleDC(GetDC(HWND_DESKTOP));
    SelectObject(hdc, hBMP);
    pbi = CreateBitmapInfoStruct(hBMP);
    pbih = (PBITMAPINFOHEADER)pbi;
    lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

#ifdef _DEBUG
    assert(lpBits);
    assert(GetDIBits(hdc, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS));
#else
    GetDIBits(hdc, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS);
#endif

    /*wchar_t filepath[256];
    wcscpy_s(filepath, L"C:\\Users\\");
    DWORD len = 0;
    GetUserName(NULL, &len);
    wchar_t* username = new wchar_t[len];    
    GetUserName(username, &len);
    wcscat_s(filepath, username);
    
    wcscat_s(filepath, pszFile);*/  

    wchar_t filepath[256];
    SHGetFolderPathW(NULL, CSIDL_MYPICTURES, NULL, SHGFP_TYPE_CURRENT, filepath);
    wcscat_s(filepath, L"\\");
    wcscat_s(filepath, pszFile);

    hf = CreateFile(filepath, GENERIC_READ | GENERIC_WRITE, (DWORD)0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

#ifdef _DEBUG
    assert(hf != INVALID_HANDLE_VALUE);
#endif

    hdr.bfType = 0x4d42;
    hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD) + pbih->biSizeImage);

    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;

    hdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD);

#ifdef _DEBUG
    assert(WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER), (LPDWORD)&dwTmp, NULL));
    assert(WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER), (LPDWORD)&dwTmp, NULL));
#else
    WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER), (LPDWORD)&dwTmp, NULL);
    WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER), (LPDWORD)&dwTmp, NULL);
#endif

    dwTotal = cb = pbih->biSizeImage;
    hp = lpBits;

#ifdef _DEBUG
    assert(WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL));
    assert(CloseHandle(hf));
#else
    WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL);
    CloseHandle(hf);
#endif

    GlobalFree((HGLOBAL)&lpBits);
}