SPARSESOS
================
Sparsesos is a simple implementation of determining whether a polynomial is cross sparse SOS polynomial.

Dependencies
----------------
* [sagemath](https://www.sagemath.org/)
* [sdpa](http://sdpa.sourceforge.net/)
* [python2](https://www.python.org/)

Using
---------------
```
python2 sparsesos.py   "polynomial"
```
Example
--------------
```
python2 sparsesos.py "2*X^4+x^2*y^2+y^4-4*x^2*z-4*x*y*z-2*y^2*w+y^2-2*y*z+8*z^2-2*z*w+2*w^2"
```
