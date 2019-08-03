
proj=main
src_list=main.cpp \
		 lua_helper.cpp
src_o=$(src_list:.cpp=.o)
CFLAGS=-std=c++11
LDFLAGS=-llua

$(proj):$(src_o)
	g++ $(CFLAGS) $^ -o $@ $(LDFLAGS)

run:$(proj)
	./$(proj)

%.o:%.cpp
	g++ $(CFLAGS) -c $< -o $@
	
clean:
	-rm -rf $(src_o) $(proj)
