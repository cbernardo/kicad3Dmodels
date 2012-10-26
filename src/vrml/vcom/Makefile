VPATH = ../../common:../../mat
includes = -I ../../common -I ../../mat

ALLOBJS = vrmlmat.o material.o keyvalparser.o transform.o vcom.o polygon.o

.PHONY : all
all :  $(ALLOBJS) testRing

.PHONY : objs
objs : $(ALLOBJS)

testRing : testRing.cpp $(ALLOBJS)
	g++ $(includes) -lm $(ALLOBJS) $< -o $@

polygon.o : polygon.cpp transform.h vrmlmat.h material.h vdefs.h vcom.h
	g++ $(includes) -c $< -o $@

vrmlmat.o : vrmlmat.cpp vrmlmat.h material.h keyvalparser.h
	g++ $(includes) -c $< -o $@

material.o : material.cpp material.h keyvalparser.h
	g++ $(includes) -c $< -o $@

transform.o : transform.cpp transform.h
	g++ $(includes) -c $< -o $@

keyvalparser.o : keyvalparser.cpp keyvalparser.h
	g++ $(includes) -c $< -o $@

vcom.o : vcom.cpp vcom.h material.h vrmlmat.h
	g++ $(includes) -c $< -o $@

.PHONY : clean
clean :
	- rm *.o
