# -*- coding: utf-8 -*-
import sys
from sage.all import QQ,PolynomialRing
import sparsesos

def SVor():
    SVor="""16a2(c2+1+d2)u4+16a(−cda2+axc+2ac2+2a+2ad2+ayd−
cd)u3+((24a2+4a4)c2+(−24da3−24ad−8ya3+24xa2−8ay)
c+24a2d2+4d2−8dxa3+4y2a2+24yda2−8axd+16a2+4x2a2)
u2+(−4ca3+4ya2−4ax−8ac+8da2+4d)(d−ac+y−ax)u+
(a2+1)(d−ac+y−ax)2"""
    SVor="".join(SVor.split())
def LL(n):
    x=PolynomialRing(QQ,n,"x").gens()
    #x=list(x)
    #x[-1]=1
    #x.append(x[0])
    f=0
    for i in range(n):
        f1=1;
        for j in range(n):
            if j!=i:
                f1=f1*(x[i]-x[j])
        f+=f1
    print(f)
    return f;

if __name__=="__main__":
    for i in [5,7]:
        print("========LL%d========="% i)
        f=LL(i)
        sparsesos.is_sparsesos(f)
