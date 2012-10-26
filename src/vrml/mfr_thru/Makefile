VPATH = ../../common:../../mat:../vcom
includes = -I ../../common -I ../../mat -I ../vcom

ALLOBJS = vrmlmat.o material.o keyvalparser.o transform.o vcom.o polygon.o resistor.o

CXXFLAGS = -g -O0

.PHONY : all
all :  $(ALLOBJS) makeMFR

.PHONY : objs
objs : $(ALLOBJS)

makeMFR : makeMFR.cpp $(ALLOBJS)
	g++ $(CXXFLAGS) $(includes) -lm $(ALLOBJS) $< -o $@

resistor.o : resistor.cpp resistor.h polygon.h transform.h vrmlmat.h material.h vdefs.h vcom.h
	g++ $(CXXFLAGS) $(includes) -c $< -o $@

polygon.o : polygon.cpp transform.h vrmlmat.h material.h vdefs.h vcom.h
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
