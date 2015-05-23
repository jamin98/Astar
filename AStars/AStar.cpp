#include "AStar.h"

AStar::AStar()
{
	memset(m_OpenList, 0, sizeof(m_OpenList));
	//memset(m_Node, 0, sizeof(m_Node));
	m_OpenCount = 0;
	m_OpenID = -1;                           //ID要作为索引值使用开始赋值为-1
	m_NodeInfo = NULL;
	m_Node = NULL;
	m_Map = NULL;
	m_Path.clear();
	m_W = 0;
	m_H = 0;
}


AStar::~AStar()
{
	for (int i = 0; i < m_H; i++)
	{
		delete[m_W] m_Map[i];
		m_Map[i] = NULL;
		delete[m_W] m_NodeInfo[i];
		m_NodeInfo[i] = NULL;
	}

	delete[m_H] m_Map;
	m_Map = NULL;
	delete[m_H] m_NodeInfo;
	m_NodeInfo = NULL;
	delete [] m_Node;
	m_Node = NULL;
}


void AStar::Init(IntList Map, int w, int h)
{
	m_W = w;
	m_H = h;
	/**
	原来用指针传值,python不支持传指针,改用vector传值
	assert(Map != NULL);

	//动态初始化二维数组保存地图
	m_Map = new int*[h];
	assert(m_Map != NULL);
	for (int i = 0; i < h; i++)
	{
		m_Map[i] = new int[w];
		assert(m_Map[i] != NULL);
		memcpy(m_Map[i], Map+i*w, w*sizeof(int));
	}*/
	
	m_Map = new int*[h];
	for (int y = 0; y < h; y++)
	{
		m_Map[y] = new int[w];
		for (int x = 0; x < w; x++)
		{
			m_Map[y][x] = Map[y*h+x];
		}
	}


	//用于保存加入过开放列表的节点
	m_Node = new Node[w*h];
	assert(m_Node != NULL);
	memset(m_Node, 0, sizeof(Node)*w*h);

	//用于记录保每个节点信息
	m_NodeInfo = new NodeInfo*[h];
	assert(m_NodeInfo != NULL);
	for (int i = 0 ; i < h; i++)
	{
		m_NodeInfo[i]=new NodeInfo[w];
		assert(m_NodeInfo[i] != NULL);
		memset(m_NodeInfo[i], 0, sizeof(NodeInfo)*w);
	}
}


void AStar::Find(int xBegin, int yBegin, int xEnd, int yEnd)
{
	AstarPOINT PtBegin = {xBegin, yBegin};
	OpenNote(PtBegin, 0, 0, 0);
	Node CurrNode;
	memset(&CurrNode, 0, sizeof(Node));
	int currID = 0;
	int tmpID = 0;
	int tmpScoreF = 0;
	int tmpScoreG = 0;
	PointList AroundNode;
	//若开放列表不为空
	while (m_OpenCount > 0)
	{
		//每次取出二叉堆（开放列表）中最前面的ID
		currID = m_OpenList[0];
		//作加入关闭列表标记
		CloseNote(currID);
		//取得此节点坐标
		CurrNode.m_Pos = m_Node[currID].m_Pos;
		//是否是重点坐标
		if ((CurrNode.m_Pos.x == xEnd) && (CurrNode.m_Pos.y == yEnd))
		{
			GetPath(PtBegin,currID);
			break;
		}
		//得到此节点周围的节点
		GetAroundNode(CurrNode.m_Pos, &AroundNode);
		PointList::iterator it = AroundNode.begin();
		for( ;it != AroundNode.end(); it++)
		{
			//计算G值和F值
			tmpScoreG = CurrNode.m_ScoreF + ((CurrNode.m_Pos.x == (*it).x) || (CurrNode.m_Pos.y == (*it).y)) ? W_H_MOVE : SLANTMOVE;
			tmpScoreF = tmpScoreG + (abs((*it).x - xEnd) + abs((*it).y - yEnd)) * W_H_MOVE;  //曼哈顿法
			//return ((state.x - goal_location[0]) ** 2 + (state.y - goal_location[1]) ** 2) ** 0.5//欧氏距离的定义
			//节点是否已在开放列表
			if (IsInOpenList(*it))
			{
				//由坐标获得节点在开放列表中的ID
				tmpID = m_NodeInfo[(*it).y][(*it).x].m_NodeID;
				//现在的G值与原来的G值进行比较
				if (tmpScoreG < m_Node[tmpID].m_ScoreG)
				{
					//若现在的G值更小则修改其F、G值，并更换其父节点
					m_Node[tmpID].m_ScoreF = tmpScoreF;
					m_Node[tmpID].m_ScoreG = tmpScoreG;
					m_Node[tmpID].m_FatherID = currID;
					//重新排序
					HeapAheadSort(GetIndex(tmpID));
				}
			}
			else //不再开放列表则将节点加入开放列表
			{
				AstarPOINT Point = {(*it).x, (*it).y};
				OpenNote(Point, tmpScoreF, tmpScoreG, currID);
			}
		}
	}
}


