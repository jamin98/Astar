/***********************************************************************************/
/* 地图设定：0表示可通过1表示障碍物地图可斜向移动， 但当其倾斜方向的正方向之一有障碍 
/* 物则不可斜向移动，例如向右下移动，如正右方和正下方其中之一有障碍则不可向右下移动。                              
/***********************************************************************************/
#pragma once
#include <memory>
#include <assert.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <deque>

namespace AStars
{
	struct POINT
	{
		int x, y;
	};
	/*
	Python lists have quite a bit more functionality than C++ vectors. vector_indexing_suite defines contains method among others, so your container class has to define operator==.
	*/
	inline bool operator == (const POINT& a, const POINT& b)
	{
		return a.x == b.x && a.y == b.y;
	}

	typedef std::vector<POINT> PointList;
	typedef std::vector<int> IntList;
	typedef std::vector<float> FloatList;
	typedef std::vector<unsigned char> ByteList;
	typedef std::deque<POINT> POS_LIST;

	#define W_H_MOVE       10 //水平或竖直移动一格的花费
	#define SLANTMOVE      14 //斜向移动（沿着对角线移动方向）一格的花费
	#define MAXNODECOUNT   10240 //开放列表可保存的最大节点数


	//用于保存节点信息
	struct NodeInfo 
	{
		int m_NodeID;        //节点ID
		bool m_InOpenList;   //是否在开放列表中，true为在开放列表中，false不在开放列表中
		bool m_InCloseList;  //是否在关闭列表中，true在关闭列表，false不在关闭列表
	};

	//用于保存节点数据
	struct Node
	{
		POINT m_Pos;             //节点坐标
		int m_ScoreF;        //节点路径评分
		int m_ScoreG;        //节点到起始点的路径评分
		int m_FatherID;      //节点的父节点ID
		//bool m_InOpenList;   //是否在开放列表中，true为在开放列表中，false不在开放列表中
		//bool m_InCloseList;  //是否在关闭列表中，true在关闭列表，false不在关闭列表
	};

	

class AStar
{
public:
	AStar();
	~AStar();

	void Init(IntList Map, int w, int h); //加载

	bool TestPoint(int x, int y, POINT& best);

	//功能:返回路线附近的可走区域
	//参数 目的地 x y
	//返回: 失败返回 0, 0 成功返回实际可走坐标
	POINT FindBestPoint(int x,int y);

	//功能：寻路
	//参数：xBegin——起始点X坐标 yBegin——起始点Y坐标
	//      xEnd——终点X坐标     yEnd——终点Y坐标
	//返回:
	void Find(int xBegin, int yBegin, int xEnd, int yEnd); 

	//功能：为节点作加入开放列表标记
	//参数：Point——节点坐标 ScoreF——节点路径评分 
	//      ScoreG——节点到起点路径评分 FatherID——节点的父节点ID
	//返回：
	void OpenNote(POINT Point, int ScoreF, int ScoreG,int FatherID);

	//功能：为节点作加入关闭列表标记
	//参数：ID——要做标记的节点ID
	//返回：
	void CloseNote(int ID);
	
	//功能：新加入开放列表或修改路径评分后对二叉堆进行排序，使得F值最小的节点的ID在最前面
	//参数：Index——修改或新加入节点ID在m_OpenList的索引
	//返回：
	void HeapAheadSort(int Index);
	
	//功能：从开放列表中取出F值最小节点的ID后对二叉堆进行排序，使得F值最小的节点的ID在最前面
	//参数：
	//返回：
	void HeapBackSort();

	//功能：判定节点是否在开放列表中
	//参数：Point——要判定节点坐标
	//返回：true——在开放列表 false——不在
	bool IsInOpenList(POINT Point);

	//功能：判定节点是否在关闭列表中
	//参数：Point——要判定节点坐标
	//返回：true——在开放列表 false——不在
	bool IsInCloseList(POINT Point);
	
	//功能：获得节点的周围可通过节点
	//参数：Point——节点坐标 AroundNode——用于保存周围节点坐标
	//返回：
	void GetAroundNode(POINT Point,PointList* AroundNode);

	//功能：获得某ID节点在OpenList中索引
	//参数：ID——要得到索引的节点ID
	//返回：节点索引值
	int GetIndex(int ID);

	//功能：将路径保存至链表m_Path中
	//参数：Point——起点坐标 ID——终点ID
	//返回：
	void  GetPath(POINT Point,int ID);

	//void Cout();

	PointList GetAstarPath()
	{
		return m_Path;
	}

	bool IsOpen(POINT Point)
	{
		return m_Map[Point.y][Point.x] == 0;
	}

	void setMapPoint(int x, int y, int point)
	{
		m_Map[y][x] = point;
	}

	/*void Text()
	{
		for (int i = 0; i < m_H; i++)
		{
			for (int j = 0; j < m_W; j++)
			{
				std::cout<<m_Map[i][j]<<"  ";
			}
			std::cout<<std::endl;
		}
	}*/
	


private:
	int m_OpenList[MAXNODECOUNT];         //开放列表（二叉堆优化）用来存放节点的ID，F值最小的排在最前面
	int m_OpenCount;                      //开放列表当前的节点数目  
	int m_OpenID;                         //每次为加入开放列表的节点分配一个唯一的ID
	Node* m_Node;                         //保存每个加入过开放列表的节点，ID为其索引值
	NodeInfo** m_NodeInfo;                //该指针用于保存地图节点的信息，用节点坐标做索引可得到节点信息
	int** m_Map;                          //地图指针  
	int m_W, m_H;                         //地图宽高
	PointList m_Path;                     //保存路径坐标
	int** m_visited;                      //找附近路径 标记已访问
	POS_LIST m_que;                       //找附近路径已经找队列
};
}

