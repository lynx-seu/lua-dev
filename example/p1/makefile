
PWD=$(shell pwd)

build:
	@g++ -O2 -fPIC --shared lmath.cpp -o math2.so

test:
	@lua test.lua

clean:
	rm -rf math2.so
