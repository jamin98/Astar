#include "export_astar.h"
#include "convertor.h"
#include "common.h"
#include "AstarEx.cpp"

void export_astar()
{
	using namespace boost::python;
	using namespace AStarEx;
	
	//2015/06/23 增加新版本

	class_<AStarEx::IntList>("AStarExIntList")
		.def(vector_indexing_suite<AStarEx::IntList>())
		;

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
