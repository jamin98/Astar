#include "export_astar.h"
#include "convertor.h"
#include "common.h"
#include <vector>

void export_astar()
{
	using namespace boost::python;

	class_<IntList>("pyIntList")
		.def(vector_indexing_suite<IntList>())
		;

	class_<AstarPOINT>("pyPoint")
		.def_readwrite("x",&AstarPOINT::x)
		.def_readwrite("y",&AstarPOINT::y)
		;

	class_<PointList>("pyPointList")
		.def( vector_indexing_suite<PointList> () )
		;
	class_<AStar>("AStar", no_init)
		.def(init<>())
		.def("Init", &AStar::Init)
		.def("Find", &AStar::Find)
		.def("IsOpen", &AStar::IsOpen)
		.def("GetAstarPath", &AStar::GetAstarPath)
		;
	/*
	vector_from_seq_converter<int>();
	to_python_converter<IntList, vector_to_list_converter<int> >();
	vector_from_seq_converter<float>();
	to_python_converter<FloatList, vector_to_list_converter<float> >();
	vector_from_seq_converter<unsigned char>();
	to_python_converter<ByteList, vector_to_list_converter<unsigned char> >();
	vector_from_seq_converter<AstarPOINT>();
	to_python_converter<PointList, vector_to_list_converter<AstarPOINT> >();
	*/
}
