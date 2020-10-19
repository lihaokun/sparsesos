#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <string.h>
#include <stdlib.h>
#include <ctime>
#include "sos.hpp"
#include "sos_mosek.hpp"
//#include<thread>
int main(int argc, char const *argv[])
{
    //int numThreads=std::thread::hardware_concurrency();
    //std::cout << "numThreads：" << numThreads << std::endl;
    std::string filename;
    int i1,i2;
    int argc_int=1;
    int B=1;
    //int min_bool=false;
    //int point_com=0;
    bool polyprint_bool=false;
    bool file_name_bool=false;
    std::string point_com_s;
    while (argc_int<argc){
        // if (strcmp(argv[argc_int],"-t")==0)
        //     B=2;
        //else 
        if (strcmp(argv[argc_int],"-s")==0)
            B=0;
        else if (strcmp(argv[argc_int],"-c")==0)
            B=1;
        // else if (strcmp(argv[argc_int],"-m")==0)
        //     min_bool=true;
        else if (strcmp(argv[argc_int],"-p")==0)
            polyprint_bool=true;
        // else if (strcmp(argv[argc_int],"-d")==0)
        // {
        //     if  (argc_int+1<argc){
        //         point_com=1;
        //         point_com_s=argv[++argc_int];
        //     }
        //     else{
        //         std::cout<<"Input parameter Error： -d.\n";
        //         return 0;
        //     }
        // }
        else if  (argv[argc_int][0]!='-' && !file_name_bool)
        {
            file_name_bool=true;
            filename=argv[argc_int];
        }
        else{
            std::cout<<argv[argc_int]<<"?\n";
            return 0;
        }
        ++argc_int;
    }
    if (!file_name_bool)
    {
        std::cout<<"Input parameter Error.\n";
        return 0;
    }
    clock_t t;
    t=clock();
    std::cout<<"Read...\n";
    std::unordered_map<std::string,int> varmap;
    std::vector<std::string> varname;
    polynomial::atomic_polynomial<polynomial::monomial,long> p;
    try{
        p=sparsesos::read_polynomial(filename,varmap,varname);
    }catch(...){
        std::cout<<"Input error.\n";
        return 0;
    }
    if  (polyprint_bool)
        std::cout<<sparsesos::polynomial_str(p,varname)<<std::endl;
    printf("Read done.(%.2fs)\n" ,(clock()-(float)t)/CLOCKS_PER_SEC);
    if (p.empty())
    {
        std::cout<<"0=0^2\n";
        std::fstream fout(filename+".sosd",std::fstream::out);
        fout<<"0"<<std::endl;
        fout.close();
        std::cout<<"Solve successful.\n";
        return 0;
    }
    std::size_t polydim=varname.size();
    std::vector<polynomial::monomial> points;
    
    //time(&t);
    t=clock();
    //if (point_com==0){
        //sparsesos::get_half(p,points,true);
    points=sparsesos::sos_support_mosek(p,polydim);
    if (!sparsesos::sos_support_check(p,points))
    {
        std::cout<<"Error:The vertex of support of polynomial is not even.\n";
        return 0;
    }
    // }
    // else{
    //     sparsesos::read_point_data(point_com_s,points);
    // }
    //printf("(%.2fs)\n" ,(clock()-(float)t)/CLOCKS_PER_SEC);
    //std::cout<<  points.size()<<std::endl;
    //if (point_com==0 && polyprint_bool){
    if (polyprint_bool){
    
        for(auto &i:points)
        {
            std::cout<<sparsesos::monomial_str(i,varname)<<",";
        }
        std::cout<<std::endl;
    }
    std::vector<std::vector<polynomial::var>> L;
    //std::vector<std::vector<polynomial::var>> L1;
        
    //time_t t;
    
    //s="data.dat-s";
    //sout="data.res";
        
    if (B==0){
        L.push_back(std::vector<polynomial::var>(points.size()));
        for(polynomial::var i=0;i<points.size();++i)
            L[0][i]=i;
        //sparsesos::output(s,p,points,L,min_bool);
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
        //if(B==1)
        //    sparsesos::output(s,p,points,L,false);
            
    }
    // if (B==2){
    //     //std::cout<<std::endl;
        
    //     sparsesos::MCS_M(p,points,L, L1);
    //     std::sort(L1.begin(),L1.end(),[](const std::vector<polynomial::var> &v1,
    //                                     const std::vector<polynomial::var> &v2){return v1.size()>v2.size();});
    //     i1=L1.begin()->size();i2=0;
    //     for(auto &i:L1)
    //         if (i.size()==i1)
    //             ++i2;
    //         else{
    //             std::cout<<i2<<"*"<<i1<<" ";
    //             i1=i.size();
    //             i2=1;
    //         }
    //     std::cout<<i2<<"*"<<i1<<std::endl;
    //     if (polyprint_bool){
    //         for (auto &i:L1){
    //             for (auto &j:i)
    //                std::cout<<sparsesos::monomial_str(points[j],varname)<<",";
    //             std::cout<<std::endl;
    //         }
    //     }
    //     L=move(L1);
    //     //s="data.dat-s";
    //     //sparsesos::output(s,p,points,L1,min_bool);    
    //     //sparsesos::SOS_solver_mosek(p,points,L1);

    // }
    // s="csdp "+s+" "+sout;
    //printf("(time:%.2fs)\n" ,difftime(time(NULL),t));
    printf("Initialization done.(%.2fs)\n" ,(clock()-(float)t)/CLOCKS_PER_SEC);
    std::cout<<"-------SDP solver begin-------\n";
    // system(s.c_str());
    std::vector<std::vector<double>> ans;
    bool sol=sparsesos::SOS_solver_mosek(p,points,L,ans);
    std::cout<<"-------SDP solver end-------\n";
    if (sol)
        std::cout<<"Solve successful.\n";
    else   
        std::cout<<"Solve failed.\n";
    std::vector<polynomial::atomic_polynomial<polynomial::monomial,double>>  sosd;
    if  (sol)
    {
        sosd=sparsesos::sosd(points,L,ans);
        std::fstream fout(filename+".sosd",std::fstream::out);
        for (auto &tmp_p:sosd)
            fout<<sparsesos::polynomial_str(tmp_p,varname,10)<<std::endl;
        fout.close();
    }
    return 0;

}
