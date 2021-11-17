// Assignment2.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Assignment2.h"
//#include <time.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
int counter = 0;
int timercounter = 0;
bool carLeftCont = false;
bool carTopCont = false;
const int maxNumCars = 8;
//int positions[maxNumCars + 1][2] = { {5,245},{65,245} };
int cars[2][maxNumCars + 1]; // biler i 0 retning og 1 retning
int x_2 = 50; //bredde
int y_2 = 20; //høyde
int pn = 0;
int pw = 0;
char dialogText[60];


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Probablity(HWND, UINT, WPARAM, LPARAM);
void                makeTrafficLigths(HDC, int, int, int);
void                MakeRoad(HDC, HWND);
void                makeCars(HDC);
void                updatePositions();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ASSIGNMENT2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASSIGNMENT2));

    MSG msg;

    // Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASSIGNMENT2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ASSIGNMENT2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   SetTimer(hWnd, 1, 100, NULL);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    /*
    case WM_LBUTTONDOWN:
        carLeftCont = true;
        
        InvalidateRect(0, 0, 0);
        break;
    case WM_RBUTTONDOWN:
        carTopCont = true;

        InvalidateRect(0, 0, 0);
        break;
      */
    case WM_LBUTTONDOWN:
        DialogBox(hInst, MAKEINTRESOURCE(IDD_PROB), hWnd, Probablity);
        InvalidateRect(hWnd, 0, 0);
        break;
  
    case WM_KEYDOWN:
        if (wParam == VK_LEFT) {
            pw -= 10;
        }
        else if (wParam == VK_RIGHT) {
            pw += 10;
        }

        if (wParam == VK_UP) {
            pn += 10;
        }
        else if (wParam == VK_DOWN) {
            pn -= 10;
        }

       // dialogText = "North: "// + pn; //", West: " + pw;
        break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
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
        HDC hdc = BeginPaint(hWnd, &ps);

        //TextOut(hdc, 200, 200, dialogText, lstrlen(dialogText) + 1);

        MakeRoad(hdc, hWnd);
        makeTrafficLigths(hdc, 600, 50, 0);
        makeTrafficLigths(hdc, 400, 350, 2);
       
        makeCars(hdc);


        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_TIMER:
        timercounter++;
        timercounter %= 50;
        if (timercounter == 0)
        {
            counter++;
            counter %= 4;
            if (counter == 1 || counter == 3) {
                timercounter = 20;
            }
        }
        carLeftCont = (rand() % 100) < pw;
        carTopCont = (rand() % 100) < pn;
        updatePositions();
        InvalidateRect(hWnd, NULL, FALSE);
        UpdateWindow(hWnd);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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

