import sys
import random
import time
from sage.all import ZZ,QQ,PolynomialRing
##10 6 10 0.02
def init(n):
        x=[0]*n
        x[-1]=-1
        return x
def next(x,m):
    i=len(x)-1
    s=sum(x)
    while i>=0 and s==m:
        s-=x[i]
        x[i]=0
        i-=1
    if i<0: 
        return False
    x[i]+=1
    return True

def random_poly(n,m,l=0.1,nl=10):
    xgen=PolynomialRing(ZZ,n,"x").gens()
    f=0
    x=init(n)
    while next(x,m):
        if random.random()<l:
            mom=random.randint(-nl,nl)
            for i in range(n):
                mom*=xgen[i]**x[i]
            f+=mom
    return f
def random_polys(n,m,d,l=0.1,nl=10):
    xgen=PolynomialRing(ZZ,n,"x").gens()
    f=[0]*d
    x=init(n)
    while next(x,m):
        if random.random()<l:
            mom=random.randint(-nl,nl)
            for i in range(n):
                mom*=xgen[i]**x[i]
            i=random.randint(0,d-1)
            f[i]+=mom
    return f

if __name__ == "__main__":
    if len(sys.argv)>3:
        n=int(sys.argv[1])
        m=int(sys.argv[2])
        d=int(sys.argv[3])
        if len(sys.argv)>4:
            l=float(sys.argv[4])
    else:
        n=input("n=")
        m=input("m=")
        d=input("d=")
        l=input("l=")
    f=0
    for i in random_polys(n,m,d,l):
        #print(i)
        f+=i**2
    #print(f)
    print(len(f.dict()))
    s="randompoly_%d_%d_%d_%.4f.txt" % (n,m,d,l)#int(time.time()) % 10000)
    fout=open(s,"w")
    fout.write(str(f))
    fout.close()