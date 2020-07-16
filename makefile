CXX=g++
RM=rm -f
CFLAGS = -std=c++17
LDFLAGS=-g $(--ldflags)
LDLIBS=$(--libs)

SRCS=main.cpp Image.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: imageops

test: Image.o ImageOpsUnitTests.o
	$(CXX) $(CFLAGS) Image.o ImageOpsUnitTests.o -o test $(LDLIBS)

ImageOpsUnitTests.o: Image.cpp ImageOpsUnitTests.cpp
	$(CXX) $(CFLAGS) -c Image.cpp ImageOpsUnitTests.cpp

imageops: $(OBJS)
	$(CXX) $(LDFLAGS) -o imageops $(OBJS) $(LDLIBS)

main.o: main.cpp Image.cpp

Image.o: Image.h Image.cpp

ImageOpsUnitTests: test
	.\test	

run: imageops
	.\imageops aaa.txt output

clean:
	$(RM) *.o
	$(RM) imageops
	$(RM) test
