EXE = pa3
OBJS_EXE = RGBAPixel.o lodepng.o PNG.o main.o rgbtree.o tileUtil.o

CXX = clang++
CXXFLAGS = -std=c++17 -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic 
LD = clang++
LDFLAGS = -std=c++17 -stdlib=libc++ -lpthread -lm 

all	: pa3

$(EXE) : $(OBJS_EXE)
	$(LD) $(OBJS_EXE) $(LDFLAGS) -o $(EXE)

#object files
RGBAPixel.o : cs221util/RGBAPixel.cpp cs221util/RGBAPixel.h
	$(CXX) $(CXXFLAGS) cs221util/RGBAPixel.cpp -o $@

PNG.o : cs221util/PNG.cpp cs221util/PNG.h cs221util/RGBAPixel.h cs221util/lodepng/lodepng.h
	$(CXX) $(CXXFLAGS) cs221util/PNG.cpp -o $@

lodepng.o : cs221util/lodepng/lodepng.cpp cs221util/lodepng/lodepng.h
	$(CXX) $(CXXFLAGS) cs221util/lodepng/lodepng.cpp -o $@

tileUtil.o : tileUtil.h tileUtil.cpp cs221util/PNG.h cs221util/RGBAPixel.h rgbtree.h
	$(CXX) $(CXXFLAGS) tileUtil.cpp -o $@

rgbtree.o : rgbtree.h rgbtree.cpp cs221util/PNG.h cs221util/RGBAPixel.h tileUtil.h
	$(CXX) $(CXXFLAGS) rgbtree.cpp -o $@

main.o : main.cpp cs221util/PNG.h cs221util/RGBAPixel.h rgbtree.h  tileUtil.h
	$(CXX) $(CXXFLAGS) main.cpp -o main.o

clean :
	-rm -f *.o $(EXE)
