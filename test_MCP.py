import sys
from sage.all import QQ,PolynomialRing
import sparsesos

def mcp():
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
    xyz=PolynomialRing(QQ,8,"xyabcdef").gens()
    x=xyz[0]
    y=xyz[1]
    a=xyz[2]
    b=xyz[3]
    c=xyz[4]
    d=xyz[5]
    e=xyz[6]
    f=xyz[7]
    sp22="".join(sp22.split())
    p22=eval(fix(sp22))
    return((a**2+2*b**2+c**2)*p22)
if __name__=="__main__":
    f=mcp()
    sparsesos.is_sparsesos(f,newton_polytope=True)
