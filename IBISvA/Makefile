CXX=g++
CXXFLAGS=-g
CXXLIBS=-L/usr/local/lib -lwiringPi -lpugixml
IBIS_DEPS=         devParent.o devmgt.o devLED.o prgParent.o PrgMgmt.o prgLED.o
IBIS_HDRS=Makefile devParent.h devmgt.h devLED.h prgParent.h PrgMgmt.h prgLED.h

all: IBISvA

IBISvA: IBISvA.o $(IBIS_DEPS) $(IBIS_HDRS)
	$(CXX) $(CXXFLAGS) -o $@ $(IBIS_DEPS) $< $(CXXLIBS)

IBISvA.o: IBISvA.cpp $(IBIS_HDRS)
	$(CXX) $(CXXFLAGS) -c $<

devParent.o: devParent.cpp $(IBIS_HDRS)
	$(CXX) $(CXXFLAGS) -c $<

devmgt.o: devmgt.cpp devmgt.h $(IBIS_HDRS)
	$(CXX) $(CXXFLAGS) -c $<

devLED.o: devLED.cpp devLED.h $(IBIS_HDRS)
	$(CXX) $(CXXFLAGS) -c $<

prgParent.o: prgParent.cpp $(IBIS_HDRS)
	$(CXX) $(CXXFLAGS) -c $<

PrgMgmt.o: PrgMgmt.cpp PrgMgmt.h $(IBIS_HDRS)
	$(CXX) $(CXXFLAGS) -c $<

prgLED.o: prgLED.cpp prgLED.h $(IBIS_HDRS)
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f IBISvA *.o
