#ifndef SPARSESOS_torational
#define SPARSESOS_torational
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/rational.hpp>
#include <cmath>
//#include <boost/numeric/ublas/operation_blocked.hpp>

namespace sparsesos{
    
    using boost::numeric::ublas::matrix;
    using boost::numeric::ublas::identity_matrix;
    using boost::numeric::ublas::vector;
    using boost::numeric::ublas::matrix_vector_range;
    using boost::numeric::ublas::trans;
    using boost::numeric::ublas::permutation_matrix;
    using boost::numeric::ublas::lu_factorize;
    using boost::numeric::ublas::lu_substitute;
    using boost::numeric::ublas::axpy_prod;
    using boost::numeric::ublas::opb_prod;
    using boost::numeric::ublas::prod;
    using boost::numeric::ublas::norm_frobenius;
    
     /* Matrix inversion routine.
    Uses lu_factorize and lu_substitute in uBLAS to invert a matrix */
    template<class T>
    bool InvertMatrix (const matrix<T>& input, matrix<T>& inverse) {
        typedef permutation_matrix<std::size_t> pmatrix;
        // create a working copy of the input
        matrix<T> A(input);
        // create a permutation matrix for the LU-factorization
        pmatrix pm(A.size1());

        // perform LU-factorization
        int res = lu_factorize(A,pm);
            if( res != 0 ) return false;

        // create identity matrix of "inverse"
        //inverse.assign(identity_matrix<T>(A.size1()));
        inverse=identity_matrix<T>(A.size1());
        // backsubstitute to get the inverse
        lu_substitute(A, pm, inverse);

        return true;
    }

    
    template<class T>
    void LDLT(matrix<T> & A)
    {
        if (A.size1()!=A.size2()){
            std::cout<<"LDLT: A is not symmetric\n";
            return void();
        }
        int n=A.size1();
        std::vector<T> v(n);
        int i,j,k;
        for(j=0;j<n;++j)
        {
            for(i=0;i<j;++i){
                v[i]=A(j,i)*A(i,i);
                A(j,j)-=A(j,i)*v[i];
            }
            for(i=j+1;i<n;++i){
                for(k=0;k<j;++k)
                    A(i,j)-=A(i,k)*v[k];
                A(i,j)/=A(j,j);    
            }
        }
    }
    //template <class Tf,class Tq>
    template <class Tf,class Tz>
    inline void float_to_rational(
        const Tf &Mf,boost::rational<Tz> &Mq, const Tz &den )
    {
        /*
        if (d==0)
        {
            std::cout<<"rationafloat_to_rational denominator 0 error.\n";
            exit(0);
        }
        */
       Mq=boost::rational<Tz>(Tz(den*Mf),den);
    }
    
    template <class Tf,class Tz>
    inline void float_to_rational(
        const matrix<Tf> &Mf,matrix<boost::rational<Tz>> &Mq, const Tf &dist)
    {
        Tz den=Tz(Tf(Mf.size1())/dist)+1;
        //std::cout<<dist<<" "<<double(Mq.size1())<<std::endl;
        Mq=matrix<boost::rational<Tz>>(Mf.size1(),Mf.size2());
        for(int i=0;i<Mq.size1();++i)
            for(int j=0;j<Mq.size2();++j)
                float_to_rational(Mf(i,j),Mq(i,j),den);
    }
    template <class Tz,class Tf>
    inline void rational_to_float(const boost::rational<Tz> &q,Tf &f)
    {
        f=Tf(q.numerator())/Tf(q.denominator());   
    }
    template <class Tq,class Tf>
    inline matrix<Tf> rational_to_float(const matrix<Tq> &Mq)
    {
        matrix<Tf> Mf(Mq.size1(),Mq.size2());
        for(int i=0;i<Mq.size1();++i)
            for(int j=0;j<Mq.size2();++j)
                rational_to_float(Mq(i,j),Mf(i,j));//=Tf(.numeric())/Tf(Mq(i,j).denominator());
        return Mf;
    }
    
    template <class Tq,class Tf>
    matrix<Tq> SOS_torational(const matrix<Tq> & A,const matrix<Tq> & b,matrix<Tf> & M)
    {
        int n=M.size1();
        identity_matrix<Tq> I(A.size2(),A.size2());
        matrix<Tq> A_T=trans(A);
        matrix<Tq> A_;
        if (!InvertMatrix(matrix<Tq>(prod(A,A_T)),A_))
        {
            std::cout<<"SOS_torational error: A*AT can't invert.";
            exit(0);
        }
        A_=prod(A_T,A_);
        
        matrix<Tq> b_=prod(A_,b);
        //std::cout<<rational_to_float<Tq,Tf>(b_)<<std::endl;
        matrix<Tq> A_I=I-prod(A_,A);
        
        M.resize(n*n,1,false);
        
        matrix<Tf> M_=rational_to_float<Tq,Tf>(b_)+prod(rational_to_float<Tq,Tf>(A_I),M);
        M.resize(n,n,false);M_.resize(n,n,false);
        
        matrix<Tf> M_L(M);
        //float L[n*n],D[n];
        LDLT(M_L);
        //(M-M_).print();
        Tf l1=norm_frobenius(M-M_);
        Tf l2=M_L(0,0);
        for(int i=1;i<n;i++)
            if (M_L(i,i)<l2) l2=M_L(i,i);
        std::cout<<l1<<" "<<l2<<" "<<(l2>l1)<<std::endl;
        if(l2>l1)
            {
                //matrix<Tq> Q=M.torational(int(float(n)/sqrt(l2*l2-l1*l1))+1);
                matrix<Tq> Q;
                float_to_rational(M,Q,sqrt(l2*l2-l1*l1));
                std::cout<<norm_frobenius(M-rational_to_float<Tq,Tf>(Q))<<std::endl;
                
                Q.resize(n*n,1,false);
                matrix<Tq> Q_=b_+prod(A_I,Q);
                Q_.resize(n,n,false);
                std::cout<<Q_<<std::endl;
                //Q_.print();
                return Q_;
            }
        else
            {
                
                std::cout<<"SOS_torational l2="<<l2<<",l1="<<l1<<" error.\n";
                exit(0);
        }
        
    }
    
    

    
}
#endif

