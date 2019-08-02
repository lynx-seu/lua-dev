
PWD=$(shell pwd)

build:
	@docker run -v $(PWD):/tmp -it lynx94/lua-dev sh -c cd /tmp; \
		g++ -O2 --shared lmath.cpp -o math2.so

test:
	@docker run -v $(PWD):/tmp -it lynx94/lua-dev sh -c cd /tmp; \
		lua test.lua

