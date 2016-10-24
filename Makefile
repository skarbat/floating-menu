#
# Makefile
#
# You need libsdl2-dev and libsdl2-image-dev packages installed
#

CXXFLAGS = -Wall -c -DHAVE_OPENGL -DBCMHOST=1 -std=c++11
LDFLAGS = -lSDL2 -lSDL2_image

app = floating-menu

all: $(app)

$(app): $(app).o
	$(CXX) $(LDFLAGS) $(app).o -o $@

$(app).o: src/$(app.cpp)
	$(CXX) $(CXXFLAGS) src/$(app).cpp -o $@

clean:
	rm *.o && rm $(app)
