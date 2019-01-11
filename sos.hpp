#ifndef SOS_HH
#define SOS_HH
#include <vector>
#include "polynomial/polynomial.h"
namespace is_sos{
    bool num_init(std::vector<int> &num,int &snum,const std::vector<int> &deg_maxv,const std::vector<int> &deg_minv,const int &degmax,const int &degmin);
    bool num_next(std::vector<int> &num,int &snum,const std::vector<int> &deg_maxv,const std::vector<int> &deg_minv,const int &degmax,const int &degmin);
    std::vector<polynomial::monomial>  sos_support_mosek(polynomial::atomic_polynomial<int> &p);
}
#endif