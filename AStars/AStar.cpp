#include "AStar.h"

AStar::AStar()
{
	memset(m_OpenList, 0, sizeof(m_OpenList));
	//memset(m_Node, 0, sizeof(m_Node));
	m_OpenCount = 0;
	m_OpenID = -1;                           //IDҪ��Ϊ����ֵʹ�ÿ�ʼ��ֵΪ-1
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
	ԭ����ָ�봫ֵ,python��֧�ִ�ָ��,����vector��ֵ
	assert(Map != NULL);

	//��̬��ʼ����ά���鱣���ͼ
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


	//���ڱ������������б�Ľڵ�
	m_Node = new Node[w*h];
	assert(m_Node != NULL);
	memset(m_Node, 0, sizeof(Node)*w*h);

	//���ڼ�¼��ÿ���ڵ���Ϣ
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
	//�������б�Ϊ��
	while (m_OpenCount > 0)
	{
		//ÿ��ȡ������ѣ������б�����ǰ���ID
		currID = m_OpenList[0];
		//������ر��б���
		CloseNote(currID);
		//ȡ�ô˽ڵ�����
		CurrNode.m_Pos = m_Node[currID].m_Pos;
		//�Ƿ����ص�����
		if ((CurrNode.m_Pos.x == xEnd) && (CurrNode.m_Pos.y == yEnd))
		{
			GetPath(PtBegin,currID);
			break;
		}
		//�õ��˽ڵ���Χ�Ľڵ�
		GetAroundNode(CurrNode.m_Pos, &AroundNode);
		PointList::iterator it = AroundNode.begin();
		for( ;it != AroundNode.end(); it++)
		{
			//����Gֵ��Fֵ
			tmpScoreG = CurrNode.m_ScoreF + ((CurrNode.m_Pos.x == (*it).x) || (CurrNode.m_Pos.y == (*it).y)) ? W_H_MOVE : SLANTMOVE;
			tmpScoreF = tmpScoreG + (abs((*it).x - xEnd) + abs((*it).y - yEnd)) * W_H_MOVE;  //�����ٷ�
			//return ((state.x - goal_location[0]) ** 2 + (state.y - goal_location[1]) ** 2) ** 0.5//ŷ�Ͼ���Ķ���
			//�ڵ��Ƿ����ڿ����б�
			if (IsInOpenList(*it))
			{
				//�������ýڵ��ڿ����б��е�ID
				tmpID = m_NodeInfo[(*it).y][(*it).x].m_NodeID;
				//���ڵ�Gֵ��ԭ����Gֵ���бȽ�
				if (tmpScoreG < m_Node[tmpID].m_ScoreG)
				{
					//�����ڵ�Gֵ��С���޸���F��Gֵ���������丸�ڵ�
					m_Node[tmpID].m_ScoreF = tmpScoreF;
					m_Node[tmpID].m_ScoreG = tmpScoreG;
					m_Node[tmpID].m_FatherID = currID;
					//��������
					HeapAheadSort(GetIndex(tmpID));
				}
			}
			else //���ٿ����б��򽫽ڵ���뿪���б�
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
	m_NodeInfo[Point.y][Point.x].m_InCloseList = true; //��Ǽ��뿪���б�
	m_NodeInfo[Point.y][Point.x].m_NodeID = m_OpenID;  //����ΨһID
	m_Node[m_OpenID].m_Pos = Point;                    //��IDΪ�������ڵ������ֵ
	m_Node[m_OpenID].m_ScoreF = ScoreF;
	m_Node[m_OpenID].m_ScoreG = ScoreG;
	m_Node[m_OpenID].m_FatherID = FatherID;
	m_OpenList[m_OpenCount-1] = m_OpenID;                //���ڵ�ID���뿪���б�
	HeapAheadSort(m_OpenCount);                        //���ж���������Ա�֤F��СID������ǰ
}


void AStar::CloseNote(int ID)
{
	m_NodeInfo[m_Node[ID].m_Pos.y][m_Node[ID].m_Pos.x].m_InCloseList = true;
	m_NodeInfo[m_Node[ID].m_Pos.y][m_Node[ID].m_Pos.x].m_InOpenList = false;
	//����������һ��ID������ǰ������������
	m_OpenList[0] = m_OpenList[--m_OpenCount];
	HeapBackSort();
}


void AStar::HeapAheadSort(int Index)
{
	//������Ϊ�ڶ����m_OpenList������
	int fatherIndex = 0;
	int tmpID = 0;
	while (Index > 1)
	{
		fatherIndex = Index/2;
		//����ǰ�ڵ�FֵС�ڸ��ڵ�Fֵ����Fֵ��С��ID��ǰ�ƶ�
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
	int currIndex = 1;                            //��ǰ�ڵ�
	int currChildIndex = 2*currIndex;             //��ǰ�ڵ���ӽڵ�
	int tmp = m_OpenList[currIndex-1];            //�����׽ڵ�ID
	//�жϽڵ��Ƿ����ӽڵ�
	while (currChildIndex <= m_OpenCount)
	{  
		//�ж��Ƿ��������ӽڵ�
		if (currChildIndex + 1 <= m_OpenCount)
		{
			//�����СFֵ���ӽڵ�
			if (m_Node[m_OpenList[currChildIndex-1]].m_ScoreF > m_Node[m_OpenList[currChildIndex-1+1]].m_ScoreF)
			{
				currChildIndex++;
			}
		}
		//����ӽڵ��Fֵ�ȵ�ǰ�ڵ�󣬽���ѭ��
		if (m_Node[m_OpenList[currIndex-1]].m_ScoreF < m_Node[m_OpenList[currChildIndex-1]].m_ScoreF)
		{
			break;
		}
		else
		{
			//�����ӽڵ�ID���赱ǰǰ�ڵ�
			m_OpenList[currIndex-1] = m_OpenList[currChildIndex-1];
			//�ı���������ѭ��
			currIndex = currChildIndex;
			currChildIndex *= 2;
		}
	}
	//ѭ������ʱ�ҵ�����Ľڵ�ID����ȷλ��
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
	//ÿ�λ��ǰ���list
	AroundNode->clear();

	int CurrX = 0;
	int CurrY = 0;
	bool LCanPass = false;
	bool RCanPass = false;
	bool UCanPass = false;
	bool DCanPass = false;

	//��߽ڵ�
	CurrX = Point.x - 1;
	CurrY = Point.y;
	//�Ƿ����
	if ((CurrX >= 0) && (CurrX < m_W) && (CurrY >= 0) && (CurrY < m_H))
	{
		//�Ƿ�Ϊ�ϰ���
		if (m_Map[CurrY][CurrX] != 1)
		{
			LCanPass = true;
			AstarPOINT tmpPoint = { CurrX, CurrY };
			//�Ƿ��ڹر��б�
			if (!IsInCloseList(tmpPoint))
			{
				AroundNode->push_back(tmpPoint);
			}
			
		}
		
	}
	//�ұ߽ڵ�
	CurrX = Point.x + 1;
	CurrY = Point.y;
	//�Ƿ����
	if ((CurrX >= 0) && (CurrX < m_W) && (CurrY >= 0) && (CurrY < m_H))
	{
		//�Ƿ�Ϊ�ϰ���
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
	//�Ͻڵ�
	CurrX = Point.x;
	CurrY = Point.y - 1;
	//�Ƿ����
	if ((CurrX >= 0) && (CurrX < m_W) && (CurrY >= 0) && (CurrY < m_H))
	{
		//�Ƿ�Ϊ�ϰ���
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
	//�½ڵ�
	CurrX = Point.x;
	CurrY = Point.y + 1;
	//�Ƿ����
	if ((CurrX >= 0) && (CurrX < m_W) && (CurrY >= 0) && (CurrY < m_H))
	{
		//�Ƿ�Ϊ�ϰ���
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
	//���Ͻڵ㣬�����ж���������Ϸ��Ƿ����ϰ���
	if (LCanPass && UCanPass)
	{
		CurrX = Point.x - 1;
		CurrY = Point.y - 1;
		//�Ƿ����
		if ((CurrX >= 0) && (CurrX < m_W) && (CurrY >= 0) && (CurrY < m_H))
		{
			//�Ƿ�Ϊ�ϰ���
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
	//���½ڵ�
	if (LCanPass && DCanPass)
	{
		CurrX = Point.x - 1;
		CurrY = Point.y + 1;
		//�Ƿ����
		if ((CurrX >= 0) && (CurrX < m_W) && (CurrY >= 0) && (CurrY < m_H))
		{
			//�Ƿ�Ϊ�ϰ���
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
	//���Ͻڵ�
	if (RCanPass && UCanPass)
	{
		CurrX = Point.x + 1;
		CurrY = Point.y - 1;
		//�Ƿ����
		if ((CurrX >= 0) && (CurrX < m_W) && (CurrY >= 0) && (CurrY < m_H))
		{
			//�Ƿ�Ϊ�ϰ���
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
	//���½ڵ�
	if (RCanPass && DCanPass)
	{
		CurrX = Point.x + 1;
		CurrY = Point.y + 1;
		//�Ƿ����
		if ((CurrX >= 0) && (CurrX < m_W) && (CurrY >= 0) && (CurrY < m_H))
		{
			//�Ƿ�Ϊ�ϰ���
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