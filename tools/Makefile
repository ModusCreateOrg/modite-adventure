INCLUDE=-I../src \
	-I../creative-engine/src \
	-I../creative-engine/src/Types

CFLAGS=-std=c++17

all:
	gcc -o object_attribute $(CFLAGS) $(INCLUDE) object_attribute.cpp

install: all
	cp object_attributes /usr/local/bin

clean:
	rm -rf *.o object_attribute
