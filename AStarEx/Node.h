#pragma once
#include <iostream>

struct Node
{
    Node(int _x, int _y, bool wkb=true){
        x = _x;
        y = _y;
		f = 0.0f;
		g = 0.0f;
		version = 1;
		parent = NULL;
		walkable = wkb;
    }
	Node(){}
	int x,y;
    float f,g;
    Node* parent;
    int version;
	bool walkable;
};
inline bool operator == (const Node& a, const Node& b)
{
	return a.x == b.x && a.y == b.y && a.version == b.version  ;
}
inline bool operator != (const Node& a, const Node& b)
{
	return a.x != b.x || a.y != b.y || a.version != b.version  ;
}
