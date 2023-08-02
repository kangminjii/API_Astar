// API_Astar.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "API_Astar.h"
#include "Node.h"
#include <iostream>
#include <cmath>
#include <queue>


#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
#endif

// >> : BITMAP
#pragma comment(lib, "msimg32.lib")

//HBITMAP hBackImage;
//BITMAP  bitBack;
//
//HBITMAP hFrontImage;
//BITMAP  bitFront;
//
//RECT rectView;
//
//HBITMAP hDoubleBufferImage;
//void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc);
// << : BITMAP



// painting
vector<vector<Node*>> node;
POINT mousePos;
Information status;

// A*
priority_queue<Node, vector<Node>, less<vector<Node>::value_type>> open;
vector<Node> closed;
void Astar();

POINT arrayIndex1, arrayIndex2;
void InitNode();
bool isEnd = FALSE;



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
    if (!InitInstance(hInstance, nCmdShow))
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

    return (int)msg.wParam;
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

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APIASTAR));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_APIASTAR);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
        CW_USEDEFAULT, 0, 1000, 900, nullptr, nullptr, hInstance, nullptr);

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
    switch (message)
    {
    case WM_CREATE:
    {
        InitNode();

        // 노드 위치 설정
        for (int i = 0; i < ROW; i++)
        {
            for (int j = 0; j < COLUMN; j++)
                node[i][j]->setCenter({ 60 + 60 * j, 60 + 60 * i });
        }
    }
    break;
    case WM_KEYDOWN:
    {
        // 키보드 입력으로 메뉴 구분 (1. 시작노드 설정  2. 장애물 설정  3. 끝노드 설정  4. 시작)
        if (wParam == '1')
            status = StartNode;
        else if (wParam == '2')
            status = Obstacle;
        else if (wParam == '3')
            status = EndNode;
        else if (wParam == '4')
            status = Start;

        // A* 실행
        if (wParam == VK_SPACE)
            Astar();

        InvalidateRect(hWnd, NULL, TRUE);
    }
    break;
    case WM_LBUTTONDOWN:
    {
        // 마우스 위치로 클릭 판별 및 노드 저장
        mousePos.x = LOWORD(lParam);
        mousePos.y = HIWORD(lParam);

        for (int i = 0; i < ROW; i++)
        {
            for (int j = 0; j < COLUMN; j++)
            {
                if (node[i][j]->InsideRect(mousePos))  // 클릭을 했다
                {
                    node[i][j]->setPaintStat(status);

                    // 시작 점 넣기
                    if (status == StartNode)
                    {
                        arrayIndex1 = node[i][j]->getIndex();
                    }
                    else if (status == EndNode)
                    {
                        arrayIndex2 = node[i][j]->getIndex();
                    }
                }
            }
        }
        InvalidateRect(hWnd, NULL, TRUE);
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);


        // 전체 맵, 숫자 그리기
        for (int i = 0; i < ROW; i++)
        {
            for (int j = 0; j < COLUMN; j++)
            {
                //cout << "status = " << status << endl;
                node[i][j]->Paint(hdc);

                TCHAR buffer[10];
                wsprintf(buffer, TEXT("%d"), node[i][j]->getG());
                TextOut(hdc, node[i][j]->getCenterX() - 20, node[i][j]->getCenterY() - 15, buffer, lstrlen(buffer));
                wsprintf(buffer, TEXT("%d"), node[i][j]->getH());
                TextOut(hdc, node[i][j]->getCenterX() + 5, node[i][j]->getCenterY() - 15, buffer, lstrlen(buffer));
                wsprintf(buffer, TEXT("%d"), node[i][j]->getF());
                TextOut(hdc, node[i][j]->getCenterX() - 5, node[i][j]->getCenterY() + 5, buffer, lstrlen(buffer));
            }
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


void InitNode()
{
    for (int i = 0; i < ROW; i++)
    {
        vector<Node*> temp;

        for (int j = 0; j < COLUMN; j++)
        {
            temp.push_back(new Node(i, j));
        }
        node.push_back(temp);
    }

}

void Astar()
{
    // H값 저장
    if (status == EndNode)
    {
        for (int i = 0; i < ROW; i++)
        {
            for (int j = 0; j < COLUMN; j++)
            {
                int width = abs(arrayIndex2.x - i);
                int height = abs(arrayIndex2.y - j);
                int max = (width > height) ? width : height;
                int min = (width > height) ? height : width;
                node[i][j]->setH(min * 14 + (max - min) * 10);
                node[i][j]->setF();
            }
        }
    }

    // 왼쪽 대각선부터 시계 방향
    int xDir[8] = { -1,0,1,1,1,0,-1,-1 };
    int yDir[8] = { -1,-1,-1,0,1,1,1,0 };
    int cost[8] = { 14,10,14,10,14,10,14,10 };

    //printf("start node pushed, open.size() : %d\n",open.size());
    

    // 탐색 시작
    for (int i = 0; i < 8; i++)
    {
        // 시작점 주변 neighbor의 G값과 부모 저장
        if ((arrayIndex1.x + xDir[i] >= 0) && (arrayIndex1.x + xDir[i] < ROW))
        {
            if ((arrayIndex1.y + yDir[i] >= 0) && (arrayIndex1.y + yDir[i] < COLUMN))
            {
                Node* tempNode = node[arrayIndex1.x + xDir[i]][arrayIndex1.y + yDir[i]];
                tempNode->setG(cost[i]);
                tempNode->setF();
                open.push(*tempNode);
                tempNode->setPaintStat(OpenNode);

                //printf("%d, %d pushed in pq\n", arrayIndex1.x + xDir[i], arrayIndex1.y + yDir[i]);
                tempNode->setParent(node[arrayIndex1.x][arrayIndex1.y]); // parent를 const Node*로 변경
            }
        }  
    }
    closed.push_back(*node[arrayIndex1.x][arrayIndex1.y]);
    //open.pop();
    printf("open.size() : %d \n:", open.size());
   
    
    // 계속 탐색하기
    while (!open.empty() && isEnd == FALSE)
    {
        printf("isend: %d\n", isEnd);
        Node temp = open.top();
        printf("poped node's f val : %d\n", temp.getF());
        closed.push_back(temp);
        open.pop();
        
        printf("isend: %d\n", isEnd);

        for (int i = 0; i < 8; i++)
        {
            // 새로운 노드의 neighbor 비교
            if ((temp.getIndex().x + xDir[i] >= 0) && (temp.getIndex().x + xDir[i] < ROW))
            {
                if ((temp.getIndex().y + yDir[i] >= 0) && (temp.getIndex().y + yDir[i] < COLUMN))
                {
                    bool isClosed = FALSE;

                    Node* neighbor = node[temp.getIndex().x + xDir[i]][temp.getIndex().y + yDir[i]];
                    // closed인가? 
                    for (int j = 0; j < closed.size(); j++)
                    {
                        if (temp.getIndex().x == closed[j].getIndex().x && temp.getIndex().y == closed[j].getIndex().y)
                        {
                            isClosed = TRUE;
                            break;
                        }
                    }


                    if (isClosed)
                        continue;

                    // 장애물인가?
                    if (temp.getPaintStat() == Obstacle)
                        continue;

                    // 종료 조건
                    if (neighbor->getPaintStat() == EndNode)
                    {
                        cout << "찾았따" << endl;
                        isEnd = TRUE;
                        break;
                    }

                    // open에 있는가?
                    if (temp.getPaintStat() != OpenNode)
                    {
                        neighbor->setPaintStat(OpenNode);
                        neighbor->setParent(&temp);
                        open.push(*neighbor);
                        continue;
                    }
                    else 
                    {
                        int neighborG = neighbor->getG();
                        int tempG = temp.getG() + neighborG;
                        if (neighborG <= tempG)
                            continue;
                        else
                        {
                            // G값 갱신
                            neighbor->setG(tempG);
                            neighbor->setParent(&temp);
                            neighbor->setF();
                            open.push(*neighbor);
                        }
                    }
                }
            }
        }
    }
}