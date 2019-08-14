#ifndef SOS_MOSEK_HH
#define SOS_MOSEK_HH
#include <vector>
#include "polynomial/polynomial.hpp"
#include <unordered_map>
#include <string>

namespace sparsesos{
    std::vector<polynomial::monomial>  sos_support_mosek(polynomial::atomic_polynomial<polynomial::monomial,long>&p,std::size_t polydim,int numthreads=0);
    bool SOS_solver_mosek(polynomial::atomic_polynomial<polynomial::monomial,long>&p,std::vector<polynomial::monomial> &points,std::vector<std::vector<polynomial::var>> &L,std::vector<std::vector<double>> &ans,int numthreads=0);
}
#endif