INT_PTR CALLBACK Probablity(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            pw = GetDlgItemInt(hDlg, IDC_pwest, NULL, true);
            pn = GetDlgItemInt(hDlg, IDC_PNORTH, NULL, true);
            
            
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void updatePositions() {
    //const int maxNumCars = 3; 2
    //int positions[maxNumCars + 1][2] = { {5,245},{60,245} };
    for (int i = 0; i < 2; i++) { //retningene 0 = vannrett, 1=loddrett
        
        int speed = 5;
        int pos = cars[i][maxNumCars];
        int current = (pos - 1 + maxNumCars) % maxNumCars;

        
        if (cars[i][current] >= 0) {
            if (cars[i][pos] == 0) {
                if (carLeftCont && i==0) { //fra venstre
                    cars[i][pos] = -60; //startposisjon
                    cars[i][pos] += cars[i][pos] % speed == 0 ? 1 : 0;
                    pos++;
                    cars[i][maxNumCars] = pos % maxNumCars;
                    carLeftCont = false;

                } else if (carTopCont && i == 1) { //fra top
                    cars[i][pos] = -60;
                    cars[i][pos] += cars[i][pos] % speed == 0 ? 1 : 0;
                    pos++;
                    cars[i][maxNumCars] = pos % maxNumCars;
                    carTopCont = false;
                }
            }
        }
        

        for (int j = pos; j < pos + maxNumCars; j++) {
            int car = j % maxNumCars;
            int previous = (car - 1 + maxNumCars) % maxNumCars; 

            if (cars[i][car] == 0) { 
                //bilen har ikke startet
            }
            else if (counter != 0 && i == 0 && cars[i][car] < (500 - x_2) && cars[i][car] + speed > (500 - x_2)) {
                //bilene fra vest stopper for rødt lys
            }
            else if (counter != 2 && i == 1 && cars[i][car] < (240 - x_2) && cars[i][car] + speed > (240 - x_2)) {
                //bilene fra nord stopper
            }
            else if (j == pos || cars[i][previous] == 0) { // første bil starter alltid
                cars[i][car] += speed;
            }
            else if (cars[i][previous] - (cars[i][car] + x_2) >= speed +20) {
                cars[i][car] += speed;
            }
            else if (cars[i][previous] - (cars[i][car] + x_2) < speed +20) {
                //bilen stopper fordi den forran stoppet
               cars[i][car] += cars[i][previous] - (cars[i][car] + x_2 ) - 20;
            }
            
            if (cars[i][car] > 1150 + x_2) {
                cars[i][car] = 0;
            }

        }
       
    }
}

void makeTrafficLigths (HDC hdc, int x, int y, int start) {
    int traffic = (start + counter) % 4;

    HBRUSH hBrush[5];
    HGDIOBJ hOrg;

    hBrush[0] = CreateSolidBrush(RGB(192, 192, 192)); //gray
    hBrush[1] = CreateSolidBrush(RGB(0, 0, 0)); //black
    hBrush[2] = CreateSolidBrush(RGB(255, 0, 0)); //red
    hBrush[3] = CreateSolidBrush(RGB(255, 255, 0)); //yellow
    hBrush[4] = CreateSolidBrush(RGB(0, 225, 0)); //green

    hOrg = SelectObject(hdc, hBrush[1]);
    Rectangle(hdc, x, y, (x + 50), (y + 150));
    DeleteObject(hBrush);

    //rød
    if (traffic < 2) {
        SelectObject(hdc, hBrush[2]);
    }
    else {
        SelectObject(hdc, hBrush[0]);
    }
    Ellipse(hdc, x+5, y + 5, x + 45, y + 45);

    //gul
    if (traffic == 1 || traffic == 3) {
        SelectObject(hdc, hBrush[3]);
    }
    else {
        SelectObject(hdc, hBrush[0]);
    }
    Ellipse(hdc, x + 5, y + 55, x + 45, y + 95);

    //grønn
    if (traffic == 2) {
        SelectObject(hdc, hBrush[4]);
    }
    else {
        SelectObject(hdc, hBrush[0]);
    }
    Ellipse(hdc, x + 5, y+105, x + 45, y+145);



    SelectObject(hdc, hOrg);
    for (int i = 0; i < 5; i++) {
        DeleteObject(hBrush[i]);
    }
}

void MakeRoad(HDC hdc, HWND hwnd) {
    HBRUSH hBrush;
    HGDIOBJ hOrg;
    HPEN hPen;
    
    /*
    RECT rect;
    int width;
    int height;
    if (GetWindowRect(hwnd, &rect))
    {
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;
    }

    Rectangle(hdc, 0, height/2 -50, width, height/2 +50);
    Rectangle(hdc, width / 2 - 50, 0, width / 2 + 50, height);
    */
    hBrush = CreateSolidBrush(RGB(192, 192, 192)); //gray
    hOrg = SelectObject(hdc, hBrush);

    SelectObject(hdc, hBrush);
    Rectangle(hdc, 0, 240, 1150, 300); //vannrett
    Rectangle(hdc, 500, 0, 560, 600); //Loddrett

    hPen = CreatePen(PS_DASH | PS_COSMETIC, 2, RGB(240, 240, 0));
    hOrg = SelectObject(hdc, hPen);

    MoveToEx(hdc, 0, 270, 0); //vannrett
    LineTo(hdc, 1150, 270);
    MoveToEx(hdc, 530, 0, 0); //loddrett
    LineTo(hdc, 530, 600);

    Rectangle(hdc, 500, 240, 560, 300);

    SelectObject(hdc, hOrg);
    DeleteObject(hBrush);
    DeleteObject(hPen);

}

void makeCars(HDC hdc) {
    HBRUSH hBrush[5];
    HGDIOBJ hOrg;
    //5, 245

    hBrush[0] = CreateSolidBrush(RGB(0, 200, 0)); //green
    hBrush[1] = CreateSolidBrush(RGB(0, 0, 0)); //black
    hBrush[2] = CreateSolidBrush(RGB(255, 0, 0)); //red
    hBrush[3] = CreateSolidBrush(RGB(255, 255, 0)); //yellow
      

    hOrg = SelectObject(hdc, hBrush[2]);
    for (int j = 0; j < 5; j++) {
        
        int x = cars[0][j];
        int y = 245;
        if (x != 0) {
            Rectangle(hdc, x, y, x + x_2, y + y_2);
        }
    }

    for (int j = 0; j < 5; j++) {

        int a = 535;
        int b = cars[1][j];
        if (b != 0) {
            Rectangle(hdc, a, b, a + y_2, b + x_2);
        }
    }

    DeleteObject(hBrush);

    SelectObject(hdc, hOrg);
    for (int i = 0; i < 5; i++) {
        DeleteObject(hBrush[i]);
    }
}

