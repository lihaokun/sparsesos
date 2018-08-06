# -*- coding: utf-8 -*-
from __future__ import print_function
import sys
import os
def strtodict(s,d):
    def onem(s):
        co=[0]*n
        #if not("*" in s):
        #    return  tuple(co),int(s)
        l=s.split("*")
        if l[0][0]>="0" and l[0][0]<="9":
            st=1
            coeff=int(l[0])
        else:
            st=0;
            coeff=1
        co=[0]*n
        for i in range(st,len(l)):
            if "^" in l[i]:
                ll=l[i].split("^")
                co[d[ll[0]]]=int(ll[1])
            else:
                co[d[l[i]]]=1
        return(tuple(co),coeff)
    n=len(list(d))
    s="".join(s.split())
    #l=s.split("+")
    print("=============")
    ln=len(s)
    dct=dict()
    j=0;j1=1
    for i in range(ln):
        print("%0.2f%%\r" % ((i*100.0)/ln),end="")
        if s[i] in "+-":
            if j<i:
                co,coeff=onem(s[j:i])
                if co in dct:
                    print("error ........")
                dct[co]=j1*coeff
            j=i+1
            if s[i]=="+":
                j1=1
            else:
                j1=-1
    if j<i:
        co,coeff=onem(s[j:])
        dct[co]=j1*coeff

    return dct
def com(points,dct):
    def deep(i,l,ll):
        this=[None,ll[0]];
        ln=len(l)
        while this[1]!=None:
            j=this[1][0];
            l1=list_add(points[i],points[j])
            if k[j]==0 and (tuple_even(l1) or l1 in dct):
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
    n=len(points)
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
def strtoPolynomial_1(s):
    def is_good(x):
        return (x<="z" and x>="a") or (x<="Z" and x>="A") or (x<="9" and x>="0") or x in "()+-*^_ \n"
    def is_num(x):
        return (x<="9" and x>="0")
    def is_var(x):
        return (x<="z" and x>="a") or (x<="Z" and x>="A") or (x<="9" and x>="0") or x == "_"
    def is_op(x):
        return x in "+-*^()"
    def op(s,f1,f2):
        if s=="+":
            return f1+f2
        if s=="*":
            return f1*f2
        if s=="-":
            return f1-f2
        if s=="^":
            return f1**f2
    if type(s)!=str:
        print("str error.",s,"is not a str.")
        sys.exit(0)
    dct=dict()
    i=0;n=0;j=0
    L=[];
    for i in range(len(s)):
        print("%0.2f%%\r" % ((i*100.0)/len(s)/3),end="")
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
    N=len(L)
    if j<len(s):
        x=s[j:]
        L.append(x)
        if not(is_num(x[0])) and not (x in dct):
            dct[x]=n;
            n+=1;
    print(dct)
    if n>0:
        R=PolynomialRing(ZZ,n,"x")
        x=R.gens()
        for i in range(len(L)):
            print("%0.2f%%\r" % ((i*100.0)/N/3+33.333),end="")
            if L[i] in dct:
                L[i]=x[dct[L[i]]]
            elif not is_op(L[i]):
                L[i]=int(L[i])
    L1=[]#[(0,"+")]
    i=1;
    f=L[0]
    dop={"+":0,"-":0,"*":1,"^":2}

    while i<len(L):
        print("%0.2f%%\r" % ((i*100.0)/N/3+66.666),end="")
        o=L[i];
        if len(L1)!=0 and dop[L1[-1][1]]>=dop[o]:
            ll=L1.pop()
            f=op(ll[1],ll[0],f)
        elif i+2<len(L):
            i=i+1
            a=L[i];o1=L[i+1]
            if dop[o]>=dop[o1]:
                f=op(o,f,a)
            else:
                L1.append((f,o))
                f=a
            i=i+1
        else:
            i=i+1
            a=L[i]
            f=op(o,f,a)
            i=i+1
    if len(L1)!=0:
        for i in range(len(L1)-1,-1,-1):
            ll=L1.pop()
            f=op(ll[1],ll[0],f)
    print("done.   ")
    return f
def list_add(l1,l2):
    n=min(len(l1),len(l2))
    L=[0]*n
    for i in range(n):
        L[i]=l1[i]+l2[i]
    return(tuple(L))
def tuple_even(l):
    for i in l:
        if i % 2!=0:
            return False
    return True