void AStar::OpenNote(AstarPOINT Point, int ScoreF, int ScoreG, int FatherID)
{
	m_OpenCount++;
	m_OpenID++; 
	m_NodeInfo[Point.y][Point.x].m_InCloseList = true; //标记加入开放列表
	m_NodeInfo[Point.y][Point.x].m_NodeID = m_OpenID;  //赋予唯一ID
	m_Node[m_OpenID].m_Pos = Point;                    //以ID为索引给节点变量赋值
	m_Node[m_OpenID].m_ScoreF = ScoreF;
	m_Node[m_OpenID].m_ScoreG = ScoreG;
	m_Node[m_OpenID].m_FatherID = FatherID;
	m_OpenList[m_OpenCount-1] = m_OpenID;                //将节点ID存入开放列表
	HeapAheadSort(m_OpenCount);                        //进行二叉堆排序以保证F最小ID排在最前
}


void AStar::CloseNote(int ID)
{
	m_NodeInfo[m_Node[ID].m_Pos.y][m_Node[ID].m_Pos.x].m_InCloseList = true;
	m_NodeInfo[m_Node[ID].m_Pos.y][m_Node[ID].m_Pos.x].m_InOpenList = false;
	//将二叉堆最后一个ID置于最前，并进行排序
	m_OpenList[0] = m_OpenList[--m_OpenCount];
	HeapBackSort();
}


void AStar::HeapAheadSort(int Index)
{
	//索引均为在二叉堆m_OpenList中索引
	int fatherIndex = 0;
	int tmpID = 0;
	while (Index > 1)
	{
		fatherIndex = Index/2;
		//若当前节点F值小于父节点F值，将F值较小的ID向前移动
		if (m_Node[m_OpenList[Index-1]].m_ScoreF < m_Node[m_OpenList[fatherIndex-1]].m_ScoreF)
		{
			tmpID = m_OpenList[Index-1];
			m_OpenList[Index-1] = m_OpenList[fatherIndex-1];
			m_OpenList[fatherIndex-1] = tmpID;
			Index = fatherIndex;
		}
		else
		{
			break;
		}
	}
}


void AStar::HeapBackSort()
{
	int currIndex = 1;                            //当前节点
	int currChildIndex = 2*currIndex;             //当前节点的子节点
	int tmp = m_OpenList[currIndex-1];            //保存首节点ID
	//判断节点是否有子节点
	while (currChildIndex <= m_OpenCount)
	{  
		//判断是否有两个子节点
		if (currChildIndex + 1 <= m_OpenCount)
		{
			//获得最小F值的子节点
			if (m_Node[m_OpenList[currChildIndex-1]].m_ScoreF > m_Node[m_OpenList[currChildIndex-1+1]].m_ScoreF)
			{
				currChildIndex++;
			}
		}
		//如果子节点的F值比当前节点大，结束循环
		if (m_Node[m_OpenList[currIndex-1]].m_ScoreF < m_Node[m_OpenList[currChildIndex-1]].m_ScoreF)
		{
			break;
		}
		else
		{
			//否则将子节点ID赋予当前前节点
			m_OpenList[currIndex-1] = m_OpenList[currChildIndex-1];
			//改变索引继续循环
			currIndex = currChildIndex;
			currChildIndex *= 2;
		}
	}
	//循环结束时找到保存的节点ID的正确位置
	m_OpenList[currIndex-1] = tmp;

}


bool AStar::IsInOpenList(AstarPOINT Point)
{
	return m_NodeInfo[Point.y][Point.x].m_InOpenList;
}


bool AStar::IsInCloseList(AstarPOINT Point)
{
	return m_NodeInfo[Point.y][Point.x].m_InCloseList;
}


