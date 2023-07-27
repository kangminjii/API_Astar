#pragma once
#include <list>
#include <string>
using namespace std;

// 마우스 위치
static POINT mousePos[2];
// 맵 크기
const int MapSize = 80;

class Node
{
private:
    //const int neighborSize = 8;
    POINT location; // 중심점
    list<Node*> neighbor;

    int G;
    int H;
    int F;
public:
    Node();
    ~Node();

    //Node(POINT p, list<Node*> neigh, int g, int h, int f) { location = p; neighbor = neigh; G = g; H = h; F = f; }

    void setG(int g) { G = g; }
    void setH(int h) { H = h; }
    void setF(int f) { F = f; }

    int getG() { return G; }
    int getH() { return H; }
    int getF() { return F; }


    void Astar();
    void PaintGrid(HDC hdc);
};

// 탐색 노드 관리 리스트
list<Node*> open;
list<Node*> closed;

Node::Node()
{
    location = { 0, 0 };
    for (int i = 0; i < 8; i++)
        neighbor.push_back(NULL);
    G = H = F = 0;
}

Node::~Node()
{
}

void Node::Astar()
{
    // 왼쪽 대각선부터 시계 방향
    int xDir[8] = { -1,0,1,1,1,0,-1,-1 };
    int yDir[8] = { -1,-1,-1,0,1,1,1,0 };
    int cost[8] = { 14,10,14,10,14,10,14,10 };

    // 시작과 도착 노드 설정
    Node source, dest;
    source.location = { 200, 200 };
    source.setF(0);
    source.setG(42);
    source.setH(42);

    dest.location = { 400, 400 };
    dest.setF(42);
    dest.setG(0);
    dest.setH(42);

    //source->location = mousePos[0];
    //dest->location = mousePos[1];

    // 시작 노드의 neighbor 설정
    for (int i = 0; i < 8; i++)
    {
        Node* temp = new Node;
       /* temp.location.x = source.location.x + (xDir[i] * MapSize);
        temp.location.y = source.location.y + (yDir[i] * MapSize);
        temp.setG(cost[i]);
        temp.setF(temp.getG() + temp.getH());
        */
        temp->location.x = source.location.x + (xDir[i] * MapSize);
        temp->location.y = source.location.y + (yDir[i] * MapSize);
        temp->setG(cost[i]);
        temp->setF(temp->getG() + temp->getH());
        //temp.setH(); // ?

        source.neighbor.push_back(temp);
    }

    // 시작 노드 push
    open.push_back(&source);

    //while (1)
    //{
    //    break;
    //    // 변수 설정
    //    Node current;
    //    list<Node*>::iterator itC;
    //    list<Node*>::iterator itO;
    //    list<Node*>::iterator temp;
    //    list<Node*>::iterator curNeigh;

    //    int minF = 0; // F값 중 최소

    //    if (open.size() == 1)
    //        current = source;
    //    else
    //    {
    //        // F값이 제일 작은 게 current 노드가 됨
    //        for (itO = closed.begin(); itO != closed.end(); itO++)
    //        {
    //            if ((*itO)->getF() < minF)
    //                minF = (*itO)->getF();

    //            //current = (*itO);
    //            temp = itO;
    //        }
    //        open.erase(temp); // F값 중 제일 작은 걸 삭제
    //    }

    //    closed.push_back(&current);

    //    //if (current == dest)
    //        break;
    //    
    //    for (curNeigh = current.neighbor.begin(); curNeigh != current.neighbor.end(); curNeigh++)
    //    {
    //        // closed에 neighbor가 들어갔는지 확인
    //        int check1 = 0;
    //        for (itC = closed.begin(); itC != closed.end(); itC++)
    //        {
    //            if ((*curNeigh) == (*itC))
    //                check1 = 1;
    //        }

    //        if (check1 == 1)
    //            continue;

    //        // open에 neighbor가 들어갔는지 확인
    //        int check2 = 0;
    //        for (itO = closed.begin(); itO != closed.end(); itO++)
    //        {
    //            if ((*curNeigh) == (*itO))
    //                check2 = 1;
    //        }

    //        if ((*curNeigh) || check2 == 1)
    //        {
    //            //current->neighbor[i]->Fvalue = ;
    //        }

    //    }

    //}

}

void Node::PaintGrid(HDC hdc)
{
    // 사각형(맵)
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
            Rectangle(hdc, MapSize * j, MapSize * i, MapSize * (j + 1), MapSize * (i + 1));
    }

    //list<POINT*>::iterator it;

    //// 숫자
    // 시작 노드의 G, H, F
    TCHAR buffer[20];
    wsprintf(buffer, TEXT("%d"), open.front()->getG());
    TextOut(hdc, open.front()->location.x - 30, open.front()->location.y - 20, buffer, lstrlen(buffer));
    wsprintf(buffer, TEXT("%d"), open.front()->getH());
    TextOut(hdc, open.front()->location.x + 10, open.front()->location.y - 20, buffer, lstrlen(buffer));
    wsprintf(buffer, TEXT("%d"), open.front()->getF());
    TextOut(hdc, open.front()->location.x - 10, open.front()->location.y + 10, buffer, lstrlen(buffer));
   
    list<Node*>::iterator curNeigh1;

    // neighbor의 G, H, F
    for (curNeigh1 = open.front()->neighbor.begin(); curNeigh1 != open.front()->neighbor.end(); curNeigh1++)
    {
        TCHAR buffer[20];
        wsprintf(buffer, TEXT("%d"), (*curNeigh1)->getG());
        TextOut(hdc, (*curNeigh1)->location.x - 30, (*curNeigh1)->location.y - 20, buffer, lstrlen(buffer)); // G
        wsprintf(buffer, TEXT("%d"), (*curNeigh1)->getH());
        TextOut(hdc, (*curNeigh1)->location.x + 10, (*curNeigh1)->location.y - 20, buffer, lstrlen(buffer)); // F
        wsprintf(buffer, TEXT("%d"), (*curNeigh1)->getF());
        TextOut(hdc, (*curNeigh1)->location.x - 10, (*curNeigh1)->location.y + 10, buffer, lstrlen(buffer)); // H
    }
}