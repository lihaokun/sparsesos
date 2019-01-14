CXX=g++
MOSEK=-lmosek64 -lfusion64
CFLAGS=-O3 -flto
objects=polynomial.o  sparsesos.o sos.o is_sos.o 
is_sos: $(objects)  
	$(CXX) $(CFLAGS) $(objects) $(MOSEK) -o is_sos 
is_sos.o: is_sos.cpp sparsesos.h polynomial/polynomial.hpp sos.hpp
	$(CXX) $(CFLAGS) is_sos.cpp -c 
sos.o: sos.cpp polynomial/polynomial.hpp sos.hpp
	$(CXX) $(CFLAGS) sos.cpp -c
sparsesos.o: sparsesos.cpp sparsesos.h polynomial/polynomial.hpp
	$(CXX) $(CFLAGS) sparsesos.cpp -c
polynomial.o: polynomial/polynomial.cpp polynomial/polynomial.hpp
	$(CXX) $(CFLAGS) polynomial/polynomial.cpp -c
.PHONY : clean
clean :
	-rm  $(objects)
	-rm is_sos
