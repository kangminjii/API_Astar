#pragma once
#include <list>
#include <string>
using namespace std;

// ���콺 ��ġ
static POINT mousePos[2];
// �� ũ��
const int MapSize = 80;

class Node
{
private:
    //const int neighborSize = 8;
    POINT location; // �߽���
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

// Ž�� ��� ���� ����Ʈ
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
    // ���� �밢������ �ð� ����
    int xDir[8] = { -1,0,1,1,1,0,-1,-1 };
    int yDir[8] = { -1,-1,-1,0,1,1,1,0 };
    int cost[8] = { 14,10,14,10,14,10,14,10 };

    // ���۰� ���� ��� ����
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

    // ���� ����� neighbor ����
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

    // ���� ��� push
    open.push_back(&source);

    //while (1)
    //{
    //    break;
    //    // ���� ����
    //    Node current;
    //    list<Node*>::iterator itC;
    //    list<Node*>::iterator itO;
    //    list<Node*>::iterator temp;
    //    list<Node*>::iterator curNeigh;

    //    int minF = 0; // F�� �� �ּ�

    //    if (open.size() == 1)
    //        current = source;
    //    else
    //    {
    //        // F���� ���� ���� �� current ��尡 ��
    //        for (itO = closed.begin(); itO != closed.end(); itO++)
    //        {
    //            if ((*itO)->getF() < minF)
    //                minF = (*itO)->getF();

    //            //current = (*itO);
    //            temp = itO;
    //        }
    //        open.erase(temp); // F�� �� ���� ���� �� ����
    //    }

    //    closed.push_back(&current);

    //    //if (current == dest)
    //        break;
    //    
    //    for (curNeigh = current.neighbor.begin(); curNeigh != current.neighbor.end(); curNeigh++)
    //    {
    //        // closed�� neighbor�� ������ Ȯ��
    //        int check1 = 0;
    //        for (itC = closed.begin(); itC != closed.end(); itC++)
    //        {
    //            if ((*curNeigh) == (*itC))
    //                check1 = 1;
    //        }

    //        if (check1 == 1)
    //            continue;

    //        // open�� neighbor�� ������ Ȯ��
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
    // �簢��(��)
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
            Rectangle(hdc, MapSize * j, MapSize * i, MapSize * (j + 1), MapSize * (i + 1));
    }

    //list<POINT*>::iterator it;

    //// ����
    // ���� ����� G, H, F
    TCHAR buffer[20];
    wsprintf(buffer, TEXT("%d"), open.front()->getG());
    TextOut(hdc, open.front()->location.x - 30, open.front()->location.y - 20, buffer, lstrlen(buffer));
    wsprintf(buffer, TEXT("%d"), open.front()->getH());
    TextOut(hdc, open.front()->location.x + 10, open.front()->location.y - 20, buffer, lstrlen(buffer));
    wsprintf(buffer, TEXT("%d"), open.front()->getF());
    TextOut(hdc, open.front()->location.x - 10, open.front()->location.y + 10, buffer, lstrlen(buffer));
   
    list<Node*>::iterator curNeigh1;

    // neighbor�� G, H, F
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