#ifndef CONFIG_H_
#define CONFIG_H_

// boost includes
#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>
#include <boost/format.hpp>
#include <boost/random.hpp>
#include <boost/foreach.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#ifndef FOREACH
#define FOREACH BOOST_FOREACH
#endif

// detour includes
#include "AstarEx.h"

#endif /* CONFIG_H_ */
