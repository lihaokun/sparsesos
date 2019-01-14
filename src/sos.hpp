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
    std::vector<polynomial::monomial>  sos_support_mosek(polynomial::atomic_polynomial<polynomial::monomial,long>&p,std::size_t polydim);
    bool sos_support_check(polynomial::atomic_polynomial<polynomial::monomial,long> check,std::vector<polynomial::monomial> &point);
    std::string polynomial_str(const polynomial::atomic_polynomial<polynomial::monomial,long> &p,const std::unordered_map<std::string,int> &varmap,const std::vector<std::string> &varname);
}
#endif