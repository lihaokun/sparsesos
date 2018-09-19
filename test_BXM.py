import sys
from sage.all import ZZ,PolynomialRing
import sparsesos

def BXM(m):
    y=PolynomialRing(ZZ,3*m+2,"x").gens();
    x=[]
    n=3*m+2
    f1=0;f2=0;
    for i in range(n):
        x.append(y[i]**2)
    for i in range(3*(m+1)):
        x.append(x[i])
    for i in range(n):
        f1+=x[i]
        for j in range(m+1):
            f2+=x[i]*x[i+3*j+1]
    f=(f1**2-2*f2)*f1;
    print(len(f.dict()))
    return(f)

if __name__=="__main__":
    if len(sys.argv)<2:
        m=input("m=")
    else:
        #print(sys.argv[1])
        m=sys.argv[1]
    f=BXM(int(m))
    b=-1;
    if (b>0):
        sparsesos.is_sparsesos(f,newton_polytope=False)
    if (b<0):
        dct=f.dict();
        l=list(dct)
        fout=open("BXM_"+m+"_data.txt","w")
        fout.write("%d %d\n" % (len(l),len(l[0])))
        for i in f.dict():
            fout.write(" ".join(map(str,i)))
            fout.write(" %d\n" % dct[i])
        fout.close()

