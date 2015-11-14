#include "AstarEx.h"
#define SQRT2 1.41421356237309504880f

using namespace AStarEx;

AstarEx::AstarEx()
{
	nodes = NULL;
	m_W = 0;
	m_H = 0;
	nowVersion = 1;
}


AstarEx::~AstarEx()
{
	for (int i = 0; i < m_H; i++)
	{
		delete[] nodes[i];
		nodes[i] = NULL;
	}
	delete[] nodes;
	nodes = NULL;
}

void AstarEx::Init(IntList &Map, int w, int h)
{
	m_W = w;
	m_H = h;

	//用于记录保每个节点信息
	nodes = new Node*[h];
	assert(nodes != NULL);
	for (int y = 0 ; y < h; y++)
	{
		nodes[y]=new Node[w];
		assert(nodes[y] != NULL);
		memset(nodes[y], 0, sizeof(Node)*w);

		//只需要关闭列表 后续可优化内存
		for (int x = 0; x < w; x++) {
			//if(Map[y*h+x]!=1)
				nodes[y][x] = Node(x,y,Map[y*h+x]!=1);
		}
	}
}

void AstarEx::setMapPoint(int x, int y, int point)
{
	if( x < 0 || x >= m_W  ||  y < 0 || y >= m_H )
	{
		return;
	}
	if (this->nodes[y]!=NULL&&point==1){
		//memset(nodes[y]+x, 0, sizeof(Node));
		nodes[y][x].walkable = false;
	}else{
		nodes[y][x].walkable = true;
	}
}

bool AstarEx::findPath(int startX, int startY, int endX, int endY) {

	if (nodes[startY]==NULL || (startX==endX&&startY==endY) 
		||( startX < 1 || startX >= m_W  ||  startY < 1 || startY >= m_H )
		||( endX < 1 || endX >= m_W  ||  endY < 1 || endY >= m_H )) {
		return false;
	}
	path.clear();

	nowVersion++;
	open = BinaryHeap();
	Node *startNode= &nodes[startY][startX];
	startNode->g = 0;
	startNode->version = nowVersion;

	Node *endNode = &nodes[endY][endX];
	Node *node = startNode;
	while (node != endNode) {
		for (int y = -1; y < 2; y++ ) {
			if (node->y+y>0&&node->y+y<m_H) {
				Node* line = nodes[node->y+y];
				for (int x = -1; x < 2; x++ ) {
					if (y != 0 || x != 0) {
						if(node->x+x>0&&node->x+x<m_W){
							Node *test = &line[node->x + x];
							if (test->walkable!=false){
								float cost = 0.0;
								if (x==0||y==0) {
									cost = 1;
								}else {
									cost = SQRT2;
								}
								float g = node->g + cost;
								float f = g + abs(test->x - endX) + abs(test->y - endY);
								if (test->version == nowVersion){
									if (test->f > f){
										test->f = f;
										test->g = g;
										test->parent = node;
									}
								} else {
									test->f = f;
									test->g = g;
									test->parent =  node;
									open.ins(test);
									test->version = nowVersion;
								}
							}
						}
					}
				}
			}
		}
		if (open.a.size() == 1){
			return false;
		}
		node = open.pop();
	}
	path.push_back(Point(node->x, node->y));
	while (node != startNode){
		node = node->parent;
		path.push_front(Point(node->x, node->y));
	}
	return true;
}


bool AstarEx::TestPoint(int x, int y, Point& best)
{
	if(( x >=0 && x < this->m_W ) &&  ( y >=0 && y < this->m_H ))//此结点是否存在
	{//如果存在
		if(m_visited[y*this->m_H+x]!=1)//检查是否已经访问
		{//如果未访问
			Point Pt = Point(x, y);
			if(IsOpen(Pt))//检查是否能
			{//如果能通过
				best = Pt; //返回最佳点
				return true;//退出
			}else
			{//如果不能通过
				m_visited[y*this->m_H+x] = 1;//标记已经访问
				m_que.push_back(Pt);//放入队列
			}
		}//如果已经访问则忽略这个结点
	}
	return false;
}

Point AstarEx::FindBestPoint(int x, int y)
{
	Point Pt = Point(0, 0);
	if( x < 0 || x >= m_W  ||  y < 0 || y >= m_H )
	{//此结点不合法
		return Pt;
	}
	Point best = Point(x, y);
	if(IsOpen(best))//检查是否能通过
	{//如果能通过
		return best;
	}

	//重新初始化数组
	m_visited.clear();
	m_visited.resize(m_H*m_W, 0);
	

	m_visited[y*this->m_H+x] = 1;		//标记已访问

	Point u;
	m_que.push_back(best);				//初始结点v入队列

	while(!m_que.empty())			//队列非空时
	{
		u = m_que.front();m_que.pop_front();	//出队列
		//右下
		if(TestPoint(u.x+1, u.y+1, best))break;
		//下
		if(TestPoint(u.x, u.y+1, best))break;
		//左下
		if(TestPoint(u.x-1, u.y+1, best))break;
		//左
		if(TestPoint(u.x-1, u.y, best))break;
		//左上
		if(TestPoint(u.x-1, u.y-1, best))break;
		//上
		if(TestPoint(u.x, u.y-1, best))break;
		//右上
		if(TestPoint(u.x+1, u.y-1, best))break;
		//右
		if(TestPoint(u.x+1, u.y, best))break;
	}
	m_que.clear();
	return best;
}

