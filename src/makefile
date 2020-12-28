CXX=g++
MOSEKlib=-lmosek64 -lfusion64
CFLAGS=-O3 -flto -pthread
IPATHS=-I../include 
LPATHS=-L../lib  '-Wl,-rpath=$$ORIGIN/../lib' -Wl,-rpath,../lib
ifdef mosek90
	MOSEKIPATHS=-I../mosek/9.0/tools/platform/linux64x86/h -I../mosek/9.0/tools/platform/linux64x86/include -D mosek_v=9
#	MOSEKLPATHS=-L../mosek/9.0/tools/platform/linux64x86/bin -Wl,-rpath-link,../mosek/9.0/tools/platform/linux64x86/bin '-Wl,-rpath=$$ORIGIN/../mosek/9.0/tools/platform/linux64x86/bin'
endif
ifdef mosek91
	MOSEKIPATHS=-I../mosek/9.1/tools/platform/linux64x86/h -I../mosek/9.1/tools/platform/linux64x86/include -D mosek_v=9
endif
ifdef mosek92
	MOSEKIPATHS=-I../mosek/9.2/tools/platform/linux64x86/h -I../mosek/9.2/tools/platform/linux64x86/include -D mosek_v=9
endif

ifdef mosek8
	MOSEKIPATHS=-I../mosek/8/tools/platform/linux64x86/h -I../mosek/8/tools/platform/linux64x86/include -D mosek_v=8
endif
#	MOSEKLPATHS=-L../mosek/8/tools/platform/linux64x86/bin -Wl,-rpath-link,../mosek/8/tools/platform/linux64x86/bin '-Wl,-rpath=$$ORIGIN/../mosek/8/tools/platform/linux64x86/bin'
ifndef MOSEKIPATHS
	MOSEKIPATHS=-I../mosek/9.2/tools/platform/linux64x86/h -I../mosek/9.2/tools/platform/linux64x86/include -D mosek_v=9
endif
objects=polynomial.o  sos.o sos_mosek.o is_sos.o   libsos.so  is_sos#sparsesos.o
is_sos: is_sos.o libsos.so
	install libsos.so ../lib
	$(CXX) $(CFLAGS) is_sos.o -o is_sos  $(LPATHS) -lsos
	install is_sos ../bin
libsos.so:polynomial.o sos_mosek.o sos.o
	$(CXX) $(CFLAGS) polynomial.o sos_mosek.o  sos.o -shared  -fPIC -o libsos.so $(LPATHS) $(MOSEKlib)
is_sos.o: is_sos.cpp ../include/polynomial/polynomial.hpp ../include/sos.hpp  ../include/sos_mosek.hpp
	$(CXX) $(CFLAGS) $(IPATHS) is_sos.cpp -c 
sos.o: sos.cpp ../include/polynomial/polynomial.hpp ../include/sos.hpp
	$(CXX) $(CFLAGS) $(IPATHS) sos.cpp -c
sos_mosek.o: sos_mosek.cpp ../include/sos.hpp ../include/sos_mosek.hpp ../include/polynomial/polynomial.hpp
	$(CXX) $(CFLAGS) $(IPATHS) $(MOSEKIPATHS) sos_mosek.cpp -c 	
polynomial.o: polynomial/polynomial.cpp ../include/polynomial/polynomial.hpp
	$(CXX) $(CFLAGS) $(IPATHS) polynomial/polynomial.cpp -c
.PHONY : clean
clean :
	-rm  $(objects)
	-rm ../bin/is_sos
	-rm ../lib/libsos.so
