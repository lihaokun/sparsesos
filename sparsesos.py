import sys
import os
from sage.all import QQ,PolynomialRing
from sage.rings.polynomial.polydict import ETuple
def strtoPolynomial(s):
    def is_good(x):
        return (x<="z" and x>="a") or (x<="Z" and x>="A") or (x<="9" and x>="0") or x in "()+-*^_"
    def is_num(x):
        return (x<="9" and x>="0")
    def is_var(x):
        return (x<="z" and x>="a") or (x<="Z" and x>="A") or (x<="9" and x>="0") or x == "_"
    def is_op(x):
        return x in "+-*^()"
    def skip(i):
        while s[i]==" " and i<len(s):
            i+1
        return i;
    if type(s)!=str:
        print("str error.",s,"is not a str.")
        sys.exit(0)
    dct=dict()
    i=0;n=0;j=0
    L=[];
    for i in range(len(s)):
        if not is_good(s[i]):
            print("polynomial error. %s" % s[i])
        if is_op(s[i]) or s[i]==" ":
            if j<i:
                x=s[j:i]
                L.append(x)
                #print(x,x[0])
                if not(is_num(x[0])) and not (x in dct):
                    #print(n)
                    dct[x]=n;
                    n+=1
            if is_op(s[i]):
                L.append(s[i])
            j=i+1
    if j<len(s):
        x=s[j:]
        L.append(x)
        if not(is_num(x[0])) and not (x in dct):
            dct[x]=n;
            n+=1;
    if n>0:
        R=PolynomialRing(QQ,n,"x")
        x=R.gens()
        for i in range(len(L)):
            if L[i] in dct:
                L[i]="x["+str(dct[L[i]])+"]"
            if L[i]=="^":
                L[i]="**"
    s1="".join(L);
    #print(s1)
    try:
        f=eval(s1);
    except:
        print("polynomial error. sage can't build this polynomial.")
        sys.exit(0)
    else:
        print(f)
    if n==0:
        R=None
    return f,R
def com(R):
    n=len(R);
    for k in range(n):
        for i in range(n):
            for j in range(n):
                if not R[i][j] and R[i][k] and R[j][k] :
                    R[i][j]=1
                    R[j][i]=1
    L=[];k=[0]*n;
    for i in range(n):
        if k[i]==0:
            L.append([])
            for j in range(n):
                if R[i][j]:
                    k[j]=1
                    L[-1].append(j)
    return L

def is_sparsesos(f,f1="sparsesos.dat-s",f2="sparsesos.result"):
    def tuple_even(l):
        for i in l:
            if i % 2!=0:
                return False
        return True
    #print(f)
    dct=f.dict()
    P1=f.newton_polytope()
    #P_points=P1.integral_points()
    P2=(QQ(1)/2)*P1
    points=P2.integral_points()
    print(len(points))
    #print(dct,points,P_points,P2,P1)
    n=len(points)
    R=[];
    for i in range(n):
        R.append([0]*n)
        for j in range(n):
            l=ETuple(tuple(points[i]+points[j]))
            #print(l,dct)
            #l in dct
            if tuple_even(l) or l in dct:
                R[i][j]=1
    L=com(R)
    L.sort(key=lambda x:-len(x))
    Ln=len(L)
    Ln1=Ln;
    while Ln>0 and len(L[Ln-1])==1:
        Ln-=1

    S=set()
    #for i in range(len(L)):
    #    co2=[]
    #    for j in range(len(L[i])):
    #        co2.append(points[L[i][j]])
    #    print(co2)
    co=[]
    Ls=[]
    for i in range(Ln):
        co1=[]
        Ls.append(len(L[i]))
        for j in range(Ls[-1]):
            co1.append([])
            for k in range(Ls[-1]):
                lk=ETuple(tuple(points[L[i][j]]+points[L[i][k]]))
                co1[-1].append(lk)
                S.add(lk)
        co.append(co1)
    co1=[]
    if Ln<len(L):
        Ls.append(Ln-len(L))
        for i in range(Ln,len(L)):
            lk=ETuple(tuple(points[L[i][0]]+points[L[i][0]]))
            co1.append(lk)
            S.add(lk)
        co.append(co1)
    P_points=[None]
    P_points.extend(list(S))
#    print(P_points)
    fout=open(f1,"w");
    fout.write("%d=mDIM\n%d=nBLOCK\n"% (len(P_points)-1,len(Ls)))
    fout.write(" ".join(map(str,Ls))+"= bLOCKsTRUCT\n")
    c=[]
    for i in P_points[1:]:
        #i1=ETuple(tuple(i))
        if i in dct:
            c.append(dct[i])
        else:
            c.append(0)
    fout.write(",".join(map(str,c))+"\n")
    ni=-1
    for i in P_points:
        ni=ni+1
        for j in range(Ln):
            for k1 in range(len(L[j])):

                for k2 in range(k1,len(L[j])):
                    if co[j][k1][k2]==i:
                        fout.write(" ".join([str(ni),str(j+1),str(k1+1),str(k2+1),"1"])+"\n")
        for j in range(-Ls[-1]):
            if co[-1][j]==i:
                fout.write(" ".join([str(ni),str(Ln+1),str(j+1),str(j+1),"1"])+"\n")




    fout.close()
    #os.system("sdpa %s %s" % (f1,f2))
    os.system("csdp %s %s" % (f1,f2))


if __name__=="__main__":
    if len(sys.argv)<2:
        #help()
        pass
    else:
        #print(sys.argv[1])
        f,R=strtoPolynomial(sys.argv[1])
        is_sparsesos(f)
