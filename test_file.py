import sys
from sage.all import ZZ,QQ,PolynomialRing
import sparsesos

if __name__=="__main__":
    S=raw_input();
    fin=open(S+".txt","r");
    s=fin.read();
    fin.close()
    f,R=sparsesos.strtoPolynomial(s);
    print(f);
    b=-1;
    if (b>0):
        sparsesos.is_sparsesos(f,newton_polytope=True)#com=sparsesos.com)
    if (b<0):
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
