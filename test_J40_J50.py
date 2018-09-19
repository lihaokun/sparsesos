import sys
from sage.all import ZZ,QQ,PolynomialRing
import sparsesos

if __name__=="__main__":
    print("1:j40\n2:j50\n?")
    i=int(input())
    j=["J40","J50"]
    fin=open(j[i-1]+".txt","r");
    s=fin.read();
    fin.close()
    f,R=sparsesos.strtoPolynomial(s);
    b=1;
    if (b>0):
        sparsesos.is_sparsesos(f,newton_polytope=True)#com=sparsesos.com)
    if (b<0):
        dct=f.dict();
        l=list(dct)
        fout=open(j[i-1]+"_data.txt","w")
        fout.write("%d %d\n" % (len(l),len(l[0])))
        for i in f.dict():
            fout.write(" ".join(map(str,i)))
            fout.write(" %d\n" % dct[i])
        fout.close()
