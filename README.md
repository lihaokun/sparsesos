SPARSESOS
================
Sparsesos is a simple implementation of determining whether a polynomial is sparse SOS polynomial.  It is licensed under the [Apache License 2.0](https://gitlab.com/haokunli/sparsesos/blob/master/LICENSE).

Dependencies
----------------
* [mosek 8.1 or mosek 9.0](https://www.mosek.com/) 

The intaller will install  mosek automatically, but you need to get a mosek lincense.

Windows 10 X64 binary  package
---------------
You can download  binary  package at

* [mega](https://mega.nz/#!vjRGDYSY!Rrm-QninwxZHLFIeO11W--QJ6c5VEA8YcAXOPy6I8K4)
* [baidu](https://pan.baidu.com/s/11J9k2F7EA315iDcOByZKIA)  pw:s7fs  

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

Linux Uninstall
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
