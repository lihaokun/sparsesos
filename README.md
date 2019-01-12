SPARSESOS
================
Sparsesos is a simple implementation of determining whether a polynomial is cross sparse SOS polynomial.

Dependencies
----------------
* [csdp](https://projects.coin-or.org/Csdp)
* [mosek](https://www.mosek.com/)

Install
---------------
```
g++ polynomial/polynomial.cpp sos.cpp sparsesos.cpp is_sos.cpp -o is_sos -lmosek64 -lfusion64 -pthread -O3
```

Using
---------------
```
./is_sos filename
```
Example
--------------
```
./is_sos  example/F2.txt 
```
