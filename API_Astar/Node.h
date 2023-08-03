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
    POINT center; // �߽���

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

    // ��� ��ġ üũ
    bool InsideRect(POINT mouse);

    // ��ü Ʋ �׸��� + ��ĥ
    void Draw(HDC hdc);
    void Paint(HDC hdc);
};

// Ž�� ��� ���� ����Ʈ

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

    // ��� �귯�� ����
    switch (PaintStat)
    {
    case StartNode:
    {
        DeleteObject(hBrush); // �����ڿ��� ������ �귯�� ����
        hBrush = CreateSolidBrush(RGB(255, 255, 0));  // ��ĥ�� ���� ����
        oldBrush = (HBRUSH)SelectObject(hdc, hBrush); // ���� �귯���� �ٸ� ���� ��ȯ
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

    Draw(hdc);   // ������ �귯���� ���� �׸���(+��ĥ)
    SelectObject(hdc, oldBrush); // �ٽ� ������ �귯���� ���ư���
}

// priority queue�� ���� ������ �����ε�
bool operator< (const Node& node1, const Node& node2)
{
    return node1.getF() > node2.getF();
}
bool operator> (const Node& node1, const Node& node2)
{
    return node1.getF() < node2.getF();
}