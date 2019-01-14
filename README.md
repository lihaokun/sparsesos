SPARSESOS
================
Sparsesos is a simple implementation of determining whether a polynomial is cross sparse SOS polynomial.

Dependencies
----------------
* [csdp](https://projects.coin-or.org/Csdp)
* [mosek](https://www.mosek.com/)

Install
---------------
Pleasa ensure that
*  csdp is in path, 
*  mosek for c++ is installed,
*  the license of mosek is valid.

For more information about mosek please visit the [link](https://www.mosek.com/documentation/).
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