inline void floydVector(Point &target, const Point n1, const Point n2) {
	target.x = n1.x - n2.x;
	target.y = n1.y - n2.y;
}

void AstarEx::floyd() {
	if (path.size() == 0)
		return;
	floydPath.clear();
	floydPath = path;
	int len = floydPath.size();
	if (len > 2){
		Point vector = Point(0, 0);
		Point tempVector = Point(0, 0);
		floydVector(vector, floydPath[len - 1], floydPath[len - 2]);
		for (int i = floydPath.size() - 3; i >= 0; i--){
			floydVector(tempVector, floydPath[i + 1], floydPath[i]);
			if (vector.x == tempVector.x && vector.y == tempVector.y){
				floydPath.erase(floydPath.begin() +i + 1, floydPath.begin() +i + 1 + 1);
			} else {
				vector.x = tempVector.x;
				vector.y = tempVector.y;
			}
		}
	}
	len = floydPath.size();
	for (int i = len - 1; i >= 0; i--){
		for (int j = 0; j <= i - 2; j++){
			if (floydCrossAble(floydPath[i], floydPath[j])){
				for (int k = i - 1; k > j; k--){
					floydPath.erase(floydPath.begin() + k, floydPath.begin() + k +1);
				}
				i = j;
				len = floydPath.size();
				break;
			}
		}
	}
}

bool AstarEx::floydCrossAble(const Point &n1, const Point &n2) {
	PointList ps = bresenhamNodes(n1, n2);
	for (int i = ps.size() - 2; i > 0; i--){
		int y = ps[i].y;
		int x = ps[i].x;
		if (nodes[y][x].walkable!=true) {
			return false;
		}
	}
	return true;
}

bool AstarEx::visual(const Point &n1, const Point &n2) {
	const PointList &ps = bresenhamNodes(n1, n2);
	//for (int i = ps.size()-1; i > 0; i--){
	//	int y = ps[i].y;
	//	int x = ps[i].x;
	//	if (nodes[y][x].walkable!=true) {
	//		return false;
	//	}
	//}
	for (PointList::const_iterator it=ps.begin(); it!=ps.end();++it)
	{
		if (nodes[(*it).y][(*it).x].walkable!=true) {
			return false;
		}
	}
	return true;
}

PointList AstarEx::bresenhamNodes(Point p1, Point p2) {
	bool steep = abs(p2.y - p1.y) > abs(p2.x - p1.x);
	if (steep) {
		int temp = p1.x;
		p1.x = p1.y;
		p1.y = temp;
		temp = p2.x;
		p2.x = p2.y;
		p2.y = temp;
	}
	int stepX = p2.x > p1.x?1:(p2.x < p1.x? -1:0);
	int	deltay = p2.x-p1.x!=0 ? (p2.y - p1.y)/abs(p2.x-p1.x):0;

	PointList ret;
	int nowX = p1.x + stepX;
	int nowY = p1.y + deltay;
	if (steep) {
		ret.push_back(Point(p1.y, p1.x));
	}else {
		ret.push_back(Point(p1.x,p1.y));
	}
	if (abs(p1.x - p2.x) == abs(p1.y - p2.y)) {
		if(p1.x<p2.x&&p1.y<p2.y){
			ret.push_back(Point(p1.x, p1.y + 1));
			ret.push_back(Point(p2.x, p2.y - 1));
		}else if(p1.x>p2.x&&p1.y>p2.y){
			ret.push_back(Point(p1.x, p1.y - 1));
			ret.push_back(Point(p2.x, p2.y + 1));
		}else if(p1.x<p2.x&&p1.y>p2.y){
			ret.push_back(Point(p1.x, p1.y - 1));
			ret.push_back(Point(p2.x, p2.y + 1));
		}else if(p1.x>p2.x&&p1.y<p2.y){
			ret.push_back(Point(p1.x, p1.y + 1));
			ret.push_back(Point(p2.x, p2.y - 1));
		}
	}
	while (nowX != p2.x) {
		int fy=(int) floor((float)nowY);
		int cy=(int) ceil((float)nowY);
		if (steep) {
			ret.push_back(Point(fy, nowX));
		}else{
			ret.push_back(Point(nowX, fy));
		}
		if (fy != cy) {
			if (steep) {
				ret.push_back(Point(cy,nowX));
			}else{
				ret.push_back(Point(nowX, cy));
			}
		}else if(deltay!=0){
			if (steep) {
				ret.push_back(Point(cy+1,nowX));
				ret.push_back(Point(cy-1,nowX));
			}else{
				ret.push_back(Point(nowX, cy+1));
				ret.push_back(Point(nowX, cy-1));
			}
		}
		nowX += stepX;
		nowY += deltay;
	}
	if (steep) {
		ret.push_back(Point(p2.y,p2.x));
	}else {
		ret.push_back(Point(p2.x,p2.y));
	}
	return ret;
}
