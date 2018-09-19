import sys
from sage.all import QQ,ZZ,PolynomialRing
import sparsesos

def mcp(n):
    def is_num(s):
        return s>="0"and s<="9"
    def fix(s):
        l=[]
        for i in range(len(s)):
            if s[i]=="+":
                l.append(s[i])
            elif is_num(s[i]):
                if s[i-1]!="+" and not is_num(s[i-1]):
                    l.append("**")
                l.append(s[i])
            else:
                if s[i-1]!="+":
                    l.append("*")
                l.append(s[i])
        return("".join(l))
    xyz=PolynomialRing(ZZ,8,"xyabcdef").gens()
    x=xyz[0]
    y=xyz[1]
    a=xyz[2]
    b=xyz[3]
    c=xyz[4]
    d=xyz[5]
    e=xyz[6]
    f=xyz[7]
    if n==12:
        p12=(8*y**2*b**2*c**2 + 8*a**4*f**2*e**2 + 2*a**2*c**2*e**4 + 24*x**2*y*e**2 + 48*a**2*x*y**2
+ 24*x*b**2*d**4 + 12*x**2*y*f**2 + 12*x*y**2*c**2 + 24*a**4*e**2*y
+ 28*a**2*b**2*d**4 + 4*a**2*b**2*e**4 + 28*a**2*y**2*b**2 + 12*a**4*f**2*y
+ 8*x**2*e**2*f**2 + 16*a**2*x*e**4 + 4*y*c**4*d**2 + 28*a**4*e**2*d**2 + a**2*b**2*f**4
+ 24*x*y**2*b**2 + 2*b**4*d**2*f**2 + 4*b**4*d**2*e**2 + 16*y*b**4*d**2 + 4*a**2*x*f**4
+ 48*a**2*x*d**4 + 14*a**2*y**2*c**2 + 14*a**2*c**2*d**4 + 8*b**2*c**2*d**4
+ 48*x**2*y*d**2 + 14*x**2*d**2*f**2 + 28*x**2*d**2*e**2 + 48*a**4*d**2*y
+ 24*x*y*c**2*d**2 + 4*x*y*c**2*e**2 + 4*x*y*c**2*f**2 + 8*x*c**2*d**2*e**2
+ 4*x*c**2*d**2*f**2 + 16*y*b**2*c**2*d**2 + 4*b**2*c**2*d**2*e**2 + b**2*c**2*d**2*f**2
+ 48*x*y*b**2*d**2 + 12*x*c**2*d**4 + c**4*d**2*e**2 + 2*y**2*c**4 + 2*c**4*d**4
+ 8*y**2*b**4 + 8*b**4*d**4 + 24*x**2*y**2 + 24*x**2*d**4 + 8*x**2*e**4 + 2*x**2*f**4
+ 8*x*y*b**2*e**2 + 4*x*y*b**2*f**2 + 16*x*b**2*d**2*e**2 + 8*x*b**2*d**2*f**2 + 2*a**4*f**4
+ 14*a**4*f**2*d**2 + 48*a**2*x*y*e**2 + 28*a**2*y*c**2*d**2 + 12*a**2*b**2*d**2*f**2
+ 24*a**2*x*y*f**2 + 28*a**2*x*d**2*f**2 + 56*a**2*x*d**2*e**2 + 4*a**2*b**2*e**2*f**2
+ a**2*c**2*e**2*f**2 + 8*a**2*y*b**2*f**2 + 56*a**2*y*b**2*d**2 + 16*a**2*y*b**2*e**2
+ 96*a**2*x*y*d**2 + 4*a**2*c**2*d**2*f**2 + 8*a**2*y*c**2*e**2 + 24*a**2*b**2*d**2*e**2
+ 16*a**2*x*e**2*f**2 + 12*a**2*c**2*d**2*e**2 + 4*a**2*y*c**2*f**2 + 24*a**4*y**2
+ 24*a**4*d**4 + 8*a**4*e**4)
        return (a**2+2*b**2+c**2)*p12;
    if n==13:
        p13=(2*x**2*e**2*f**2 + 12*y**2*a**4 + 12*a**4*d**4 + 4*a**4*e**4 
+ 4*y**2*b**4 + 4*b**4*d**4 + a**2*c**2*e**4 + b**4*d**2*f**2 
+ 4*x**2*e**4 + 24*x**2*y*d**2 + a**2*b**2*e**2*f**2 + 12*x**2*d**4 
+ b**2*c**2*d**2*e**2 + 48*x*y*a**2*d**2 + 12*x**2*y**2 + 12*x**2*y*e**2
+ 14*x**2*d**2*e**2 + 4*x**2*d**2*f**2 + 24*x*y**2*a**2 + 24*x*a**2*d**4 + 8*x*a**2*e**4
+ 12*x*y**2*b**2 + 12*x*b**2*d**4 + 24*y*a**4*d**2 + 12*y*a**4*e**2 + 14*a**4*d**2*e**2
+ 4*a**4*d**2*f**2 + 2*a**4*e**2*f**2+ 14*y**2*a**2*b**2+ 14*a**2*b**2*d**4+ 2*a**2*b**2*e**4
+ 4*y**2*a**2*c**2 + 4*a**2*c**2*d**4 + 8*y*b**4*d**2 + 24*x*y*a**2*e**2 + 28*x*a**2*d**2*e**2
+ 8*x*a**2*d**2*f**2 + 4*x*a**2*e**2*f**2 + 24*x*y*b**2*d**2 + 4*x*y*b**2*e**2
+ 8*x*b**2*d**2*e**2 + 4*x*b**2*d**2*f**2 + 28*y*a**2*b**2*d**2 + 8*y*a**2*b**2*e**2
+ 12*a**2*b**2*d**2*e**2 + 4*a**2*b**2*d**2*f**2 + 8*y*a**2*c**2*d**2 + 4*y*a**2*c**2*e**2
+ 4*a**2*c**2*d**2*e**2 + 4*y*b**2*c**2*d**2 + 2*b**4*d**2*e**2 + 2*y**2*b**2*c**2 + 2*b**2*c**2*d**4)
        return p13;
    if n==22:
        sp22="""24x2yf2+16yb4d2+40x2d2e2+4a4f2e2+16a4e2d2
    +72a2xy2+4b2c2d4+8a4f2d2+4yc4d2+8xc2d4
    +48xy2b2+20a2y2c2+16xb2d4+16a4e2y+48x2ye2
    +40a2xd4+4a2xf4+20x2d2f2+72x2yd2+c4d4
    +40a4d2y+40a2y2b2+16x2e2f2+8a4f2y+24xy2c2
    +4a4e4+16a4d4+16y2b2c2+8xyb2f2+16xyb2e2
    +56xyb2d2+16a2xe4+4y2c4+4b4d4+16y2b4
    +4x2f4+16x2e4+28x2d4+48x2y2+8xb2d2e2
    +4xb2d2f2+28xyc2d2+8xyc2e2+8xyc2f2
    +4xc2d2e2+4xc2d2f2+16yb2c2d2+16a2xe2f2
    +2a2c2d2f2+4a2b2d2f2+104a2xyd2+56a2xye2
    +28a2xyf2+24a2yc2d2+48a2yb2d2+24a2xd2f2
    +8a2yb2e2+4a2yc2e2+48a2xd2e2+4a2yb2f2
    +8a2b2d2e2+8a2c2d4+16a2b2d4+4a2c2d2e2
    +4a2yc2f2+28a4y2+a4f4"""
        
        sp22="".join(sp22.split())
        p22=eval(fix(sp22))
        return((a**2+2*b**2+c**2)*p22)
    if n==23:
        p23=(8*e**4*x**2 + 36*x**2*y*d**2 + 36*x*y**2*a**2 + 2*x**2*d**2*f**2 + 8*x*b**2*d**4
+ 24*x*y**2*b**2 + 20*x*a**2*d**4 + 24*e**2*x*a**2*d**2 + 28*e**2*x*y*a**2
+ 4*e**2*x*b**2*d**2 + 8*e**2*x*y*b**2 + 4*e**2*x*a**2*f**2 + 8*e**4*x*a**2 + 24*e**2*x**2*y
+ 4*e**2*x**2*f**2 + 20*e**2*x**2*d**2 + 8*b**4*y**2 + 2*d**4*b**4 + 4*c**2*b**2*y**2
+ d**4*c**2*b**2 + 8*d**2*b**4*y + 4*d**2*c**2*b**2*y + 2*y**2*a**2*c**2 + 2*a**2*c**2*d**4
+ 20*y**2*a**2*b**2 + 8*a**2*b**2*d**4 + 20*y*a**4*d**2 + 8*y*a**4*e**2 + 8*a**4*d**2*e**2
+ 2*a**4*d**2*f**2 + a**4*e**2*f**2 + 14*y**2*a**4 + 8*a**4*d**4 + 2*a**4*e**4
+ 4*y*a**2*c**2*d**2 + a**2*c**2*d**2*e**2 + 24*y*a**2*b**2*d**2 + 4*y*a**2*b**2*e**2
+ 4*a**2*b**2*d**2*e**2 + a**2*b**2*d**2*f**2 + 4*x*a**2*d**2*f**2 + 24*x**2*y**2
+ 14*x**2*d**4 + 52*x*y*a**2*d**2 + 28*x*y*b**2*d**2)
        return ((a**2+2*b**2+c**2)*p23)
if __name__=="__main__":
    n=int(input())
    f=mcp(n)
    B=False;
    if (B):
        sparsesos.is_sparsesos(f)
    else:
        dct=f.dict();
        l=list(dct)
        fout=open("P"+str(n)+"_data.txt","w")
        fout.write("%d %d\n" % (len(l),len(l[0])))
        for i in f.dict():
            fout.write(" ".join(map(str,i)))
            fout.write(" %d\n" % dct[i])
        fout.close()
        fout=open("P"+str(n)+"_data_2.txt","w")
        points=((QQ(1)/2)*f.newton_polytope()).integral_points()
        fout.write("%d %d\n" % (len(points),len(points[0])))
        for i in points:
            fout.write(" ".join(map(str,i)))
            fout.write("\n")
        fout.close()
