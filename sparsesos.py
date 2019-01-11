from __future__ import print_function
import sys
import os
from sage.all import ZZ,QQ,PolynomialRing
from sage.rings.polynomial.polydict import ETuple

def strtoPolynomial(s):
    def is_good(x):
        return (x<="z" and x>="a") or (x<="Z" and x>="A") or (x<="9" and x>="0") or x in "()+-*^_ \n"
    def is_num(x):
        return (x<="9" and x>="0")
    def is_var(x):
        return (x<="z" and x>="a") or (x<="Z" and x>="A") or (x<="9" and x>="0") or x == "_"
    def is_op(x):
        return x in "+-*^()"
    if type(s)!=str:
        print("str error.",s,"is not a str.")
        sys.exit(0)
    dct=dict()
    s="".join(s.split())
    i=0;n=0;j=0
    L=[];
    for i in range(len(s)):
        print("%0.2f%%\r" % ((i*100.0)/len(s)),end="")
        if not is_good(s[i]):
            print("polynomial error. %s,%d" % (s[i],i))
            sys.exit(0)
        if is_op(s[i]) or s[i] in " \n":
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
        R=PolynomialRing(ZZ,n,"x")
        x=R.gens()
        for i in range(len(L)):
            if L[i] in dct:
                L[i]="x["+str(dct[L[i]])+"]"
            if L[i]=="^":
                L[i]="**"
    s1="".join(L);
    print("read...done.")
    try:
        f=eval(s1);
    except:
        print("polynomial error. sage can't build this polynomial.")
        sys.exit(0)
    else:
        print(len(f.dict()))
    if n==0:
        R=None
    return f,R
def com(R):
    def deep(i,l,ll):
        this=[None,ll[0]];
        ln=len(l)
        while this[1]!=None:
            j=this[1][0];
            if k[j]==0 and R[i][j]:
                k[j]=1;
                l.append(j)
                if this[0]==None:
                    ll[0]=this[1]
                    this=[None,this[1]]
                else:
                    this[0][1]=this[1][1]
                    this=[this[0],this[1][1]]
            else:
                this=[this[1],this[1][1]]
        for j in range(ln,len(l)):
            deep(l[j],l,ll)
    n=len(R)
    k=[0]*n
    L=[]
    head=[0,None];end=head
    ll=[head]
    for i in range(1,n):
        end[1]=[i,None]
        end=end[1]
    for i in range(n):
        if k[i]==0:
            l=[]
            deep(i,l,ll);
            l.sort()
            L.append(l)
    return(L)
def com1(R):
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
def com2(R):
    n=len(R)
    return [list(range(n))]
def is_sparsesos(f,f1="sparsesos.dat-s",f2="sparsesos.result",newton_polytope=True,com=com):
    def tuple_even(l):
        for i in l:
            if i % 2!=0:
                return False
        return True
    def getpoint(mn,mx,n):
        if n==1:
            return [ETuple([m])]
        co=[0]*(n)
        L=[]
        for m in range(mn,mx+1):
            co=[0]*(n);i=n-1;sm=m;#s=0
            while i>=0:
                if i==n-1:
                    co[i]=sm
                    L.append(tuple(co))
                    #s=s+1
                    i=i-1;
                    co[i]+=1;
                    sm-=1
                elif sm>=0:
                    i=i+1;
                    co[i]=0;
                else:
                    sm+=co[i]
                    i=i-1
                    co[i]+=1;
                    sm-=1
            #print(m,s)
        return L;
    def list_add(l1,l2):
        n=min(len(l1),len(l2))
        L=[0]*n
        for i in range(n):
            L[i]=l1[i]+l2[i]
        return(ETuple(L))
    def test(a,b):
        for i in a:
            if not(list_add(i,i) in b):
                print(i)
                return False
        return True
    dct=f.dict()
    if newton_polytope:
        P1=f.newton_polytope()
        P2=(QQ(1)/2)*P1
        points=P2.integral_points()
        #print(test(points,dct))
        print(len(points))
    else:
        L=map(sum,list(dct))
        mx=max(L);mn=min(L)
        if mx%2!=0 or mn%2!=0:
            print("not sos!")
            sys.exit(0)
        print(mn,mx)
        points=getpoint(mn//2,mx//2,len(list(dct)[0]))
        print(len(points))
    n=len(points)
    R=[];
    for i in range(n):
        R.append([0]*n)
    for i in range(n):
        for j in range(i,n):
            l=list_add(points[i],points[j])
            if tuple_even(l) or l in dct:
                R[i][j]=1
                R[j][i]=1
    print("================")
    L=com(R)
    L.sort(key=lambda x:-len(x))
    #print(L)
    print("================")
    Ln=len(L)
    Ln1=Ln;
    while Ln>0 and len(L[Ln-1])==1:
        Ln-=1
    S=dict()
    Sn=0
    Ls=[]
    #FOUTL=[]
    FOUTLS=[];
    P_points=[]
    for i in range(Ln):
        Ls.append(len(L[i]))
        for j in range(Ls[-1]):
            for k in range(j,Ls[-1]):
                lk=list_add(points[L[i][j]],points[L[i][k]])
                if lk in S:
                    FOUTLS.append(" ".join([str(S[lk]+1),str(i+1),str(j+1),str(k+1),"1"]))
                    #FOUTL[S[lk]].append(" ".join([str(S[lk]+1),str(i+1),str(j+1),str(k+1),"1"]))
                else:
                    S[lk]=Sn
                    P_points.append(lk)
                    #FOUTL.append([])
                    FOUTLS.append(" ".join([str(S[lk]+1),str(i+1),str(j+1),str(k+1),"1"]))
                    Sn+=1
    if Ln<len(L):
        Ls.append(Ln-len(L))
        for i in range(Ln,len(L)):
            lk=list_add(points[L[i][0]],points[L[i][0]])
            if lk in S:
                FOUTLS.append(" ".join([str(S[lk]+1),str(Ln+1),str(i+1-Ln),str(i+1-Ln),"1"]))
            else:
                S[lk]=Sn
                P_points.append(lk)
                #FOUTL.append([])
                FOUTLS.append(" ".join([str(S[lk]+1),str(Ln+1),str(i+1-Ln),str(i+1-Ln),"1"]))
                Sn+=1

    print(" ".join(map(str,Ls)))
    #print("\n".join(map(str,L)))
    print("====file out begin=====")
    fout=open(f1,"w");
    fout.write("%d=mDIM\n%d=nBLOCK\n"% (len(P_points),len(Ls)))
    fout.write(" ".join(map(str,Ls))+"= bLOCKsTRUCT\n")
    c=[]
    for i in P_points:
        #i1=ETuple(tuple(i))
        if i in dct:
            c.append(dct[i])
        else:
            c.append(0)
    fout.write(",".join(map(str,c))+"\n")
    ni=-1;nn=len(P_points)
    #for i in range(Sn):
    fout.write("\n".join(FOUTLS))
    fout.write("\n")
    fout.close()
    print("=====file out end=====")
    #os.system("sdpa %s %s" % (f1,f2))
    os.system("csdp %s %s" % (f1,f2))


if __name__=="__main__":
    if len(sys.argv)<2:
        #help()
        file=raw_input("file_name=")
        fin=open(file,"r");
        s=fin.read()
        fin.close()
    else:
        #print(sys.argv[1])
        s=sys.argv[1];
    f,R=strtoPolynomial(s)
    is_sparsesos(f)
