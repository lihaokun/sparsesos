CXX=g++
MOSEK=-lmosek64 -lfusion64
CFLAGS=-O3 -flto -pthread 
ifdef mosek9
	IPATHS=-I../mosek/9.0/tools/platform/linux64x86/h -I../mosek/9.0/tools/platform/linux64x86/include -D mosek_v=9
	LPATHS=-L../mosek/9.0/tools/platform/linux64x86/bin -Wl,-rpath-link,../mosek/9.0/tools/platform/linux64x86/bin '-Wl,-rpath=$$ORIGIN/../mosek/9.0/tools/platform/linux64x86/bin'
else
	IPATHS=-I../mosek/8/tools/platform/linux64x86/h -I../mosek/8/tools/platform/linux64x86/include
	LPATHS=-L../mosek/8/tools/platform/linux64x86/bin -Wl,-rpath-link,../mosek/8/tools/platform/linux64x86/bin '-Wl,-rpath=$$ORIGIN/../mosek/8/tools/platform/linux64x86/bin'
endif

objects=polynomial.o  sos.o sos_mosek.o is_sos.o   #sparsesos.o
is_sos: $(objects)  
	$(CXX) $(CFLAGS) $(IPATHS) $(LPATHS) $(objects)  -o is_sos $(MOSEK) 
is_sos.o: is_sos.cpp sparsesos.h polynomial/polynomial.hpp sos.hpp  sos_mosek.hpp
	$(CXX) $(CFLAGS) is_sos.cpp -c 
sos.o: sos.cpp polynomial/polynomial.hpp sos.hpp
	$(CXX) $(CFLAGS) sos.cpp -c
sos_mosek.o: sos_mosek.cpp sos.hpp sos_mosek.hpp polynomial/polynomial.hpp
	$(CXX) $(CFLAGS) $(IPATHS) sos_mosek.cpp -c	
#sparsesos.o: sparsesos.cpp sparsesos.h polynomial/polynomial.hpp
#	$(CXX) $(CFLAGS) sparsesos.cpp -c
polynomial.o: polynomial/polynomial.cpp polynomial/polynomial.hpp
	$(CXX) $(CFLAGS) polynomial/polynomial.cpp -c
.PHONY : clean
clean :
	-rm  $(objects)
	-rm is_sos
