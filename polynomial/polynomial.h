#ifndef SMT_MATH_POLYNOMIAL_H_
#define SMT_MATH_POLYNOMIAL_H_
#include <string>
#include <iostream>
#include <set>
#define SMT_MATH_POLYNOMIAL_monomial_shrink_bound  0.7
#define SMT_MATH_POLYNOMIAL_polynomial_shrink_bound 0.7
#define SMT_MATH_POLYNOMIAL_polynomial_var_bound 100

namespace polynomial {

  typedef std::size_t var;
  //const var null_var = UINT_MAX;
  //class polynomial;
  typedef std::pair<var,unsigned> var_pair;
  class monomial
  {
    private:
      unsigned __deg;
      std::size_t __dim;
      var_pair* __times;
      void monomial_init(var_pair* ts,std::size_t size,bool is_sorted,bool is_RD);
    public:
      monomial();
      //monomial(std::size_t n);
      monomial(var v);
      monomial(var v,unsigned time);
      monomial(var* vs,unsigned* ts,std::size_t size,bool is_sorted=false,bool is_RD=false);
      monomial(var_pair* ts,std::size_t size,bool is_move=false,bool is_sorted=false,bool is_RD=false); 
      monomial(const monomial &m);
      monomial(monomial &&m);
      ~monomial();
      void clear();
      inline bool empty() const{
        return(this->__dim==0);
      }
      monomial operator*(const monomial &m) const;
      monomial& operator=(const monomial &m);
      monomial& operator=(monomial &&m);
      //monomial& move(monomial &m);
      bool operator<(const monomial &m) const;
      bool operator==(const monomial &m) const;
      inline bool operator!=(const monomial &m) const{
        return !(*this==m);
      }  
      inline bool operator<=(const monomial &m) const{
        return !(m<*this);  
      }
      inline bool operator>(const monomial &m) const{
        return (m<*this);    
      }
      inline bool operator>=(const monomial &m) const{
        return !(*this<m);    
      }
      monomial power(unsigned) const;
      //unsigned set_time(var i,unsigned j);
      unsigned operator[](var i) const;
      unsigned re_deg();
      var_pair* find(var i) const;
      
      inline unsigned deg() const{
        return this->__deg;
      }
      inline var_pair* begin() const{
        return this->__times;
      }
      inline var_pair* end() const{
        return this->__times+this->__dim;
      }
      inline var_pair* at(std::size_t i) const{
        if (i>=this->size())
        {
          std::cout<<"monomial at "<<i<<" error. "<<i<<">=size.\n";
          i=0;
        }
        return this->__times+i;
      }
      inline std::size_t dim() const{
        return this->__dim;
      }
      inline std::size_t size() const{
        return this->__dim;
      }
      std::string str(std::string s="x",bool var_str=false) const;
      
  };
  
  template <typename Type> 
  using monomial_pair=std::pair<monomial,Type>;
  template <typename Type>
  class atomic_polynomial
  {
    private:
      const Type zero=0;
      unsigned __deg;
      std::size_t __size;
      std::pair<std::set<var>,bool> __vars;
      monomial_pair<Type>* __coeffs;
      void com_var();
      void atomic_polynomial_init
      (monomial_pair<Type>* cs,std::size_t size,bool is_sorted,bool is_RD);
    public:
      atomic_polynomial();
      //atomic_polynomial(std::size_t dim);
      atomic_polynomial(const Type& a);
      atomic_polynomial(Type&& a);
      atomic_polynomial(const monomial &m);
      atomic_polynomial(monomial &&m);
      atomic_polynomial(const monomial &m,const Type& coeff);
      atomic_polynomial(monomial &&m,Type&& coeff);
      atomic_polynomial(const atomic_polynomial<Type> &p);
      atomic_polynomial(atomic_polynomial<Type> &&p);
      atomic_polynomial(monomial* m,Type* c,std::size_t size,bool is_move=false,bool is_sorted=false,bool is_RD=false);
      atomic_polynomial(monomial_pair<Type>* cs1,std::size_t size,bool is_move=false,bool is_sorted=false,bool is_RD=false);
      void clear();
      ~atomic_polynomial();
    
      inline bool empty() const{
        return(this->__size==0);
      }
      inline unsigned deg() const
      {
        return this->__deg;
      }
      inline std::size_t size() const
      {
        return this->__size;
      }
      inline monomial_pair<Type>* begin() const
      {
        return this->__coeffs;
      }
      inline monomial_pair<Type>* end() const
      {
        return this->__coeffs+this->__size;
      }
      inline std::size_t dim()
      {
        if (!this->__vars.second)
          this->com_var();
        return this->__vars.first.size();
      }
      inline std::set<var>::iterator vars_begin()
      {
        if (!this->__vars.second)
          this->com_var();
        return this->__vars.first.begin();
      }
      inline std::set<var>::iterator vars_end()
      {
        if (!this->__vars.second)
          this->com_var();
        return this->__vars.first.end();
      }
      inline bool vars_find(const var &i) const
      {
        return this->__vars.first.find(i)!=this->__vars.first.end();
      }
      atomic_polynomial<Type>& operator=(const atomic_polynomial<Type> &p);
      atomic_polynomial<Type>& operator=(atomic_polynomial<Type> &&p);

      atomic_polynomial<Type>  operator*(const atomic_polynomial<Type> &p) const;
      //atomic_polynomial<Type>  operator*(const Type &i) const;
      atomic_polynomial<Type>  operator+(const atomic_polynomial<Type> &p) const;
      //atomic_polynomial<Type>  operator+(const Type &i) const;
      monomial_pair<Type>* find(const monomial& m) const;
      const Type& operator[](const monomial& m) const;
      inline monomial_pair<Type>* at(std::size_t i) const{
        if (i>=this->size())
        {
          std::cout<<"monomial at "<<i<<" error. "<<i<<">=size.\n";
          i=0;
        }
        return this->beign()+i;
      }
      //Type set_coeff(const monomial& m,Type c);
      bool operator==(const atomic_polynomial<Type> &p) const;
      bool operator<(const atomic_polynomial<Type> &p) const;
      inline bool operator!=(const atomic_polynomial<Type> &p) const{
        return !(p!=*this);
      }
      
      inline bool operator<=(const atomic_polynomial<Type> &p) const{
        return (!(p<(*this)));
      }
      inline bool operator>(const atomic_polynomial<Type> &p) const{
        return (p<(*this));
      }
      inline bool operator>=(const atomic_polynomial<Type> &p) const{
        return (!((*this)<p));
      }
      std::string str(std::string x="x", bool var_str=false) const; 
      //atomic_polynomial power(unsigned) const;

  };
}

#include "polynomial.tpp"
#endif
