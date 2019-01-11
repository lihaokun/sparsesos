from __future__ import print_function
import os
import sys
from sage.all import QQ,PolynomialRing
import sparsesos

def main(S):
    if os.path.isfile(S):
        print(os.path.isfile(S))
        fin=open(S,"r")
        SP=fin.read()
        fin.close()
    else:
        print("Can't find file ",S,".")
        sys.exit(0)
    L=S.split(".")
    if len(L)>1:
        L=L[:-1]
    file_name=".".join(L)
    f,R=sparsesos.strtoPolynomial(SP)
    # s=raw_input("Is print polynomial?(y)")
    # if s in ['y','Y','']:
    #     print(f)
    print("Write poly to file...",end="")
    dct=f.dict();
    l=list(dct)
    fout=open(file_name+"_data.txt","w")
    fout.write("%d %d\n" % (len(l),len(l[0])))
    for i in f.dict():
        fout.write(" ".join(map(str,i)))
        fout.write(" %d\n" % dct[i])
    fout.close()
    print("done.")
    # s=raw_input("Is compute polytope?(y)")
    # if s in ['y','Y','']:
    #     print("Compute polytope...",end=" ")
    #     points=((QQ(1)/2)*f.newton_polytope()).integral_points()
    #     print("done.")
    #     print("Write poly to file...",end=" ")
    #     fout=open(file_name+"_data_polytope.txt","w")
    #     fout.write("%d %d\n" % (len(points),len(points[0])))
    #     for i in points:
    #         fout.write(" ".join(map(str,i)))
    #         fout.write("\n")
    #     fout.close()
    #     print("done.")
    #     print("./is_sos %s_data.txt -d %s_data_polytope.txt" % (file_name,file_name))
    #     os.system("./is_sos %s_data.txt -d %s_data_polytope.txt" % (file_name,file_name))
    
    # else:
    print("./is_sos %s_data.txt" % file_name)
    os.system("./is_sos %s_data.txt" % file_name)

if __name__=="__main__":
    if len(sys.argv)<2:
        file=raw_input("file_name=")
        main(file)
    else:
        file=sys.argv[1];
        main(file)
    
    
    
