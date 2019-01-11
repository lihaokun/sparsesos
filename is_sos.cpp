#include <vector>
#include "sparsesos.h"
#include <string>
#include <iostream>
#include <list>
#include <string.h>
#include <stdlib.h>
#include <ctime>
//#include "SOS_torational.hpp"
#include "sos.hpp"
//#include "polynomial.h"
int main(int argc, char const *argv[])
{
        
    std::string s,sout;
    int i1,i2;
    int argc_int=1;
    if (argc>1 && argv[0]!="-"){
        s=argv[1];
        ++argc_int;
    }
    else{
        std::cout<<"file_name=";
        std::cin>>s;
    }
    int B=1;
    int min_bool=false;
    int point_com=0;
    std::string point_com_s;
    while (argc_int<argc){
        if (strcmp(argv[argc_int],"-t")==0)
            B=2;
        else
        if (strcmp(argv[argc_int],"-s")==0)
            B=0;
        else
        if (strcmp(argv[argc_int],"-c")==0)
            B=1;
        else
        if (strcmp(argv[argc_int],"-m")==0)
            min_bool=true;
        else
        if (strcmp(argv[argc_int],"-d")==0 && argc_int+1<argc)
        {
            point_com=1;
            point_com_s=argv[++argc_int];
        }
        else{
            std::cout<<argv[argc_int]<<" is no def.\n";
            return 0;
        }
        ++argc_int;
    }
    
    //s="j421_data.txt";
    //s="j621_data.txt";
    //s="j521_data.txt";
    //s="test_data.txt";
    //std::cin>>s;
    //sparsesos::monomial_map dct;
    polynomial::atomic_polynomial<int> p;
    sparsesos::read_data(s,p);
    //std::cout<<p.str()<<std::endl;
    std::cout<<"deg:"<<p.deg()<<std::endl;
    std::vector<polynomial::monomial> points;
    clock_t t;
    //time(&t);
    t=clock();
    if (point_com==0){
        //sparsesos::get_half(p,points,true);
        points=is_sos::sos_support_mosek(p);
    }
    else{
        sparsesos::read_point_data(point_com_s,points);
    }
    std::cout<<  points.size()<<std::endl;
    std::vector<std::vector<polynomial::var>> L;
    //time_t t;
    
    s="data.dat-s";
    sout="data.res";
        
    if (B==0){
        L.push_back(std::vector<polynomial::var>(points.size()));
        for(polynomial::var i=0;i<points.size();++i)
            L[0][i]=i;
        sparsesos::output(s,p,points,L,min_bool);
    }
    if (B>=1){
        sparsesos::com_connect(p,points,L);
        i1=L.begin()->size();i2=0;
        for(auto &i:L){
            if (i.size()==i1)
                ++i2;
            else{
                std::cout<<i2<<"*"<<i1<<" ";
                i1=i.size();
                i2=1;
            }
        }
        std::cout<<i2<<"*"<<i1<<std::endl; 
        /*
        for(auto &i:L)
            for(auto &j:i)
                std::cout<<points[j].str()<<",";
        std::cout<<std::endl;
        */
        //s="data.dat-s";
        if(B==1)
            sparsesos::output(s,p,points,L,min_bool);
    }
    if (B==2){
        //std::cout<<std::endl;
        std::vector<std::vector<polynomial::var>> L1;
        
        sparsesos::MCS_M(p,points,L, L1);
        std::sort(L1.begin(),L1.end(),[](const std::vector<polynomial::var> &v1,
                                        const std::vector<polynomial::var> &v2){return v1.size()>v2.size();});
        i1=L1.begin()->size();i2=0;
        for(auto &i:L1)
            if (i.size()==i1)
                ++i2;
            else{
                std::cout<<i2<<"*"<<i1<<" ";
                i1=i.size();
                i2=1;
            }
        std::cout<<i2<<"*"<<i1<<std::endl;
        
        //s="data.dat-s";
        sparsesos::output(s,p,points,L1,min_bool);    

    }
    s="./csdp/bin/csdp "+s+" "+sout;
    //printf("(time:%.2fs)\n" ,difftime(time(NULL),t));
    printf("(time:%.2fs)\n" ,(clock()-(float)t)/CLOCKS_PER_SEC);
    std::cout<<"----SDP solver------\n";
    system(s.c_str());
    
    return 0;

}
