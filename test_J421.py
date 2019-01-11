# -*- coding: utf-8 -*-
import sys
from sage.all import ZZ,QQ,PolynomialRing
import sparsesos

def j421():
    x=PolynomialRing(ZZ,6,"abcdef").gens()
    a=x[0]
    b=x[1]
    c=x[2]
    d=x[3]
    e=x[4]
    f=x[5]
    j421=(a**2 + a**2*b**2 - a*b*c + 2*a**2*c**2 + b**2*c**2 - 2*a*b*c**3 + a**2*c**4 + a**2*d**2 + a**2*b**2*d**2 - a*b*c*d**2 + 2*a**2*c**2*d**2 + b**2*c**2*d**2 - 2*a*b*c**3*d**2 + a**2*c**4*d**2 - a*d*e - a*b**2*d*e - a*c**2*d*e + 2*a**2*e**2 + 2*a**2*b**2*e**2 - a*b*c*e**2 + 3*a**2*c**2*e**2 +
  b**2*c**2*e**2 - 2*a*b*c**3*e**2 + a**2*c**4*e**2 + d**2*e**2 + b**2*d**2*e**2 + c**2*d**2*e**2 - 2*a*d*e**3 - 2*a*b**2*d*e**3 - 2*a*c**2*d*e**3 + a**2*e**4 + a**2*b**2*e**4 + a**2*c**2*e**4 + a*c*d*f - b*c**2*d*f + a*c**3*d*f + a*b*e*f - 2*a**2*c*e*f - b**2*c*e*f + 3*a*b*c**2*e*f -
  2*a**2*c**3*e*f - c*d**2*e*f - 2*b**2*c*d**2*e*f + 2*a*b*c**2*d**2*e*f - b*d*e**2*f + 3*a*c*d*e**2*f + 2*a*b**2*c*d*e**2*f - 2*b*c**2*d*e**2*f - 2*a**2*b*c**2*d*e**2*f + 2*a*c**3*d*e**2*f + a*b*e**3*f - 2*a**2*c*e**3*f + 2*a*b*c**2*e**3*f - 2*a**2*c**3*e**3*f + 2*a**2*f**2 +
  2*a**2*b**2*f**2 - 3*a*b*c*f**2 + 5*a**2*c**2*f**2 + 3*b**2*c**2*f**2 - 6*a*b*c**3*f**2 + 3*a**2*c**4*f**2 + a**2*d**2*f**2 + c**2*d**2*f**2 + 2*a**2*c**2*d**2*f**2 + b**2*c**2*d**2*f**2 - 2*a*b*c**3*d**2*f**2 + a**2*c**4*d**2*f**2 - 2*a*d*e*f**2 + 2*b*c*d*e*f**2 - 4*a**2*b*c*d*e*f**2 -
  4*a*c**2*d*e*f**2 + 3*a**2*e**2*f**2 + b**2*e**2*f**2 + 2*a**2*b**2*e**2*f**2 - 4*a*b*c*e**2*f**2 + 4*a**2*c**2*e**2*f**2 + b**2*c**2*e**2*f**2 - 2*a*b*c**3*e**2*f**2 + a**2*c**4*e**2*f**2 + d**2*e**2*f**2 - 2*a*d*e**3*f**2 + a**2*e**4*f**2 - 2*a**2*b*d*f**3 + 2*a*c*d*f**3 - 3*b*c**2*d*f**3 +
  3*a*c**3*d*f**3 + 2*a*b*e*f**3 - 6*a**2*c*e*f**3 - 3*b**2*c*e*f**3 + 9*a*b*c**2*e*f**3 - 6*a**2*c**3*e*f**3 - 2*c*d**2*e*f**3 - 2*b*d*e**2*f**3 + 6*a*c*d*e**2*f**3 + 2*a*b*e**3*f**3 - 4*a**2*c*e**3*f**3 + a**2*b**2*f**4 - 2*a*b*c*f**4 + 3*a**2*c**2*f**4 + 3*b**2*c**2*f**4 -
  6*a*b*c**3*f**4 + 3*a**2*c**4*f**4 + c**2*d**2*f**4 + 4*b*c*d*e*f**4 - 6*a*c**2*d*e*f**4 + b**2*e**2*f**4 - 6*a*b*c*e**2*f**4 + 6*a**2*c**2*e**2*f**4 - 2*b*c**2*d*f**5 + 2*a*c**3*d*f**5 - 2*b**2*c*e*f**5 + 6*a*b*c**2*e*f**5 - 4*a**2*c**3*e*f**5 + b**2*c**2*f**6 - 2*a*b*c**3*f**6 +
  a**2*c**4*f**6)
    print(len(j421.dict()))
    return j421
