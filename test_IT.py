import sys
from sage.all import QQ,ZZ,PolynomialRing
import sparsesos

def IT():
    y=PolynomialRing(ZZ,4,"acde").gens();
    a=y[0];
    c=y[1];
    d=y[2];
    e=y[3];
    f=(1 - 8*c**2*d**2 - 196608*e**3*a**2*d**2*c + 1536*e*a*d**4*c**2 + 21504*e**2*a*d**2*c - 4096*e**2*a*c**3*d**2-
       384*e*a*d**2+ 1024*e**2*a*c + 16*c**4*d**4 - 72*c**2*d**4 + 1024*c**2*e**2 + 36864*e**2*a**2*d**4-3456*e*a*d**4+ 
       262144*e**4*a**2*c**2 - 32768*e**3*a*c**2 + 256*c**3*d**2*e-576*c*e*d**2 + 81*d**4 + 64*c*e - 18*d**2)
    return(f)

def LL5():
    y=PolynomialRing(ZZ,4,"abcd").gens();
    a=y[0];
    b=y[1];
    c=y[2];
    d=y[3];
    

    f=(a**2+b**2+c**2+d**2)*(a**4 + 3*a**3*b + 3*a**2*b**2 + 2*a**3*c + 4*a**2*b*c + a**2*c**2 + a*b*c**2 +b**2*c**2 + a**3*d + 2*a**2*b*d + a**2*c*d
    + a*b*c*d + b**2*c*d + a*b*d**2 +b**2*d**2 + 2*a*c*d**2 + 4*b*c*d**2 + 3*c**2*d**2 + a*d**3 + 2*b*d**3 +3*c*d**3+d**4 )
    return f;
if __name__=="__main__":
    L=[(IT,"IT"),(LL5,"LL5")];
    i=1
    S=L[i][1]
    f=L[i][0]()
    b=-2;
    if (b>0):
        sparsesos.is_sparsesos(f,newton_polytope=False)
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
        


