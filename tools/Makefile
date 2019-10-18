INCLUDE=-I../src \
	-I../creative-engine/src \
	-I../creative-engine/src/Types

all:
	gcc -o object_attribute $(INCLUDE) object_attribute.cpp

install: all
	cp object_attributes /usr/local/bin

clean:
	rm -rf *.o object_attribute
