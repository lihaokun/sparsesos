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
    std::string polynomial_str(const polynomial::atomic_polynomial<polynomial::monomial,double> &p,const std::vector<std::string> &varname,int precision=8);
    std::string polynomial_str(const polynomial::atomic_polynomial<polynomial::monomial,long> &p,const std::vector<std::string> &varname);
    void Cholesky(double* M,std::size_t size); 
    std::vector<polynomial::atomic_polynomial<polynomial::monomial,double>> sosd(std::vector<polynomial::monomial> &points,std::vector<std::vector<polynomial::var>> &L,std::vector<std::vector<double>> &M);
}
#endif