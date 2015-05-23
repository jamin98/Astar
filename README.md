# Astar
python调用c++ boost.python封装的A*寻路算法

##说明
python调用c++ A*寻路
采用boost 1.49 c++库封装成python的模块

##boost编译
bjam --with-python --toolset=msvc-10.0 --build-type=complete stage
