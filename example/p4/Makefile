
# [gcc -fPIC介绍](https://stackoverflow.com/questions/48653522/gcc-fpic-vs-shared)
# lua 源码编译时增加-fPIC选项 (gcc动态库链接静态库的问题)
#     $ make linux MYCFLAGS=-fPIC MYLDFLAGS=-fPIC

proj=dlx.so
src_list=ldlx.cpp 				\
		 dlx.cpp 				\
		 lua_helper.cpp
src_o=$(src_list:.cpp=.o)
CFLAGS=-std=c++11 -fPIC
LDFLAGS=-llua

all:$(proj)
	lua test.lua

$(proj):$(src_o)
	g++ $(CFLAGS) -shared $^ -o $@ $(LDFLAGS)

%.o:%.cpp
	g++ $(CFLAGS) -c $< -o $@
	
clean:
	-rm -rf $(src_o) $(proj)
