#include <vector>
#include "sparsesos.h"
#include <string>
#include <iostream>
#include <list>
#include <string.h>
#include <stdlib.h>
//#include "polynomial.h"
int main(int argc, char const *argv[])
{
        
    std::string s;
    if (argc>1)
        s=argv[1];
    else{
        std::cout<<"file_name=";
        std::cin>>s;
    }
    int B=1;
    if (argc>2){
        if (strcmp(argv[2],"-t")==0)
            B=2;
        if (strcmp(argv[2],"--sos")==0)
            B=0;
        if (strcmp(argv[2],"--c")==0)
            B=1;
    }
    //s="j421_data.txt";
    //s="j621_data.txt";
    //s="j521_data.txt";
    //s="test_data.txt";
    //std::cin>>s;
    //sparsesos::monomial_map dct;
    
    polynomial::atomic_polynomial<int> p;
    sparsesos::read_data(s,p);
    std::vector<polynomial::monomial> points;
    std::cout<< sparsesos::get_half(p,points,true)<<" "<< points.size()<<std::endl;
    std::vector<std::vector<polynomial::var>> L;
    if (B>=1){
        sparsesos::com_connect(p,points,L);
        for(auto &i:L)
            std::cout<<i.size()<<" ";
        std::cout<<std::endl;
        s="data.dat-s";
        if(B==1)
            sparsesos::output(s,p,points,L);
    }
    if (B==2){
        std::cout<<std::endl;
        std::vector<std::vector<polynomial::var>> L1;
        
        sparsesos::MCS_M(p,points,L, L1);
        std::sort(L1.begin(),L1.end(),[](const std::vector<polynomial::var> &v1,
                                        const std::vector<polynomial::var> &v2){return v1.size()>v2.size();});
        s="data.dat-s";
        sparsesos::output(s,p,points,L1);    

    }
    s="./csdp/bin/csdp "+s;
    std::cout<<"----SDP solver------\n";
    system(s.c_str());
    return 0;

}
