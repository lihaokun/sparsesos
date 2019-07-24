#ifndef SOS_HH
#define SOS_HH
#include <vector>
#include "polynomial/polynomial.hpp"
#include <unordered_map>
#include <string>

namespace is_sos{
    bool num_init(std::vector<int> &num,int &snum,const std::vector<int> &deg_maxv,const std::vector<int> &deg_minv,const int &degmax,const int &degmin);
    bool num_next(std::vector<int> &num,int &snum,const std::vector<int> &deg_maxv,const std::vector<int> &deg_minv,const int &degmax,const int &degmin);
    polynomial::atomic_polynomial<polynomial::monomial,long> read_polynomial(std::string &filename,std::unordered_map<std::string,int> &varmap,std::vector<std::string> &varname);
    polynomial::atomic_polynomial<polynomial::monomial,long> read_polynomial(std::fstream &fin,std::unordered_map<std::string,int> &varmap,std::vector<std::string> &varname);
    bool sos_support_check(polynomial::atomic_polynomial<polynomial::monomial,long> check,std::vector<polynomial::monomial> &point);
    std::string  monomial_str(const polynomial::monomial &m,const std::vector<std::string> &varname);
    template <typename Tc>
    std::string polynomial_str(const polynomial::atomic_polynomial<polynomial::monomial,Tc> &p,const std::vector<std::string> &varname)
    {
        if (p.size()==0)
        return "0";
        std::stringstream ss;
        for(auto i=p.begin();i!=p.end();++i)
        {
        if (i!=p.begin() && i->second>=0)
            ss<<"+";
        if (!monomial_empty(i->first)){
            if (i->second==-1)
            ss<<"-";
            else
            if (i->second!=1 )
            ss<< i->second<<"*";
            ss<<monomial_str(i->first,varname);
        }
        else
            ss<<i->second;
        }
        return ss.str();
    }
    void Cholesky(double* M,std::size_t size); 
    std::vector<polynomial::atomic_polynomial<polynomial::monomial,double>> sosd(std::vector<polynomial::monomial> &points,std::vector<std::vector<polynomial::var>> &L,std::vector<std::vector<double>> &M);
}
#endif