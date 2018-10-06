#include "SOS_torational.hpp"


namespace sparsesos{

    template<class T>
    vector<T> LDLT(const matrix<T> & M,matrix<T> & L)
    {
        if (M.size1()!=M.size2()){
            std::cout<<"LDLT: M is not symmetric\n";
            return void()
        }
        int n=M.size1();
        L=matrix(n,n);
        vector<T>  D(n);
        int i,j,k;
        for(j=0;j<n;j++)
        {
            D(j)=M(j,j);
            for(k=0;k<j;k++)
                D(j)-=L(j,k)*D(k)*L(j,k);
            L(j,j)=1;
            for(i=j+1;i<n;i++)
            {
                L(i,j)=M(i,j);
                for(k=0;k<j;k++)
                    L(i,j)-=L(j,k)*D(k)*L(i,k);
                L(i,j)=L(i,j)/D(j);
            }
        }
        return D;
    }
}