#include <boost/python.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <list>
#include <ctime>
#include <string.h>
#include <stdlib.h>
#include "sos.hpp"
#include "sos_mosek.hpp"
bool is_sos(std::string strpoly)
{
    clock_t t;
    int i1,i2;
    
    t=clock();
    std::cout<<"Read...\n";
    std::unordered_map<std::string,int> varmap;
    std::vector<std::string> varname;
    polynomial::atomic_polynomial<polynomial::monomial,long> p;
    try{
        std::istringstream isstrpoly(strpoly);
        p=sparsesos::read_polynomial(isstrpoly,varmap,varname);
    }catch(...){
        std::cout<<"Input error.\n";
        return false;
    }
    //if  (polyprint_bool)
     //   std::cout<<sparsesos::polynomial_str(p,varname)<<std::endl;
    printf("Read done.(%.2fs)\n" ,(clock()-(float)t)/CLOCKS_PER_SEC);
    std::size_t polydim=varname.size();
    std::vector<polynomial::monomial> points;
    points=sparsesos::sos_support_mosek(p,polydim);
    if (!sparsesos::sos_support_check(p,points))
    {
        std::cout<<"Error:The vertex of support of polynomial is not even.\n";
        return false;
    }
    std::vector<std::vector<polynomial::var>> L;
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
    printf("Initialization done.(%.2fs)\n" ,(clock()-(float)t)/CLOCKS_PER_SEC);
    std::cout<<"-------SDP solver begin-------\n";
    // system(s.c_str());
    std::vector<std::vector<double>> ans;
    bool sol=sparsesos::SOS_solver_mosek(p,points,L,ans);
    std::cout<<"-------SDP solver end-------\n";
    if (sol){
        std::cout<<"Solve successful.\n";
        return true;
    }
    else   
    {
        std::cout<<"Solve failed.\n";
        return false;
    }
    // std::vector<polynomial::atomic_polynomial<polynomial::monomial,double>>  sosd;
    // if  (sol)
    // {
    //     sosd=sparsesos::sosd(points,L,ans);
    //     std::fstream fout(filename+".sosd",std::fstream::out);
    //     for (auto &tmp_p:sosd)
    //         fout<<sparsesos::polynomial_str(tmp_p,varname,10)<<std::endl;
    // }
    // return 0;
}
BOOST_PYTHON_MODULE(pysos)
{
    using namespace boost::python;
    def("is_sos", is_sos);
}