def J521():
    x=PolynomialRing(ZZ,10,"abcdefghij").gens()
    a=x[0]
    b=x[1]
    c=x[2]
    d=x[3]
    e=x[4]
    f=x[5]
    g=x[6]
    h=x[7]
    i=x[8]
    j=x[9]
    J521=(((a*g + h)*(-h + c*i + e*j - c*f*j)*((b*d + c*e + f)*(-f + i*j - f*j**2)*((-(b*d + c*e + f))*g*(-g + a*h + b*i - a*c*i + d*j - a*e*j - b*f*j + a*c*f*j)*(-f + i*j - f*j**2) + d*(b*g + c*h + i)*(-i + f*j)*(-d + a*e + b*f - a*c*f + g*j - a*h*j - b*i*j + a*c*i*j - d*j**2 + a*e*j**2 + b*f*j**2 - a*c*f*j**2)) -
    (1 + d**2 + e**2 + f**2)*(1 + j**2)*((-(1 + b**2 + c**2))*g*(-g + a*h + b*i - a*c*i + d*j - a*e*j - b*f*j + a*c*f*j)*(1 + f**2 + i**2 - 2*f*i*j + f**2*j**2)
    +b*(b*g + c*h + i)*(-i + f*j)*(-b + a*c + d*f - a*e*f - b*f**2 + a*c*f**2 + g*i - a*h*i - b*i**2 + a*c*i**2 - f*g*j + a*f*h*j - d*i*j + a*e*i*j + 2*b*f*i*j - 2*a*c*f*i*j + d*f*j**2 - a*e*f*j**2 - b*f**2*j**2 + a*c*f**2*j**2)) -j*(d*g + e*h + f*i + j)*((-(1 + b**2 + c**2))*d*(-d + a*e + b*f - a*c*f + g*j - a*h*j - b*i*j + a*c*i*j - d*j**2 + a*e*j**2 + b*f*j**2 - a*c*f*j**2)*(1 + f**2 + i**2 - 2*f*i*j + f**2*j**2) +
      b*(b*d + c*e + f)*(-f + i*j - f*j**2)*(-b + a*c + d*f - a*e*f - b*f**2 + a*c*f**2 + g*i - a*h*i - b*i**2 + a*c*i**2 - f*g*j + a*f*h*j - d*i*j + a*e*i*j + 2*b*f*i*j - 2*a*c*f*i*j + d*f*j**2 - a*e*f*j**2 - b*f**2*j**2 + a*c*f**2*j**2))) -
  (b*g + c*h + i)*(-i + f*j)*((a*d + e)*(-e + c*f + h*j - c*i*j - e*j**2 + c*f*j**2)*((-(b*d + c*e + f))*g*(-g + a*h + b*i - a*c*i + d*j - a*e*j - b*f*j + a*c*f*j)*(-f + i*j - f*j**2) +
      d*(b*g + c*h + i)*(-i + f*j)*(-d + a*e + b*f - a*c*f + g*j - a*h*j - b*i*j + a*c*i*j - d*j**2 + a*e*j**2 + b*f*j**2 - a*c*f*j**2)) -
    (1 + d**2 + e**2 + f**2)*(1 + j**2)*((-(a*b + c))*g*(-g + a*h + b*i - a*c*i + d*j - a*e*j - b*f*j + a*c*f*j)*(-c + e*f - c*f**2 + h*i - c*i**2 - f*h*j - e*i*j + 2*c*f*i*j + e*f*j**2 - c*f**2*j**2) +
      a*(b*g + c*h + i)*(-i + f*j)*(-a + b*c - a*c**2 + d*e - a*e**2 - c*d*f - b*e*f + 2*a*c*e*f + b*c*f**2 - a*c**2*f**2 + g*h - a*h**2 - c*g*i - b*h*i + 2*a*c*h*i + b*c*i**2 - a*c**2*i**2 - e*g*j + c*f*g*j - d*h*j + 2*a*e*h*j + b*f*h*j - 2*a*c*f*h*j + c*d*i*j +
        b*e*i*j - 2*a*c*e*i*j - 2*b*c*f*i*j + 2*a*c**2*f*i*j + d*e*j**2 - a*e**2*j**2 - c*d*f*j**2 - b*e*f*j**2 + 2*a*c*e*f*j**2 + b*c*f**2*j**2 - a*c**2*f**2*j**2)) -
    j*(d*g + e*h + f*i + j)*((-(a*b + c))*d*(-d + a*e + b*f - a*c*f + g*j - a*h*j - b*i*j + a*c*i*j - d*j**2 + a*e*j**2 + b*f*j**2 - a*c*f*j**2)*(-c + e*f - c*f**2 + h*i - c*i**2 - f*h*j - e*i*j + 2*c*f*i*j + e*f*j**2 - c*f**2*j**2) +
      a*(b*d + c*e + f)*(-f + i*j - f*j**2)*(-a + b*c - a*c**2 + d*e - a*e**2 - c*d*f - b*e*f + 2*a*c*e*f + b*c*f**2 - a*c**2*f**2 + g*h - a*h**2 - c*g*i - b*h*i + 2*a*c*h*i + b*c*i**2 - a*c**2*i**2 - e*g*j + c*f*g*j - d*h*j + 2*a*e*h*j + b*f*h*j - 2*a*c*f*h*j +
        c*d*i*j + b*e*i*j - 2*a*c*e*i*j - 2*b*c*f*i*j + 2*a*c**2*f*i*j + d*e*j**2 - a*e**2*j**2 - c*d*f*j**2 - b*e*f*j**2 + 2*a*c*e*f*j**2 + b*c*f**2*j**2 - a*c**2*f**2*j**2))) -
  j*(d*g + e*h + f*i + j)*((a*d + e)*(-e + c*f + h*j - c*i*j - e*j**2 + c*f*j**2)*((-(1 + b**2 + c**2))*g*(-g + a*h + b*i - a*c*i + d*j - a*e*j - b*f*j + a*c*f*j)*(1 + f**2 + i**2 - 2*f*i*j + f**2*j**2) +
      b*(b*g + c*h + i)*(-i + f*j)*(-b + a*c + d*f - a*e*f - b*f**2 + a*c*f**2 + g*i - a*h*i - b*i**2 + a*c*i**2 - f*g*j + a*f*h*j - d*i*j + a*e*i*j + 2*b*f*i*j - 2*a*c*f*i*j + d*f*j**2 - a*e*f*j**2 - b*f**2*j**2 + a*c*f**2*j**2)) -
    (b*d + c*e + f)*(-f + i*j - f*j**2)*((-(a*b + c))*g*(-g + a*h + b*i - a*c*i + d*j - a*e*j - b*f*j + a*c*f*j)*(-c + e*f - c*f**2 + h*i - c*i**2 - f*h*j - e*i*j + 2*c*f*i*j + e*f*j**2 - c*f**2*j**2) +
      a*(b*g + c*h + i)*(-i + f*j)*(-a + b*c - a*c**2 + d*e - a*e**2 - c*d*f - b*e*f + 2*a*c*e*f + b*c*f**2 - a*c**2*f**2 + g*h - a*h**2 - c*g*i - b*h*i + 2*a*c*h*i + b*c*i**2 - a*c**2*i**2 - e*g*j + c*f*g*j - d*h*j + 2*a*e*h*j + b*f*h*j - 2*a*c*f*h*j + c*d*i*j +
        b*e*i*j - 2*a*c*e*i*j - 2*b*c*f*i*j + 2*a*c**2*f*i*j + d*e*j**2 - a*e**2*j**2 - c*d*f*j**2 - b*e*f*j**2 + 2*a*c*e*f*j**2 + b*c*f**2*j**2 - a*c**2*f**2*j**2)) -
    j*(d*g + e*h + f*i + j)*((-b)*(a*b + c)*(-c + e*f - c*f**2 + h*i - c*i**2 - f*h*j - e*i*j + 2*c*f*i*j + e*f*j**2 - c*f**2*j**2)*(-b + a*c + d*f - a*e*f - b*f**2 + a*c*f**2 + g*i - a*h*i - b*i**2 + a*c*i**2 - f*g*j + a*f*h*j - d*i*j + a*e*i*j + 2*b*f*i*j -
        2*a*c*f*i*j + d*f*j**2 - a*e*f*j**2 - b*f**2*j**2 + a*c*f**2*j**2) + a*(1 + b**2 + c**2)*(1 + f**2 + i**2 - 2*f*i*j + f**2*j**2)*(-a + b*c - a*c**2 + d*e - a*e**2 - c*d*f - b*e*f + 2*a*c*e*f + b*c*f**2 - a*c**2*f**2 + g*h - a*h**2 - c*g*i - b*h*i + 2*a*c*h*i +
        b*c*i**2 - a*c**2*i**2 - e*g*j + c*f*g*j - d*h*j + 2*a*e*h*j + b*f*h*j - 2*a*c*f*h*j + c*d*i*j + b*e*i*j - 2*a*c*e*i*j - 2*b*c*f*i*j + 2*a*c**2*f*i*j + d*e*j**2 - a*e**2*j**2 - c*d*f*j**2 - b*e*f*j**2 + 2*a*c*e*f*j**2 + b*c*f**2*j**2 - a*c**2*f**2*j**2))) -
  (1 + g**2 + h**2 + i**2 + j**2)*((a*d + e)*(-e + c*f + h*j - c*i*j - e*j**2 + c*f*j**2)*((-(1 + b**2 + c**2))*d*(-d + a*e + b*f - a*c*f + g*j - a*h*j - b*i*j + a*c*i*j - d*j**2 + a*e*j**2 + b*f*j**2 - a*c*f*j**2)*(1 + f**2 + i**2 - 2*f*i*j + f**2*j**2) +
      b*(b*d + c*e + f)*(-f + i*j - f*j**2)*(-b + a*c + d*f - a*e*f - b*f**2 + a*c*f**2 + g*i - a*h*i - b*i**2 + a*c*i**2 - f*g*j + a*f*h*j - d*i*j + a*e*i*j + 2*b*f*i*j - 2*a*c*f*i*j + d*f*j**2 - a*e*f*j**2 - b*f**2*j**2 + a*c*f**2*j**2)) -
    (b*d + c*e + f)*(-f + i*j - f*j**2)*((-(a*b + c))*d*(-d + a*e + b*f - a*c*f + g*j - a*h*j - b*i*j + a*c*i*j - d*j**2 + a*e*j**2 + b*f*j**2 - a*c*f*j**2)*(-c + e*f - c*f**2 + h*i - c*i**2 - f*h*j - e*i*j + 2*c*f*i*j + e*f*j**2 - c*f**2*j**2) +
      a*(b*d + c*e + f)*(-f + i*j - f*j**2)*(-a + b*c - a*c**2 + d*e - a*e**2 - c*d*f - b*e*f + 2*a*c*e*f + b*c*f**2 - a*c**2*f**2 + g*h - a*h**2 - c*g*i - b*h*i + 2*a*c*h*i + b*c*i**2 - a*c**2*i**2 - e*g*j + c*f*g*j - d*h*j + 2*a*e*h*j + b*f*h*j - 2*a*c*f*h*j +
        c*d*i*j + b*e*i*j - 2*a*c*e*i*j - 2*b*c*f*i*j + 2*a*c**2*f*i*j + d*e*j**2 - a*e**2*j**2 - c*d*f*j**2 - b*e*f*j**2 + 2*a*c*e*f*j**2 + b*c*f**2*j**2 - a*c**2*f**2*j**2)) +
(1 + d**2 + e**2 + f**2)*(1 + j**2)*((-b)*(a*b + c)*(-c + e*f - c*f**2 + h*i - c*i**2 - f*h*j - e*i*j + 2*c*f*i*j + e*f*j**2 - c*f**2*j**2)*(-b + a*c + d*f - a*e*f - b*f**2 + a*c*f**2 + g*i - a*h*i - b*i**2 + a*c*i**2 - f*g*j + a*f*h*j - d*i*j + a*e*i*j +
2*b*f*i*j - 2*a*c*f*i*j + d*f*j**2 - a*e*f*j**2 - b*f**2*j**2 + a*c*f**2*j**2) + a*(1 + b**2 + c**2)*(1 + f**2 + i**2 - 2*f*i*j + f**2*j**2)*(-a + b*c - a*c**2 + d*e - a*e**2 - c*d*f - b*e*f + 2*a*c*e*f + b*c*f**2 - a*c**2*f**2 + g*h - a*h**2 - c*g*i - b*h*i +
2*a*c*h*i + b*c*i**2 - a*c**2*i**2 - e*g*j + c*f*g*j - d*h*j + 2*a*e*h*j + b*f*h*j - 2*a*c*f*h*j + c*d*i*j + b*e*i*j - 2*a*c*e*i*j - 2*b*c*f*i*j + 2*a*c**2*f*i*j + d*e*j**2 - a*e**2*j**2 - c*d*f*j**2 - b*e*f*j**2 + 2*a*c*e*f*j**2 + b*c*f**2*j**2 -
a*c**2*f**2*j**2)))))
    print(len(J521.dict()))
    return J521;

if __name__=="__main__":
    print("1:j421\n2:J521\n?")
    i=int(input())
    j=[j421,J521]
    f=j[i-1]()
    b=0;
    if (b):
        sparsesos.is_sparsesos(f,newton_polytope=True)#com=sparsesos.com)
    else:
        #fout=open("j521.txt","w")
        #fout.write(str(f))
        #fout.close()
        dct=f.dict();
        l=list(dct)
        fout=open(["j421","j521"][i-1]+"_data.txt","w")
        fout.write("%d %d\n" % (len(l),len(l[0])))
        for i in f.dict():
            fout.write(" ".join(map(str,i)))
            fout.write(" %d\n" % dct[i])
        fout.close()
