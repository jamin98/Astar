#include "config.h"

extern void export_astar();

BOOST_PYTHON_MODULE(_Astar)
{
	export_astar();
}