void AStar::GetAroundNode(AstarPOINT Point, PointList *AroundNode)
{
	//每次获得前清空list
	AroundNode->clear();

	int CurrX = 0;
	int CurrY = 0;
	bool LCanPass = false;
	bool RCanPass = false;
	bool UCanPass = false;
	bool DCanPass = false;

	//左边节点
	CurrX = Point.x - 1;
	CurrY = Point.y;
	//是否出界
	if ((CurrX >= 0) && (CurrX < m_W) && (CurrY >= 0) && (CurrY < m_H))
	{
		//是否为障碍物
		if (m_Map[CurrY][CurrX] != 1)
		{
			LCanPass = true;
			AstarPOINT tmpPoint = { CurrX, CurrY };
			//是否在关闭列表
			if (!IsInCloseList(tmpPoint))
			{
				AroundNode->push_back(tmpPoint);
			}
			
		}
		
	}
	//右边节点
	CurrX = Point.x + 1;
	CurrY = Point.y;
	//是否出界
	if ((CurrX >= 0) && (CurrX < m_W) && (CurrY >= 0) && (CurrY < m_H))
	{
		//是否为障碍物
		if (m_Map[CurrY][CurrX] != 1)
		{
			RCanPass = true;
			AstarPOINT tmpPoint = { CurrX, CurrY };
			if (!IsInCloseList(tmpPoint))
			{
				AroundNode->push_back(tmpPoint);
			}
		}
	}
	//上节点
	CurrX = Point.x;
	CurrY = Point.y - 1;
	//是否出界
	if ((CurrX >= 0) && (CurrX < m_W) && (CurrY >= 0) && (CurrY < m_H))
	{
		//是否为障碍物
		if (m_Map[CurrY][CurrX] != 1)
		{
			UCanPass = true;
			AstarPOINT tmpPoint = { CurrX, CurrY };
			if (!IsInCloseList(tmpPoint))
			{
				AroundNode->push_back(tmpPoint);
			}
		}
	}
	//下节点
	CurrX = Point.x;
	CurrY = Point.y + 1;
	//是否出界
	if ((CurrX >= 0) && (CurrX < m_W) && (CurrY >= 0) && (CurrY < m_H))
	{
		//是否为障碍物
		if (m_Map[CurrY][CurrX] != 1)
		{
			DCanPass = true;
			AstarPOINT tmpPoint = { CurrX, CurrY };
			if (!IsInCloseList(tmpPoint))
			{
				AroundNode->push_back(tmpPoint);
			}
		}
	}
	//左上节点，首先判断正左和正上方是否有障碍物
	if (LCanPass && UCanPass)
	{
		CurrX = Point.x - 1;
		CurrY = Point.y - 1;
		//是否出界
		if ((CurrX >= 0) && (CurrX < m_W) && (CurrY >= 0) && (CurrY < m_H))
		{
			//是否为障碍物
			if (m_Map[CurrY][CurrX] != 1)
			{
				AstarPOINT tmpPoint = { CurrX, CurrY };
				if (!IsInCloseList(tmpPoint))
				{
					AroundNode->push_back(tmpPoint);
				}
			}
		}
	}
	//左下节点
	if (LCanPass && DCanPass)
	{
		CurrX = Point.x - 1;
		CurrY = Point.y + 1;
		//是否出界
		if ((CurrX >= 0) && (CurrX < m_W) && (CurrY >= 0) && (CurrY < m_H))
		{
			//是否为障碍物
			if (m_Map[CurrY][CurrX] != 1)
			{
				AstarPOINT tmpPoint = { CurrX, CurrY };
				if (!IsInCloseList(tmpPoint))
				{
					AroundNode->push_back(tmpPoint);
				}
			}
		}
	}
	//右上节点
	if (RCanPass && UCanPass)
	{
		CurrX = Point.x + 1;
		CurrY = Point.y - 1;
		//是否出界
		if ((CurrX >= 0) && (CurrX < m_W) && (CurrY >= 0) && (CurrY < m_H))
		{
			//是否为障碍物
			if (m_Map[CurrY][CurrX] != 1)
			{
				AstarPOINT tmpPoint = { CurrX, CurrY };
				if (!IsInCloseList(tmpPoint))
				{
					AroundNode->push_back(tmpPoint);
				}
			}
		}
	}
	//右下节点
	if (RCanPass && DCanPass)
	{
		CurrX = Point.x + 1;
		CurrY = Point.y + 1;
		//是否出界
		if ((CurrX >= 0) && (CurrX < m_W) && (CurrY >= 0) && (CurrY < m_H))
		{
			//是否为障碍物
			if (m_Map[CurrY][CurrX] != 1)
			{
				AstarPOINT tmpPoint = { CurrX, CurrY };
				if (!IsInCloseList(tmpPoint))
				{
					AroundNode->push_back(tmpPoint);
				}
			}
		}
	}
}

int AStar::GetIndex(int ID)
{
	for (int i = 1; i <= m_OpenCount; i++)
	{
		if (m_OpenList[i] == ID)
		{
			return i;
		}
	}
	return -1;
}


void AStar::GetPath(AstarPOINT Point, int ID)
{
	int CurrX = m_Node[ID].m_Pos.x;
	int CurrY = m_Node[ID].m_Pos.y;
	while ((CurrX != Point.x) || (CurrY != Point.y))
	{
		AstarPOINT Pt = {CurrX, CurrY};
		m_Path.push_back(Pt);
		ID = m_Node[ID].m_FatherID;
		CurrX = m_Node[ID].m_Pos.x;
		CurrY = m_Node[ID].m_Pos.y;
	}
	AstarPOINT Pt = {CurrX, CurrY};
	m_Path.push_back(Pt);
}

//void AStar::Cout()
//{
//	std::list<POINT>::iterator it = m_Path.begin();
//	for (;it!=m_Path.end();it++)
//	{
//		std::cout<<it->x<<" "<<it->y<<std::endl;
//	}
//}