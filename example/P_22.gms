Variables  x,y,a,b,c,d,e,f,objvar;
Equations  e1;
e1.. 16*a^6*d^4 + 48*a^4*b^2*d^4 + 36*a^2*b^4*d^4 + 8*b^6*d^4 + 24*a^4*c^2*d^4 + 36*a^2*b^2*c^2*d^4 + 12*b^4*c^2*d^4 + 9*a^2*c^4*d^4 + 6*b^2*c^4*d^4 + c^6*d^4 + 16*a^6*d^2*e^2 + 40*a^4*b^2*d^2*e^2 + 16*a^2*b^4*d^2*e^2 + 20*a^4*c^2*d^2*e^2 + 16*a^2*b^2*c^2*d^2*e^2 + 4*a^2*c^4*d^2*e^2 + 4*a^6*e^4 + 8*a^4*b^2*e^4 + 4*a^4*c^2*e^4 + 8*a^6*d^2*f^2 + 20*a^4*b^2*d^2*f^2 + 8*a^2*b^4*d^2*f^2 + 10*a^4*c^2*d^2*f^2 + 8*a^2*b^2*c^2*d^2*f^2 + 2*a^2*c^4*d^2*f^2 + 4*a^6*e^2*f^2 + 8*a^4*b^2*e^2*f^2 + 4*a^4*c^2*e^2*f^2 + a^6*f^4 + 2*a^4*b^2*f^4 + a^4*c^2*f^4 + 40*y*a^6*d^2 + 128*y*a^4*b^2*d^2 + 112*y*a^2*b^4*d^2 + 32*y*b^6*d^2 + 64*y*a^4*c^2*d^2 + 112*y*a^2*b^2*c^2*d^2 + 48*y*b^4*c^2*d^2 + 28*y*a^2*c^4*d^2 + 24*y*b^2*c^4*d^2 + 4*y*c^6*d^2 + 40*x*a^4*d^4 + 96*x*a^2*b^2*d^4 + 32*x*b^4*d^4 + 48*x*a^2*c^2*d^4 + 32*x*b^2*c^2*d^4 + 8*x*c^4*d^4 + 16*y*a^6*e^2 + 40*y*a^4*b^2*e^2 + 16*y*a^2*b^4*e^2 + 20*y*a^4*c^2*e^2 + 16*y*a^2*b^2*c^2*e^2 + 4*y*a^2*c^4*e^2 + 48*x*a^4*d^2*e^2 + 104*x*a^2*b^2*d^2*e^2 + 16*x*b^4*d^2*e^2 + 52*x*a^2*c^2*d^2*e^2 + 16*x*b^2*c^2*d^2*e^2 + 4*x*c^4*d^2*e^2 + 16*x*a^4*e^4 + 32*x*a^2*b^2*e^4 + 16*x*a^2*c^2*e^4 + 8*y*a^6*f^2 + 20*y*a^4*b^2*f^2 + 8*y*a^2*b^4*f^2 + 12*y*a^4*c^2*f^2 + 12*y*a^2*b^2*c^2*f^2 + 4*y*a^2*c^4*f^2 + 24*x*a^4*d^2*f^2 + 52*x*a^2*b^2*d^2*f^2 + 8*x*b^4*d^2*f^2 + 28*x*a^2*c^2*d^2*f^2 + 12*x*b^2*c^2*d^2*f^2 + 4*x*c^4*d^2*f^2 + 16*x*a^4*e^2*f^2 + 32*x*a^2*b^2*e^2*f^2 + 16*x*a^2*c^2*e^2*f^2 + 4*x*a^4*f^4 + 8*x*a^2*b^2*f^4 + 4*x*a^2*c^2*f^4 + 28*y^2*a^6 + 96*y^2*a^4*b^2 + 96*y^2*a^2*b^4 + 32*y^2*b^6 + 48*y^2*a^4*c^2 + 96*y^2*a^2*b^2*c^2 + 48*y^2*b^4*c^2 + 24*y^2*a^2*c^4 + 24*y^2*b^2*c^4 + 4*y^2*c^6 + 104*x*y*a^4*d^2 + 264*x*y*a^2*b^2*d^2 + 112*x*y*b^4*d^2 + 132*x*y*a^2*c^2*d^2 + 112*x*y*b^2*c^2*d^2 + 28*x*y*c^4*d^2 + 28*x^2*a^2*d^4 + 56*x^2*b^2*d^4 + 28*x^2*c^2*d^4 + 56*x*y*a^4*e^2 + 128*x*y*a^2*b^2*e^2 + 32*x*y*b^4*e^2 + 64*x*y*a^2*c^2*e^2 + 32*x*y*b^2*c^2*e^2 + 8*x*y*c^4*e^2 + 40*x^2*a^2*d^2*e^2 + 80*x^2*b^2*d^2*e^2 + 40*x^2*c^2*d^2*e^2 + 16*x^2*a^2*e^4 + 32*x^2*b^2*e^4 + 16*x^2*c^2*e^4 + 28*x*y*a^4*f^2 + 64*x*y*a^2*b^2*f^2 + 16*x*y*b^4*f^2 + 36*x*y*a^2*c^2*f^2 + 24*x*y*b^2*c^2*f^2 + 8*x*y*c^4*f^2 + 20*x^2*a^2*d^2*f^2 + 40*x^2*b^2*d^2*f^2 + 20*x^2*c^2*d^2*f^2 + 16*x^2*a^2*e^2*f^2 + 32*x^2*b^2*e^2*f^2 + 16*x^2*c^2*e^2*f^2 + 4*x^2*a^2*f^4 + 8*x^2*b^2*f^4 + 4*x^2*c^2*f^4 + 72*x*y^2*a^4 + 192*x*y^2*a^2*b^2 + 96*x*y^2*b^4 + 96*x*y^2*a^2*c^2 + 96*x*y^2*b^2*c^2 + 24*x*y^2*c^4 + 72*x^2*y*a^2*d^2 + 144*x^2*y*b^2*d^2 + 72*x^2*y*c^2*d^2 + 48*x^2*y*a^2*e^2 + 96*x^2*y*b^2*e^2 + 48*x^2*y*c^2*e^2 + 24*x^2*y*a^2*f^2 + 48*x^2*y*b^2*f^2 + 24*x^2*y*c^2*f^2 + 48*x^2*y^2*a^2 + 96*x^2*y^2*b^2 + 48*x^2*y^2*c^2
-objvar =E= 0;
