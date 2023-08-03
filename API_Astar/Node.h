#pragma once
#include <vector>
#include <string>
#include <iostream>
using namespace std;

#define COLUMN 10
#define ROW 11
const int radius = 30;
enum Information { StartNode, Obstacle, EndNode, OpenNode, CloseNode, None };


class Node
{
private:
    POINT center; // 중심점

    int G;  int H;  int F;

    HBRUSH hBrush;
    Information PaintStat;

    Node* parent;

    // index
    POINT Index;

public:

    Node();
    Node(int i, int j);
    ~Node();

    void setCenter(POINT cen) { center = cen; }
    void setG(int g) { G = g; }
    void setH(int h) { H = h; }
    void setF() { F = getG() + getH(); }

    void setPaintStat(Information status) { PaintStat = status; }
    void setParent(Node* n) { parent = n; }

    int getCenterX() { return center.x; }
    int getCenterY() { return center.y; }
    int getG() { return G; }
    int getH() { return H; }
    int getF() const { return F; }

    int getPaintStat() { return PaintStat; }
    Node* getParent() { return parent; }
    POINT getIndex() { return Index; }

    // 노드 위치 체크
    bool InsideRect(POINT mouse);

    // 전체 틀 그리기 + 색칠
    void Draw(HDC hdc);
    void Paint(HDC hdc);
};

// 탐색 노드 관리 리스트

Node::Node()
{
    center = { 60, 60 };
    G = H = F = 0;
    PaintStat = None;
    hBrush = CreateSolidBrush(RGB(255, 255, 255));
    parent = nullptr;
}
Node::Node(int i, int j)
{
    center = { 60, 60 };
    G = H = F = 0;
    PaintStat = None;
    hBrush = CreateSolidBrush(RGB(255, 255, 255));
    parent = nullptr;
    Index = { i, j };
}
Node::~Node()
{
    DeleteObject(hBrush);
}

bool Node::InsideRect(POINT mouse)
{
    if (center.x - radius < mouse.x && mouse.x < center.x + radius)
    {
        if (center.y - radius < mouse.y && mouse.y < center.y + radius)
            return TRUE;
    }
    return FALSE;
}

void Node::Draw(HDC hdc)
{
    Rectangle(hdc, center.x - radius, center.y - radius, center.x + radius, center.y + radius);
}

void Node::Paint(HDC hdc)
{
    HBRUSH oldBrush = NULL;

    // 노드 브러쉬 설정
    switch (PaintStat)
    {
    case StartNode:
    {
        DeleteObject(hBrush); // 생성자에서 생성한 브러쉬 삭제
        hBrush = CreateSolidBrush(RGB(255, 255, 0));  // 색칠할 색깔 설정
        oldBrush = (HBRUSH)SelectObject(hdc, hBrush); // 쓰던 브러쉬를 다른 곳에 반환
    }
    break;
    case Obstacle:
    {
        DeleteObject(hBrush);
        hBrush = CreateSolidBrush(RGB(0, 0, 0));
        oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    }
    break;
    case EndNode:
    {
        DeleteObject(hBrush);
        hBrush = CreateSolidBrush(RGB(255, 0, 0));
        oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    }
    break;
    case OpenNode:
    {
        DeleteObject(hBrush);
        hBrush = CreateSolidBrush(RGB(255, 128, 0));
        oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    }
    break;
    case CloseNode:
    {
        DeleteObject(hBrush);
        hBrush = CreateSolidBrush(RGB(128, 128, 0));
        oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    }
    break;
    default:
        break;
    }

    Draw(hdc);   // 설정된 브러쉬로 도형 그리기(+색칠)
    SelectObject(hdc, oldBrush); // 다시 원래의 브러쉬로 돌아가기
}

// priority queue를 위한 연산자 오버로딩
bool operator< (const Node& node1, const Node& node2)
{
    return node1.getF() > node2.getF();
}
bool operator> (const Node& node1, const Node& node2)
{
    return node1.getF() < node2.getF();
}