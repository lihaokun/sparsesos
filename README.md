SPARSESOS
================
Sparsesos is a simple implementation of determining whether a polynomial is sparse SOS polynomial.  It is licensed under the [Apache License 2.0](https://gitlab.com/haokunli/sparsesos/blob/master/LICENSE).

Dependencies
----------------
* [mosek 8.1 or mosek 9.0](https://www.mosek.com/) 

The intaller will install  mosek automatically, but you need to get a mosek lincense.

Linux Install  
---------------
Pleasa ensure that the license of mosek is valid.(~/mosek/mosek.lic)
You can get a free academic license on this [link](https://www.mosek.com/products/academic-licenses/).

```
git clone https://gitlab.com/haokunli/sparsesos.git
cd sparsesos
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
if solving successful, the result will be saved to filename.sosd .
Example
--------------
```
is_sos  example/F2.txt 
```

