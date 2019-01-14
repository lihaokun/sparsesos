SPARSESOS
================
Sparsesos is a simple implementation of determining whether a polynomial is cross sparse SOS polynomial.  It is licensed under the [Apache License 2.0](https://gitlab.com/haokunli/sparsesos/blob/master/LICENSE).

Dependencies
----------------
* [csdp](https://projects.coin-or.org/Csdp)
* [mosek](https://www.mosek.com/)

Install
---------------
Pleasa ensure that
*  csdp is in path, 
*  mosek for c++ is installed,
*  the lib and .h file of mosek is in path(You can set you mosek compile option by edit MOSEK  variable in src/makefile),
*  the license of mosek is valid.

For more information about mosek please visit the [link](https://www.mosek.com/documentation/).
```
chmod +x ./install
./install
```

Uninstall
---------------
```
chmod +x ./uninstall
./uninstall
```

Using
---------------
```
is_sos filename
```
Example
--------------
```
is_sos  example/F2.txt 
```

