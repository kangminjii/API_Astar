// API_Astar.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "API_Astar.h"

#include <list>
#include <string>
using namespace std;


// >> 
typedef struct node
{
    POINT location; // 중심점
    node* neighbor[8];

    int Gvalue = 0;
    int Hvalue = 0;
    int Fvalue = Gvalue + Hvalue;
};

// 탐색 노드 관리 리스트
list<node*> open;
list<node*> closed;

// 마우스 위치
static POINT mousePos[2];

// 맵 크기
const int MapSize = 80;

// 함수
void AStar();
void PaintGrid(HDC hdc);
TCHAR* intToTCHAR(int value);

// <<
#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_APIASTAR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_APIASTAR));

    MSG msg;

    // 기본 메시지 루프입니다:
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
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APIASTAR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_APIASTAR);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 1000, 800, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //static bool isClicked = FALSE;
    static int clickCount = 0;

    switch (message)
    {
    case WM_LBUTTONDOWN:
    {
        /*if (clickCount == 0)
        {
            clickCount++;
            mousePos[0].x = LOWORD(lParam);
            mousePos[0].y = HIWORD(lParam);
        }
        else if (clickCount == 1)
        {
            clickCount++;
            mousePos[1].x = LOWORD(lParam);
            mousePos[1].y = HIWORD(lParam);
        }
        */

        InvalidateRect(hWnd, NULL, TRUE);
    }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        //if (clickCount >= 2)
        {
            AStar();
            PaintGrid(hdc);
        }

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

// 정보 대화 상자의 메시지 처리기입니다.
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



void AStar()
{
    // 왼쪽 대각선부터 시계 방향
    int xDir[8] = { -1,0,1,1,1,0,-1,-1 }; 
    int yDir[8] = { -1,-1,-1,0,1,1,1,0 };
    int cost[8] = { 14,10,14,10,14,10,14,10 };

    // 시작과 도착 노드 설정
    node* source = new node; 
    node* dest = new node;
    
    //source->location = mousePos[0];
    source->location = { 200, 200 };
    source->Gvalue = 0;
    source->Hvalue = 42; // 목적지 임시로 설정
    source->Fvalue = source->Gvalue + source->Hvalue;
    
    //dest->location = mousePos[1];
    dest->location = { 200,200 };
    dest->Gvalue = 42;  // 시작 지점 임시로 설정
    dest->Hvalue = 0; 
    dest->Fvalue = dest->Gvalue + dest->Hvalue;

    // 시작 노드의 neighbor 설정
    for (int i = 0; i < 8; i++)
    {
        node* temp = new node;
        temp->location.x = source->location.x + (xDir[i] * MapSize);
        temp->location.y = source->location.y + (yDir[i] * MapSize);
        temp->Gvalue += cost[i];
        //temp->Hvalue =  ; // ?
        temp->Fvalue = temp->Gvalue + temp->Hvalue;
        source->neighbor[i] = temp;
        open.push_back(source->neighbor[i]);
    }

    // 시작 노드 push
    open.push_back(source);

    while (1)
    {
        // 변수 설정
        node* current = new node;
        list<node*>::iterator itC;
        list<node*>::iterator itO;
        int minF = 0; // F값 중 최소
        list<node*>::iterator temp;

        if (open.size() == 1)
            current = source;
        else
        {
            // F값이 제일 작은 게 current 노드가 됨
            for (itO = closed.begin(); itO != closed.end(); itO++)
            {
                if ((*itO)->Fvalue < minF)
                    minF = (*itO)->Fvalue;
                
                current = *itO;
                temp = itO;
            }

            open.erase(temp); // F값 중 제일 작은 걸 삭제
        }

        closed.push_back(current);

        if (current == dest)
            break;
       

        for (int i = 0; i < 8; i++)
        {
            // closed에 neighbor가 들어갔는지 확인
            int check1 = 0;
            for (itC = closed.begin(); itC != closed.end(); itC++)
            {
                if (current->neighbor[i] == (*itC))
                    check1 = 1;
            }

            if (check1 == 1)  
                continue;
            

            // open에 neighbor가 들어갔는지 확인
            int check2 = 0;
            for (itO = closed.begin(); itO != closed.end(); itO++)
            {
                if (current->neighbor[i] == (*itO))
                    check2 = 1;
            }

            if (current->neighbor[i] || check2 == 1)
            {
                //current->neighbor[i]->Fvalue = ;
            }


        }

    }

}

void PaintGrid(HDC hdc)
{
    // 사각형(맵)
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
            Rectangle(hdc, MapSize * j, MapSize * i, MapSize * (j+1), MapSize * (i+1));
    }
    
    //list<POINT*>::iterator it;

    //// 숫자
    // 시작 노드의 G, H, F
    TCHAR* gValue = intToTCHAR(open.front()->Gvalue);
    TCHAR* hValue = intToTCHAR(open.front()->Hvalue);
    TCHAR* fValue = intToTCHAR(open.front()->Fvalue);

    // 시작 노드 숫자
    TextOut(hdc, open.front()->location.x - 30, open.front()->location.y - 20, gValue, _tcslen(gValue)); // G
    TextOut(hdc, open.front()->location.x + 10, open.front()->location.y - 20, hValue, _tcslen(hValue)); // H
    TextOut(hdc, open.front()->location.x - 10, open.front()->location.y + 10, fValue, _tcslen(fValue)); // F
    
    // neighbor의 G, H, F
    for (int i = 0; i < 8; i++)
    {
        TCHAR* gValue = intToTCHAR(open.front()->neighbor[i]->Gvalue);
        TCHAR* hValue = intToTCHAR(open.front()->neighbor[i]->Hvalue);
        TCHAR* fValue = intToTCHAR(open.front()->neighbor[i]->Fvalue);

        TextOut(hdc, open.front()->neighbor[i]->location.x - 30, open.front()->neighbor[i]->location.y - 20, gValue, _tcslen(gValue)); // G
        TextOut(hdc, open.front()->neighbor[i]->location.x + 10, open.front()->neighbor[i]->location.y - 20, hValue, _tcslen(hValue)); // H
        TextOut(hdc, open.front()->neighbor[i]->location.x - 10, open.front()->neighbor[i]->location.y + 10, fValue, _tcslen(fValue)); // F
    }
}

TCHAR* intToTCHAR(int value)
{
    // int -> string -> TCHAR
    string sValue = to_string(value);
    TCHAR* tValue = new TCHAR[sValue.size() + 1];
    tValue[sValue.size()] = 0;
    copy(sValue.begin(), sValue.end(), tValue);

    return tValue;
}