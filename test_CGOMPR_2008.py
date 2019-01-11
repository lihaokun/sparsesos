# -*- coding: utf-8 -*-
import sys
from sage.all import ZZ,QQ,PolynomialRing
import sparsesos

def Vor():
    z=PolynomialRing(ZZ,6,"xyacdu").gens()
    x=z[0]
    y=z[1]
    a=z[2]
    c=z[3]
    d=z[4]
    u=z[5]
    vor=(16*a**2*(c**2 + 1 + d**2)*u**4 + 16*a*(-c*d*a**2 + a*x*c + 2*a*c**2 + 2*a + 2*a*d**2 + a*y*d-
c*d)*u**3 + ((24*a**2 + 4*a**4)*c**2 + (-24*d*a**3 - 24*a*d - 8*y*a**3 + 24*x*a**2 - 8*a*y)*c
 + 24*a**2*d**2 + 4*d**2 - 8*d*x*a**3 + 4*y**2*a**2 + 24*y*d*a**2 - 8*a*x*d + 16*a**2 + 4*x**2*a**2)*u**2
  + (-4*c*a**3 + 4*y*a**2 - 4*a*x - 8*a*c + 8*d*a**2 + 4*d)*(d - a*c + y - a*x)*u+
(a**2 + 1)*(d - a*c + y - a*x)**2)
    #SVor="".join(SVor.split())
    return vor,vor.discriminant(u);
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
    #for i in [5,7]:
    #    print("========LL%d========="% i)
    #    f=LL(i)
    #    sparsesos.is_sparsesos(f)
    f,g=Vor();
    print(f);
    #print(g)
    b=False
    S="vor"
    if b:
        sparsesos.is_sparsesos(f)
    else:
        dct=f.dict();
        l=list(dct)
        fout=open(S+"_data.txt","w")
        fout.write("%d %d\n" % (len(l),len(l[0])))
        for i in f.dict():
            fout.write(" ".join(map(str,i)))
            fout.write(" %d\n" % dct[i])
        fout.close()
        fout=open(S+"_data_2.txt","w")
        points=((QQ(1)/2)*f.newton_polytope()).integral_points()
        fout.write("%d %d\n" % (len(points),len(points[0])))
        for i in points:
            fout.write(" ".join(map(str,i)))
            fout.write("\n")
        fout.close()
        f=g;S="vor2"
        dct=f.dict();
        l=list(dct)
        fout=open(S+"_data.txt","w")
        fout.write("%d %d\n" % (len(l),len(l[0])))
        for i in f.dict():
            fout.write(" ".join(map(str,i)))
            fout.write(" %d\n" % dct[i])
        fout.close()
        fout=open(S+"_data_2.txt","w")
        points=((QQ(1)/2)*f.newton_polytope()).integral_points()
        fout.write("%d %d\n" % (len(points),len(points[0])))
        for i in points:
            fout.write(" ".join(map(str,i)))
            fout.write("\n")
        fout.close()
