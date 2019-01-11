SPARSESOS
================
Sparsesos is a simple implementation of determining whether a polynomial is cross sparse SOS polynomial.

Dependencies
----------------
* [sagemath](https://www.sagemath.org/)
* [csdp](https://projects.coin-or.org/Csdp)
* [python2](https://www.python.org/)
* [mosek](https://www.mosek.com/)

Install
---------------
```
g++ polynomial/polynomial.cpp sos.cpp sparsesos.cpp is_sos.cpp -o is_sos -lmosek64 -lfusion64 -pthread -O3
```

Using
---------------
```
python2 IS_SOS.py   filename
```
Example
--------------
```
python2 IS_SOS.py example/F2.txt 
```
