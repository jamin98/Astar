#pragma once
#include <iostream>

struct Node
{
    Node(int _x, int _y){
        x = _x;
        y = _y;
		version = 1;
		f = 0.0f;
		g = 0.0f;
		parent = NULL;
    }
	Node(){}
	int x,y;
    float f,g;
    Node* parent;
    int version;
};
inline bool operator == (const Node& a, const Node& b)
{
	return a.x == b.x && a.y == b.y && a.version == b.version  ;
}
inline bool operator != (const Node& a, const Node& b)
{
	return a.x != b.x || a.y != b.y || a.version != b.version  ;
}