def is_sparsesos(dct,f1="sparsesos.dat-s",f2="sparsesos.result",com=com):
    def half_tuple(l):
        co=[]
        for i in l:
            co.append(i//2)
        return(tuple(co))
    def check(p,d):
        n=len(p)
        s=0
        N=((n-1)*n)/2
        d1=dict()
        #print(p)
        for i in dct:
             d1[i]=False
        for i in range(n):
            for j in range(i,n):
                s=s+1
                print("%0.2f%%\r" % ((s*100.0)/N),end="")
                l=list_add(p[i],p[j])
                #print(l,end="")
                if (l in d1):
                    #print(True,end="")
                    d1[l]=True
                #print()
        for i in d1:
            if not(d1[i]):
                print("error",i)
                return False
        return True
    #dct=f.dict()
    points=[]
    for i in dct:
        #print("%0.2f%%\r" % ((i*100.0)/ln),end="")
        if tuple_even(i):
            points.append(half_tuple(i))
    print(len(points))
    print("check")
    print(check(points,dct))
    print("point done.")
    n=len(points)
    #R=[];
    #for i in range(n):
    #    R.append([0]*n)
    #for i in range(n):
    #    for j in range(i,n):
    #        l=list_add(points[i],points[j])
    #        if tuple_even(l) or l in dct:
    #            R[i][j]=1
    #            R[j][i]=1
    print("================")
    L=com(points,dct)
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
    FOUTL=[]
    P_points=[]
    for i in range(Ln):
        Ls.append(len(L[i]))
    if Ln<len(L):
        Ls.append(Ln-len(L))
    print(" ".join(map(str,Ls)))
    for i in range(Ln):
#        Ls.append(len(L[i]))
        for j in range(Ls[-1]):
            for k in range(j,Ls[-1]):
                lk=list_add(points[L[i][j]],points[L[i][k]])
                if lk in S:
                    FOUTL[S[lk]].append(" ".join([str(S[lk]+1),str(i+1),str(j+1),str(k+1),"1"]))
                else:
                    S[lk]=Sn
                    P_points.append(lk)
                    FOUTL.append([])
                    FOUTL[S[lk]].append(" ".join([str(S[lk]+1),str(i+1),str(j+1),str(k+1),"1"]))
                    Sn+=1
    if Ln<len(L):
#        Ls.append(Ln-len(L))
        for i in range(Ln,len(L)):
            lk=list_add(points[L[i][0]],points[L[i][0]])
            if lk in S:
                FOUTL[S[lk]].append(" ".join([str(S[lk]+1),str(Ln+1),str(i+1-Ln),str(i+1-Ln),"1"]))
            else:
                S[lk]=Sn
                P_points.append(lk)
                FOUTL.append([])
                FOUTL[S[lk]].append(" ".join([str(S[lk]+1),str(Ln+1),str(i+1-Ln),str(i+1-Ln),"1"]))
                Sn+=1

    #print(" ".join(map(str,Ls)))
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
    for i in range(Sn):
        fout.write("\n".join(FOUTL[i]))
        fout.write("\n")
    fout.close()
    print("=====file out end=====")
    #os.system("sdpa %s %s" % (f1,f2))
    os.system("csdp %s %s" % (f1,f2))

def j621():
    fin = open("J621.txt","r")
    s=fin.read()
    d={'a':0, 'c':1, 'b':2, 'e':3, 'd':4, 'g':5, 'f':6, 'i':7, 'h':8, 'k':9,
                            'j':10, 'm':11, 'l':12, 'n':13, 'p':14}

    #print(s[0])
    s="2*x^4+x^2*y^2+y^4-4*x^2*z-4*x*y*z-2*y^2*w+y^2-2*y*z+8*z^2-2*z*w+2*w^2"
    d={"x":0,"y":1,"z":2,"w":3}
    #s="x^4*y^2+x^2*y^4+1-3*x^2*y^2"
    #d={"x":0,"y":1}
    f=strtodict(s,d)
    #print(f)
    #{'a':0, 'c':1, 'b':2, 'e':3, 'd':4, 'g':5, 'f':6, 'i':7, 'h':8, 'k':9,
                #            'j':10, 'm':11, 'l':12, 'n':13, 'p':14})
    return f
def com_j621():
    dct=j621()
    l=len(dct)
    print("done.  ")
    print(l)
    #L=list(dct)
    is_sparsesos(dct)
    #fout=open("test_data.txt","w")
    #fout.write("%d %d\n" % (l,len(list(dct)[0])))
    #for i in dct:
    #    fout.write(" ".join(map(str,i)))
    #    fout.write(" %d\n" % dct[i])
    #fout.close()
if __name__=="__main__":
    com_j621()
