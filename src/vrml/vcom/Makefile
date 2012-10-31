VPATH = ../../common:../../mat
includes = -I ../../common -I ../../mat

ALLOBJS = polygon.o rectangle.o circle.o vrmlmat.o material.o keyvalparser.o \
 transform.o vcom.o  pin.o funnel.o shoulder.o hole.o

CXXFLAGS = -g -Wall

.PHONY : all
#all :  $(ALLOBJS) testRing
all : $(ALLOBJS) $(PINSQOBJ) testPSq

.PHONY : objs
objs : $(ALLOBJS)

testPSq : testPSq.cpp $(ALLOBJS) $(PINSQOBJ)
	g++  $(CXXFLAGS) $(includes) -lm $(ALLOBJS) $(PINSQOBJ) $< -o $@

testRing : testRing.cpp $(ALLOBJS)
	g++ $(CXXFLAGS) $(includes) -lm $(ALLOBJS) $< -o $@

hole.o : hole.cpp polygon.h rectangle.h circle.h transform.h vrmlmat.h material.h vdefs.h vcom.h
	g++ $(CXXFLAGS) $(includes) -c $< -o $@

shoulder.o : shoulder.cpp transform.h vrmlmat.h material.h vdefs.h vcom.h
	g++ $(CXXFLAGS) $(includes) -c $< -o $@

funnel.o : funnel.cpp funnel.h polygon.h circle.h rectangle.h transform.h vrmlmat.h material.h vdefs.h vcom.h
	g++ $(CXXFLAGS) $(includes) -c $< -o $@

pin.o : pin.cpp pin.h transform.h vrmlmat.h material.h vdefs.h vcom.h
	g++ $(CXXFLAGS) $(includes) -c $< -o $@

rectangle.o : rectangle.cpp polygon.h rectangle.h transform.h vrmlmat.h material.h vdefs.h vcom.h
	g++ $(CXXFLAGS) $(includes) -c $< -o $@

circle.o : circle.cpp polygon.h circle.h transform.h vrmlmat.h material.h vdefs.h vcom.h
	g++ $(CXXFLAGS) $(includes) -c $< -o $@

polygon.o : polygon.cpp polygon.h transform.h vrmlmat.h material.h vdefs.h vcom.h
	g++ $(CXXFLAGS) $(includes) -c $< -o $@

vrmlmat.o : vrmlmat.cpp vrmlmat.h material.h keyvalparser.h
	g++ $(CXXFLAGS) $(includes) -c $< -o $@

material.o : material.cpp material.h keyvalparser.h
	g++ $(CXXFLAGS) $(includes) -c $< -o $@

transform.o : transform.cpp transform.h
	g++ $(CXXFLAGS) $(includes) -c $< -o $@

keyvalparser.o : keyvalparser.cpp keyvalparser.h
	g++ $(CXXFLAGS) $(includes) -c $< -o $@

vcom.o : vcom.cpp vcom.h material.h vrmlmat.h
	g++ $(CXXFLAGS) $(includes) -c $< -o $@

.PHONY : clean
clean :
	- rm *.o
