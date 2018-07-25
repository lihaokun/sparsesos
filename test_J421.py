# -*- coding: utf-8 -*-
import sys
from sage.all import QQ,PolynomialRing
import sparsesos

def j421():
    x=PolynomialRing(QQ,6,"abcdef").gens()
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

if __name__=="__main__":
    f=j421()
    sparsesos.is_sparsesos(f,newton_polytope=True,com=sparsesos.com)
