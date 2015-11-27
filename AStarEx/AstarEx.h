#pragma once
#include <deque>
#include <math.h>
#include "Node.h"
#include <algorithm>
#include <vector>
#include <assert.h>
#include <iostream>
#include <string.h>

namespace AStarEx
{
	struct Point
	{
		int x;
		int y;
		Point(int x=0,int y=0):x(x),y(y){}
	};
	inline bool operator == (const Point& a, const Point& b)
	{
		return a.x == b.x && a.y == b.y;
	}

	typedef std::vector<int> IntList;
	typedef std::vector<Point> PointList;
	typedef std::deque<Point> PointQue;

	class BinaryHeap {
	private:
		inline bool m_justMinFun(Node *x, Node *y) {
			return x->f < y->f;
		}
	public:
		std::deque<Node*> a;

		BinaryHeap(){
			a.resize(1);
		}

		void ins(Node *value) {
			int p = a.size();
			a.push_back(value);
			int pp = p >> 1;
			while (p > 1 && m_justMinFun(a[p], a[pp])){
				Node *temp = a[p];
				a[p] = a[pp];
				a[pp] = temp;
				p = pp;
				pp = p >> 1;
			}
		}

		Node *pop() {
			Node *min = a[1];
			a[1] = a[a.size() - 1];
			a.pop_front();
			int p = 1;
			int l = a.size();
			int sp1 = p << 1;
			int sp2 = sp1 + 1;
			while (sp1 < l){
				int minp;
				if (sp2 < l){
					minp = m_justMinFun(a[sp2], a[sp1]) ? sp2 : sp1;
				} else {
					minp = sp1;
				}
				if (m_justMinFun(a[minp], a[p])){
					Node *temp = a[p];
					a[p] = a[minp];
					a[minp] = temp;
					p = minp;
					sp1 = p << 1;
					sp2 = sp1 + 1;
				} else {
					break;
				}
			}
			return min;
		}
	};

	class AstarEx
	{
	public:

		AstarEx();
		~AstarEx();

		void Init(const IntList &Map, int w, int h); //����

		bool findPath(int startX, int startY, int endX, int endY);

		void floyd();

		bool TestPoint(int x, int y, Point& best);

	public:

		Point FindBestPoint(int x, int y);

		bool IsOpen(Point Point)
		{
			if( Point.x < 0 || Point.x >= m_W  ||  Point.y < 0 || Point.y >= m_H )
			{
				return false;
			}
			return nodes[Point.y][Point.x].walkable==true;
		}

		PointQue GetAstarPath()
		{
			return path;
		}

		void setMapPoint(int x, int y, int point);

		bool visual(const Point &n1, const Point &n2);

	private:
		PointList bresenhamNodes(Point p1, Point p2);

		bool floydCrossAble(const Point &n1, const Point &n2);

	private:
		Node** nodes;							//����ÿ������������б�Ľڵ㣬IDΪ������ֵ
		BinaryHeap open;
		PointList ps;
		IntList m_visited;                      //�Ҹ���·�� ����ѷ���
		PointQue m_que;                         //�Ҹ���·���Ѿ��Ҷ���
		int nowVersion;
		int m_W, m_H;                           //��ͼ���
		PointQue path;							//Ѱ����·��
		PointQue floydPath;						//�Ż���·��
	};
}
