#ifndef SPARSESOS_torational
#define SPARSESOS_torational
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace sparsesos{
    template <class T>
    using matrix=boost::numeric::ublas::matrix<T>;
    template <class T>
    using vector=boost::numeric::ublas::vector<T>;
    
    template <class T>
    using symmetric_matrix=boost::numeric::ublas::symmetric_matrix<T>;
    
    template<class T>
    vector<T> LDLT(const matrix<T> & M,matrix<T> & L,matrix<T> & D);
}
#endif

