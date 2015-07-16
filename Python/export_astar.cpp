#include "export_astar.h"
#include "convertor.h"
#include "common.h"
#include <vector>

void export_astar()
{
	using namespace boost::python;
	using namespace AStarEx;
	using namespace AStars;

	class_<AStars::IntList>("IntList")
		.def(vector_indexing_suite<AStars::IntList>())
		;

	class_<AStars::POINT>("AStarsPoint")
		.def_readwrite("x",&AStars::POINT::x)
		.def_readwrite("y",&AStars::POINT::y)
		;

	class_<AStars::PointList>("AStarsPointList")
		.def( vector_indexing_suite<AStars::PointList> () )
		;

	class_<AStar>("AStar", no_init)
		.def(init<>())
		.def("Init", &AStar::Init)
		.def("Find", &AStar::Find)
		.def("IsOpen", &AStar::IsOpen)
		.def("GetAstarPath", &AStar::GetAstarPath)
		.def("FindBestPoint", &AStar::FindBestPoint)
		.def("setMapPoint", &AStar::setMapPoint)
		;

	//2015/06/23 增加新版本

	//class_<AStarEx::IntList>("AStarExIntList")
	//	.def(vector_indexing_suite<AStarEx::IntList>())
	//	;

	class_<AStarEx::Point>("AStarExPoint")
		.def_readwrite("x",&AStarEx::Point::x)
		.def_readwrite("y",&AStarEx::Point::y)
		;

	class_<AStarEx::PointList>("AStarExPointList")
		.def( vector_indexing_suite<AStarEx::PointList> () )
		;

	class_<AStarEx::PointQue>("AStarExPointQue")
		.def( vector_indexing_suite<AStarEx::PointQue> () )
		;

	class_<AstarEx>("AStarEx", no_init)
		.def(init<>())
		.def("Init", &AstarEx::Init)
		.def("Find", &AstarEx::findPath)
		.def("IsOpen", &AstarEx::IsOpen)
		.def("FindBestPoint", &AstarEx::FindBestPoint)
		.def("GetAstarPath", &AstarEx::GetAstarPath)
		.def("setMapPoint", &AstarEx::setMapPoint)
		.def("visual", &AstarEx::visual)
		;


}
