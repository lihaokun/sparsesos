#ifndef SPARSESOS_H
#define SPARSESOS_H
#include <vector>
//#include <map>
#include "polynomial/polynomial.h"
#include <unordered_map>
#include <map>
#include <boost/functional/hash.hpp>
namespace sparsesos{

  template <typename Container> // we can make this generic for any container [1]
  struct container_hash {
    std::size_t operator()(Container const& c) const {
        return boost::hash_range(c.begin(), c.end());
    } 
  };
  
  typedef std::map<polynomial::monomial,int> monomial_map;

  int read_data(std::string &str_file,polynomial::atomic_polynomial<int> &p);
  bool get_half(polynomial::atomic_polynomial<int> &p,std::vector<polynomial::monomial> &point,bool is_check);
  void com_connect
    (polynomial::atomic_polynomial<int> &p,std::vector<polynomial::monomial> &points,std::vector<std::vector<polynomial::var>> &L);
  void MCS_M
    (polynomial::atomic_polynomial<int> &p,std::vector<polynomial::monomial> &points,std::vector<std::vector<polynomial::var>> &V,std::vector<std::vector<polynomial::var>> & L);
  void output
    (std::string& str_file,polynomial::atomic_polynomial<int> &p,std::vector<polynomial::monomial> &points,std::vector<std::vector<polynomial::var>> &L);
          
}


#endif
