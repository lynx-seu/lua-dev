
proj=array.so
src_list=array.cpp \
		 lua_helper.cpp
src_o=$(src_list:.cpp=.o)
CFLAGS=-std=c++11
LDFLAGS=-llua

$(proj):$(src_o)
	g++ $(CFLAGS) -fPIC --shared $^ -o $@ $(LDFLAGS)

%.o:%.cpp
	g++ $(CFLAGS) -c $< -o $@
	
clean:
	-rm -rf $(src_o) $(proj)
