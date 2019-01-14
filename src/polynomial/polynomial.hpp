#ifndef SMT_MATH_POLYNOMIAL_H_
#define SMT_MATH_POLYNOMIAL_H_
#include <string>
#include <iostream>
#include <sstream>
#include <set>
#include <cstdint>
#include <algorithm>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>
#include <time.h>
// #include <gmpxx.h>
#define SMT_MATH_POLYNOMIAL_monomial_shrink_bound  0.7
#define SMT_MATH_POLYNOMIAL_polynomial_shrink_bound 0.7
#define SMT_MATH_POLYNOMIAL_polynomial_var_bound 100
#define SMT_MATH_POLYNOMIAL_polynomial_to_uni_bound 1
namespace polynomial {

  typedef std::size_t var;
  //const var null_var = UINT_MAX;
  //class polynomial;
  //typedef std::set<var> polynomialring;
  struct uint64_monomial_data{
    std::size_t __len;
    std::uint64_t __range;
    std::uint64_t __div;
    std::size_t __size;
  };
  inline void uint64_monomial_data_init(uint64_monomial_data &m,std::size_t dim)
  {
    if (dim>1){
      ++dim;
      //std::cout<<dim;
      m.__size=dim;
      m.__len=64/dim;
      m.__range=(1L<<(m.__len))-1;
      m.__div=0;
      for(;dim>0;--dim){
        m.__div<<=(m.__len);
        m.__div+=1L<<(m.__len-1);
      }
    }
    else{
      m.__size=1;
      m.__len=64;
      m.__range=0;
      --m.__range;
      //std::cout<<m.__range<<std::endl;
      m.__div=1;
      m.__div<<=63;
    }
  }
  class polynomialring{
    private:
      var* __vars;
      var* __vars_end;
      std::size_t __size;
      uint64_monomial_data __data64;
    public:
      polynomialring();
      polynomialring(var* vs1,std::size_t size,bool is_move=false,bool is_sorted=false,bool is_RD=false);
      polynomialring(const polynomialring & p);
      polynomialring(polynomialring&& p);
      inline void clear()
      {
        if (this->__vars){
          delete [] this->__vars;
          this->__vars=nullptr;
          this->__vars_end=nullptr;
        }
        this->__size=0;
        uint64_monomial_data_init(this->__data64,0);
      }
      ~polynomialring()
      {
        if (this->__vars)
          delete [] this->__vars;
      }
      inline var operator[](std::size_t i) const{
        return this->__vars[i];
      }
      inline var at(std::size_t i) const{
        if (i>=this->__size){
          std::cout<<"polynomialring [] error:i>=dim.\n";
          return 0;
        }
        return this->__vars[i];
      }
      
      inline var* begin() const{
        return this->__vars;
      } 
      inline var* end() const{
        return this->__vars_end;
      } 
      inline std::size_t size() const{
        return this->__size;
      }
      inline std::size_t data64_len() const
      {
        return this->__data64.__len;
      }
      inline std::size_t data64_range() const
      {
        return this->__data64.__range;
      }
      inline std::size_t data64_div() const
      {
        return this->__data64.__div;
      }
      inline std::size_t data64_size() const
      {
        return this->__data64.__size;
      }
      polynomialring& operator=(const polynomialring & R);
      polynomialring& operator=(polynomialring&& R);
      polynomialring  operator+(const polynomialring& R) const;
      bool operator==(const polynomialring& R) const;
      inline bool find(var i) const{
        return (*(std::lower_bound(this->begin(),this->end(),i))==i);
      }
  };  
  typedef std::pair<var,unsigned> var_pair;
  class monomial
  {
    public:
     typedef var_pair iterator;
    private:
      unsigned __deg;
      std::size_t __dim;
      var_pair* __times;
      var_pair* __times_end;
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
      monomial operator+(const monomial &m) const;
      inline monomial operator*(const monomial &m) const
      {
        return m+(*this);
      }
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
      inline unsigned operator[](var i) const
      {
        var_pair* j=this->find(i);
        if (j!=this->end())
          return j->second;
        else
          return 0;
      }
      unsigned re_deg();
      var_pair* find(var i) const;
      
      inline unsigned deg() const{
        return this->__deg;
      }
      inline var_pair* begin() const{
        return this->__times;
      }
      inline var_pair* end() const{
        return this->__times_end;
      }
      inline var_pair* at(std::size_t i) const{
        /*if (i>=this->size())
        {
          std::cout<<"monomial at "<<i<<" error. "<<i<<">=size.\n";
          i=0;
        }*/
        return this->__times+i;
      }
      inline std::size_t dim() const{
        return this->__dim;
      }
      inline std::size_t size() const{
        return this->__dim;
      }
      std::string str(std::string s="x",bool var_str=false) const;
    friend monomial monomial_transform(uint64_t m,const polynomialring &R);
    friend inline bool monomial_divisible(const monomial &m1,const monomial &m2,monomial &m3,polynomialring &R)
    {
      var_pair* ptr;
      std::vector<var_pair> v;
      ptr=m2.begin();
      for(auto &i:m1)
      {
        if (ptr!=m2.end() && ptr->first==i.first){
          if (ptr->second>i.second)
            return false;
          if (ptr->second<i.second)
            v.push_back(std::make_pair(ptr->first,i.second-ptr->second));
          ++ptr;
        }
        else
          v.push_back(i);
      }
      if (ptr!=m2.end())
        return false;
      m3=monomial(v.data(),v.size(),false,true,true);
      return true;
    }
  };
  /*
  class monomial_all
  {
    //public:
    // typedef var_pair iterator;
    private:
      unsigned __deg;
      std::size_t __dim;
      std::size_t* __times;
      std::size_t* __times_end;
    public:
      monomial_all()
      :__dim(0),__deg(0),__times(nullptr),__times_end(nullptr)
      {}
      //monomial(std::size_t n);
      monomial_all(std::size_t dim)
      :__dim(dim),__deg(0),__times(nullptr),__times_end(nullptr)
      {
        if (dim){
          this->__times=new std::size_t[this->__dim];
          this->__times_end=this->__times+this->__dim;        
          for(auto i=this->__times;i!=this->__times_end;++i)
            *i=0;
        
        }
      }
      monomial_all(std::size_t dim,var a)
      :__dim(dim),__deg(1),__times(nullptr),__times_end(nullptr)
      {
        if (dim>a)
        {
          this->__times=new std::size_t[this->__dim];
          this->__times_end=this->__times+this->__dim;        
          for(auto i=this->__times;i!=this->__times_end;++i)
            *i=0;
          this->__times[a]=1;  
        }
        else
          std::cout<<"monomial_all init error: dim>=a\n";
      }
      monomial_all(std::size_t* ts,std::size_t dim,bool is_move,std::size_t deg=0)
      :__dim(dim),__deg(deg),__times(nullptr),__times_end(nullptr)
      { 
        if (dim)
        {
          if (is_move)
            this->__times=ts;
          else{
            this->__times=new std::size_t[this->__dim];
            std::copy(ts,ts+dim,this->__times);  
          }
          this->__times_end=this->__times+this->__dim;
          if (!deg)
            for (auto i=this->__times;i!=this->__times_end;++i)
              this->__deg+=*i;
        }
      }
      monomial_all(const monomial_all &m)
      :__dim(m.__dim),__deg(m.__deg),__times(nullptr),__times_end(nullptr)
      {
        if (m.__dim)
        {
          this->__times=new std::size_t[this->__dim];
          std::copy(m.__times,m.__times_end,this->__times);  
          this->__times_end=this->__times+this->__dim;
        }
      }
      monomial_all(monomial_all &&m)
      :__dim(m.__dim),__deg(m.__deg),__times(m.__times),__times_end(m.__times_end)
      {
        m.__dim=m.__deg=0;m.__times=m.__times_end=nullptr;
      }
      ~monomial_all()
      {
        if (this->__times)
          delete [] this->__times;
      }
      inline void clear()
      {
        if (this->__times)
        {
          delete [] this->__times;
          this->__times=this->__times_end=nullptr;
        }
        this->__deg=this->__dim=0;
      }
      inline bool empty() const{
        return(this->__dim==0);
      }
      monomial_all operator+(const monomial_all &m) const
      {
        monomial_all newm;
        newm.__deg=this->__deg+m.__deg;
        std::size_t* ptr1,*ptr1_end,*ptr2,*ptr2_end,*ptr,dim;
        if (this->__dim>m.__dim){
          ptr1=this->__times;
          ptr2=m.__times;
          ptr1_end=this->__times_end;
          ptr2_end=m.__times_end;
          newm.__dim=this->__dim;
          ptr=new std::size_t[this->__dim];
        }
        else{
          ptr2=this->__times;
          ptr1=m.__times;
          ptr2_end=this->__times_end;
          ptr1_end=m.__times_end;
          newm.__dim=m.__dim;
          ptr=new std::size_t[m.__dim];
        }
        newm.__times=ptr;
        for(;ptr2!=ptr2_end;*ptr=*ptr2+*ptr1,++ptr,++ptr2,++ptr1);
        for(;ptr1!=ptr1_end;*ptr=*ptr1,++ptr,++ptr1);
        newm.__times_end=ptr;
        return newm;
      }
      inline monomial_all operator*(const monomial_all &m) const
      {
        return m+(*this);
      }
      monomial& operator=(const monomial &m){
        
      }
      monomial& operator=(monomial &&m)
      {

      }
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
        return this->__times_end;
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
      friend monomial monomial_transform(uint64_t m,const polynomialring &R);
  };
  */
 inline bool monomial_empty(const uint64_t &m)
  {
    return m==0;
  }
  inline bool monomial_empty(const monomial &m)
  {
    return m.empty();
  }
  inline void  monomial_gen(uint64_t & m,const std::size_t & index,const polynomialring &R)
  {
    m=1;
    if (index>=R.size())
    {
      std::cout<<"monomial_gen error index>=R.size.\n";
      exit(0);
    }
    if (R.size()!=1)
    {
      m<<=(index+1)*R.data64_len();
      ++m;
      m<<=(R.data64_size()-index-2)*R.data64_len();
    }
  }
    //size [] find begin end
  inline bool monomial_divisible(const uint64_t &m1,const uint64_t &m2, uint64_t &m,const polynomialring &R)
  {
    m=m1-m2;
    return ((m&R.data64_div())==0);
  }

  inline uint64_t uint64_get(uint64_t m,std::size_t index,polynomialring R)
  {
    //std::size_t len=64/;
    return (m>>(R.data64_len()*(R.data64_size()-index)))&R.data64_range();
  }
  uint64_t monomial_transform(const monomial &m,const polynomialring &R);
  monomial monomial_transform(uint64_t m,const polynomialring &R);
  inline std::string monomial_str(const uint64_t &m,std::string x, bool var_str,const polynomialring &R)
  {
    return monomial_transform(m,R).str(x,var_str);
  }
  inline std::string monomial_str(const monomial &m,std::string x, bool var_str,const polynomialring &R)
  {
    return m.str(x,var_str);
  }
 
  inline std::ostream& operator<<(std::ostream& os,const monomial &m)
  {
    os<<m.str();
    return os;
  }
  
  //template <typename Type> 
  //using monomial_pair=std::pair<monomial,Type>;
  template <typename Tm,typename Tc>
  class atomic_polynomial
  {
    public:
      typedef std::pair<Tm,Tc> monomial_pair;
    private:
      static const Tc __zero;
      std::size_t __size;
      monomial_pair* __coeffs;
      monomial_pair* __coeffs_end; 
      void atomic_polynomial_init
      (monomial_pair* cs,std::size_t size,bool is_sorted,bool is_RD);
    public:
      typedef monomial_pair iterator;
      atomic_polynomial()
      :__size(0),__coeffs(nullptr),__coeffs_end(nullptr)
      {}
      //atomic_polynomial(std::size_t dim);
      /*atomic_polynomial(const Tm &m)
      :__size(1)
      {
        this->__coeffs=new monomial_pair[1];
        this->__coeffs[0]=monomial_pair(m,1);
        this->__coeffs_end=this->__coeffs+1;
      }
      atomic_polynomial(Tm &&m)
      :__size(1)
      {
        this->__coeffs=new monomial_pair[1];
        this->__coeffs[0]=monomial_pair(m,1);
        this->__coeffs_end=this->__coeffs+1;
      }
      
      atomic_polynomial(int a)
      :__size(1)
      {
        this->__coeffs=new monomial_pair[1];
        this->__coeffs[0]=monomial_pair(Tm(),a);
        this->__coeffs_end=this->__coeffs+1;
      }
      */
      atomic_polynomial(const  Tc &a)
      :__size(1),__coeffs(nullptr),__coeffs_end(nullptr)

      {
        if (a!=0)
        {
          this->__coeffs=new monomial_pair[1];
          this->__coeffs[0]=monomial_pair(Tm(),a);
          this->__coeffs_end=this->__coeffs+1;
        }
        else{
          this->__size=0;
        }
      }
      atomic_polynomial(Tc &&a)
      :__size(1),__coeffs(nullptr),__coeffs_end(nullptr)
      {
        if (a!=0)
        {
          this->__coeffs=new monomial_pair[1];
          this->__coeffs[0]=monomial_pair(Tm(),a);
          this->__coeffs_end=this->__coeffs+1;
        }
        else{
          this->__size=0;
        }
      }
      atomic_polynomial(const Tm &m,const Tc &coeff)
      :__size(1),__coeffs(nullptr),__coeffs_end(nullptr)
      {
        if (coeff!=0)
        {
          this->__coeffs=new monomial_pair[1];
          this->__coeffs[0]=monomial_pair(m,coeff);
          //std::cout<<"poly init:"<<this->__coeffs->second<<std::endl;
          this->__coeffs_end=this->__coeffs+1;
        }
        else{
          this->__size=0;
        }
      }
      atomic_polynomial(Tm && m,Tc &&coeff)
      :__size(1),__coeffs(nullptr),__coeffs_end(nullptr)
      {
        if (coeff!=0)
        {
          this->__coeffs=new monomial_pair[1];
          this->__coeffs[0]=monomial_pair(m,coeff);
          //std::cout<<"poly init:"<<this->__coeffs->second<<std::endl;
          this->__coeffs_end=this->__coeffs+1;
        }
        else{
          this->__size=0;
        }
      }
      atomic_polynomial(const atomic_polynomial<Tm,Tc> &p)
      :__size(p.__size),__coeffs(nullptr),__coeffs_end(nullptr)
      {
        //std::cout<<"init 1\n";
        if (p.__size!=0){
          this->__coeffs=new monomial_pair[p.__size];
          std::copy(p.begin(),p.end(),this->__coeffs);
          this->__coeffs_end=this->__coeffs+p.__size;
        }
      }
      atomic_polynomial(atomic_polynomial<Tm,Tc> &&p)
      :__size(p.__size),__coeffs(p.__coeffs),__coeffs_end(p.__coeffs_end)
      {
        p.__size=0;
        p.__coeffs=nullptr;
        p.__coeffs_end=nullptr;
      }

      explicit atomic_polynomial(Tm* m,Tc* c,std::size_t size,bool is_move=false,bool is_sorted=false,bool is_RD=false);
      explicit atomic_polynomial(monomial_pair* cs1,std::size_t size,bool is_move=false,bool is_sorted=false,bool is_RD=false);
      void clear();
      ~atomic_polynomial();
    
      inline bool empty() const{
        return(this->__size==0);
      }
      // inline unsigned deg() const
      // {
      //   return this->__deg;
      // }
      inline std::size_t size() const
      {
        return this->__size;
      }
      inline monomial_pair* begin() const
      {
        return this->__coeffs;
      }
      inline monomial_pair* end() const
      {
        return this->__coeffs_end;
      }
      atomic_polynomial<Tm,Tc>& operator=(const atomic_polynomial<Tm,Tc> &p);
      atomic_polynomial<Tm,Tc>& operator=(atomic_polynomial<Tm,Tc> &&p);
      inline atomic_polynomial<Tm,Tc>& operator=(const Tc &p)
      {
        *this=atomic_polynomial(p);
        return *this;
      }
      
      atomic_polynomial<Tm,Tc>  operator*(const atomic_polynomial<Tm,Tc> &p) const;
      inline atomic_polynomial<Tm,Tc>&  operator*=(const atomic_polynomial<Tm,Tc> &p) 
      {
        *this=*this*p;
        return  *this;
      }
      inline atomic_polynomial<Tm,Tc>  operator*(const Tc & a) const
      {
        if (a==0)
          return atomic_polynomial<Tm,Tc>();
        atomic_polynomial<Tm,Tc> p(*this);
        for(auto & i:p)
          i.second*=a;
        return  p;
      }

      //atomic_polynomial<Tm,Tc>  operator*(const Type &i) const;
      atomic_polynomial<Tm,Tc>  operator+(const atomic_polynomial<Tm,Tc> &p) const;
      inline atomic_polynomial<Tm,Tc>  operator+() const
      {
        return  *this;
      }
      
      inline atomic_polynomial<Tm,Tc>&  operator+=(const atomic_polynomial<Tm,Tc> &p) 
      {
        *this=(*this+p);
        return  *this;
      }
      inline atomic_polynomial<Tm,Tc>  operator+(const Tc & a) const
      {
        if (this->size()>0 && monomial_empty((this->end()-1)->first))
        {
          atomic_polynomial<Tm,Tc> p(*this);
          (p.end()-1)->second+=a;
          if ((p.end()-1)->second==0)
          {
            --p.__size;
            --p.__coeffs_end;
          }
          return p;
        }
        if (a==0)
          return atomic_polynomial<Tm,Tc>(*this);
        std::pair<Tm,Tc>* cs=new std::pair<Tm,Tc>[this->size()+1];
        std::copy(this->begin(),this->end(),cs);
        cs[this->size()].first=Tm();
        cs[this->size()].second=a;
        return  atomic_polynomial<Tm,Tc>(cs,this->size()+1,true,true,true);
      }
      
      //atomic_polynomial<Tm,Tc>  operator+(const Type &i) const;
      atomic_polynomial<Tm,Tc>  operator-(const atomic_polynomial<Tm,Tc> &p) const;
      inline atomic_polynomial<Tm,Tc>  operator-() const
      {
        atomic_polynomial<Tm,Tc>  p(*this);
        for(auto & i:p)
          i.second=-i.second;
        return p;
      }
      inline atomic_polynomial<Tm,Tc>&  operator-=(const atomic_polynomial<Tm,Tc> &p)
      {
        *this=*this-p;
        return  *this;
      }
      inline atomic_polynomial<Tm,Tc>  operator-(const Tc & a) const
      {
        if (this->size()>0 && monomial_empty((this->end()-1)->first))
        {
          atomic_polynomial<Tm,Tc> p(*this);
          (p.end()-1)->second-=a;
          if ((p.end()-1)->second==0)
          {
            --p.__size;
            --p.__coeffs_end;
          }
          return p;
        }
        if (a==0)
          return atomic_polynomial<Tm,Tc>(*this);
        std::pair<Tm,Tc>* cs=new std::pair<Tm,Tc>[this->size()+1];
        std::copy(this->begin(),this->end(),cs);
        cs[this->size()].first=Tm();
        cs[this->size()].second=-a;
        return  atomic_polynomial<Tm,Tc>(cs,this->size()+1,true,true,true);
      }
      atomic_polynomial<Tm,Tc>  power(unsigned i) const;
      
      inline atomic_polynomial<Tm,Tc>  operator^(unsigned i) const
      {
        return this->power(i);
      }
      
      monomial_pair* find(const Tm& m) const;
      const Tc& operator[](const Tm& m) const;
      inline monomial_pair* at(std::size_t i) const{
        /*if (i>=this->size())
        {
          std::cout<<"monomial at "<<i<<" error. "<<i<<">=size.\n";
          i=0;
        }*/
        return this->begin()+i;
      }
      //Type set_coeff(const monomial& m,Type c);
      bool operator==(const atomic_polynomial<Tm,Tc> &p) const;
      inline bool operator==(const Tc &a) const
      {
        if (a==0)
          return (this->__size==0);
        if (this->__size==0 || !monomial_empty((this->__coeffs_end-1)->first))
          return false;
        return (this->__coeffs_end-1)->second==a;
        
      }
      
      bool operator<(const atomic_polynomial<Tm,Tc> &p) const;
      /*inline bool operator<(const Tc &a) const
      {
        if (this->__size==0)
          return 0<a;
        if (this->__size>1 || !monomial_empty((this->__coeffs_end-1)->first))
          return false;
        return this->__coeffs->second<a;
      }*/
      inline bool operator!=(const atomic_polynomial<Tm,Tc> &p) const{
        return !(p==*this);
      }
      inline bool operator!=(const Tc &p) const{
        return !(*this==p);
      }
      inline bool operator<=(const atomic_polynomial<Tm,Tc> &p) const{
        return (!(p<(*this)));
      }
      
      
      inline bool operator>(const atomic_polynomial<Tm,Tc> &p) const{
        return (p<(*this));
      }
      
      inline bool operator>=(const atomic_polynomial<Tm,Tc> &p) const{
        return (!((*this)<p));
      }
      std::string str(const polynomialring& R,std::string x="x", bool var_str=false) const; 
      //atomic_polynomial power(unsigned) const;
  };
  //template<typename Tm,typename Tc>
  //inline std::ostream& operator<<(std::ostream& os,const atomic_polynomial<Tm,Tc> &p)
  //  os<< p.str();
  //  return os;
  //}

  template <class Tm,class Tc>
  void polynomialring_gens(std::vector<atomic_polynomial<Tm,Tc>> &v,const polynomialring &R)
  {
    v.resize(R.size());
    Tm m;
    for (std::size_t i=0;i<R.size();++i)
    {
      monomial_gen(m,i,R);
      v[i]=atomic_polynomial<Tm,Tc>(m,1);
    }
  } 


  template<typename Tm1,typename Tm2,typename Tc>
  inline atomic_polynomial<Tm1,Tc> polynomial_transform(const atomic_polynomial<Tm2,Tc> &p,const polynomialring &R)
  {
    std::pair<Tm1,Tc>* cs=new std::pair<Tm1,Tc>[p.size()];
    std::pair<Tm1,Tc>* cs_ptr=cs;
    for(auto & i:p){
      cs_ptr->first=monomial_transform(i.first,R);
      cs_ptr->second=i.second;
      ++cs_ptr;
    } 
    return atomic_polynomial<Tm1,Tc>(cs,p.size(),true,true,true);
  }
  
  template <typename Type> 
  using polynomial_64=atomic_polynomial<uint64_t,Type>;
  template <class T>
  std::size_t pair_array_RD(T* l1,std::size_t size)
  {
    bool k=true;
    var i=0;
    while (i<size && l1[i].second==0)
      ++i;
    if (i==size)
      return 0;
    var size1=0;
    if (i>0)
      l1[size1]=std::move(l1[i]);
    ++i; 
    for(;i<size;++i)
      if (l1[i].second!=0)
        if (l1[i].first!=l1[size1].first){
          ++size1;
          if (size1<i)
            l1[size1]=std::move(l1[i]);
        }
        else
          std::get<1>(l1[size1])+=l1[i].second;
    return size1+1;
  }
  template <typename Type>
  bool pair_compare_max
  (const Type& firstElem, const Type& secondElem) {
  return firstElem.first > secondElem.first;
  }
  template <typename Type>
  bool pair_compare_min
  (const Type& firstElem, const Type& secondElem) {
  return firstElem.first < secondElem.first;
  }
  template<class Tm,class Type>
  void op_mult(const atomic_polynomial<Tm,Type> &p1,const atomic_polynomial<Tm,Type> &p2,std::vector<std::pair<Tm,Type>> &v);
  //template<class Tm>
  //void op_mult(const atomic_polynomial<Tm,mpz_class> &p1,const atomic_polynomial<Tm,mpz_class> &p2,std::vector<std::pair<Tm,mpz_class>> &v);
  
  template <typename Tm,typename Tc>
  const Tc atomic_polynomial<Tm,Tc>::__zero=0;

  template <typename Tm,typename Tc>
  void atomic_polynomial<Tm,Tc>::atomic_polynomial_init
  (monomial_pair* cs,std::size_t size,bool is_sorted,bool is_RD)
  {
    if (!is_sorted && !std::is_sorted(cs,cs+size,pair_compare_max<monomial_pair>))
      std::sort(cs,cs+size,pair_compare_max<monomial_pair>);
    if (is_RD)
    {
      this->__size=size;
      this->__coeffs=cs;
      this->__coeffs_end=this->__coeffs+this->__size;
    }
    else
    {
      this->__size=pair_array_RD(cs,size);
      if (this->__size==0){
        this->__coeffs=nullptr;
        this->__coeffs_end=nullptr;
        delete [] cs;
      }
      else{
        this->__coeffs=cs;
        if (this->__size<=size*SMT_MATH_POLYNOMIAL_polynomial_shrink_bound){
          this->__coeffs=new monomial_pair[this->__size];
          std::move(cs,cs+this->__size,this->__coeffs);
          delete [] cs;
        }
        this->__coeffs_end=this->__coeffs+this->__size;
      }

    }
  }
  template <typename Tm,typename Tc>
  atomic_polynomial<Tm,Tc>::atomic_polynomial
  (Tm* m,Tc* c,std::size_t size,bool is_move,bool is_sorted,bool is_RD)
  :__size(0),__coeffs(nullptr),__coeffs_end(nullptr)
  {
    if (size!=0)
    {
      monomial_pair *cs=new monomial_pair[size];
      if (is_move)
        for(std::size_t i=0;i<size;++i)
          cs[i]=monomial_pair(std::move(m[i]),std::move(c[i]));
      else
        for(std::size_t i=0;i<size;++i)
          cs[i]=monomial_pair(m[i],c[i]);
      this->atomic_polynomial_init(cs,size,is_sorted,is_RD);
    }
  }
  template <typename Tm,typename Tc>
  atomic_polynomial<Tm,Tc>::atomic_polynomial
  (monomial_pair* cs1,std::size_t size,bool is_move,bool is_sorted,bool is_RD)
  :__size(0),__coeffs(nullptr),__coeffs_end(nullptr)
  {
    if(size!=0)
    {
      monomial_pair* cs=cs1;
      if (!is_move)
      {
        cs=new monomial_pair[size];
        std::copy(cs1,cs1+size,cs);
      }
      this->atomic_polynomial_init(cs,size,is_sorted,is_RD);
    }
  }
  template <typename Tm,typename Tc>
  void atomic_polynomial<Tm,Tc>::clear()
  {
    if (this->__coeffs){
      delete [] this->__coeffs;
      this->__coeffs=nullptr;
      this->__coeffs_end=nullptr;
    }
    this->__size=0;
  }

  template <typename Tm,typename Tc>
  atomic_polynomial<Tm,Tc>::~atomic_polynomial()
  {
    this->clear();
  }

  template <typename Tm,typename Tc>
  atomic_polynomial<Tm,Tc>& atomic_polynomial<Tm,Tc>::operator=
  (const atomic_polynomial<Tm,Tc> &p)
  {
    //std::cout<<"= 1\n";
    //this->clear();
    if(p.__size){
      if (p.__size!=this->__size){
        delete [] this->__coeffs;
        this->__size=p.__size;
        this->__coeffs=new monomial_pair[this->__size];
        this->__coeffs_end=this->__coeffs+this->__size;
      }
      std::copy(p.begin(),p.end(),this->__coeffs);
    }
    else
      this->clear();
    return *this;
  }
  template <typename Tm,typename Tc>
  atomic_polynomial<Tm,Tc>& atomic_polynomial<Tm,Tc>::operator=(atomic_polynomial<Tm,Tc> &&p)
  {
    this->clear();
    this->__size=p.__size;
    p.__size=0;
    this->__coeffs=p.__coeffs;
    p.__coeffs=nullptr;
    this->__coeffs_end=p.__coeffs_end;
    p.__coeffs_end=nullptr;
    return *this;
  }

  template <typename Tm,typename Tc>
  atomic_polynomial<Tm,Tc> atomic_polynomial<Tm,Tc>::operator*(const atomic_polynomial<Tm,Tc> &p) const
  {
    if (this->size()*p.size()!=0)
    {
      std::vector<monomial_pair> v;
      if (this->size()<=p.size())
        op_mult(*this,p,v);
      else
        op_mult(p,*this,v);
      monomial_pair* cs=new monomial_pair[v.size()];
      std::move(v.begin(),v.end(),cs);
      atomic_polynomial<Tm,Tc> newp;
      newp.__coeffs=cs;
      newp.__size=v.size();
      newp.__coeffs_end=cs+v.size();
      return newp;

    }
    else
      return atomic_polynomial<Tm,Tc>();
    
  }
  template <typename Tm,typename Tc>
  atomic_polynomial<Tm,Tc> atomic_polynomial<Tm,Tc>::operator+(const atomic_polynomial<Tm,Tc> &p) const
  {
    //std::cout<<p.str()<<std::endl;
    if (p.empty())
      return atomic_polynomial<Tm,Tc>(*this);
    if (this->empty())
      return atomic_polynomial<Tm,Tc>(p);
    std::size_t size=this->size()+p.size();
    monomial_pair* l=new monomial_pair[size];
    //std::merge(this->begin(),this->end(),p.begin(),p.end(),l,pair_compare_max<monomial_pair>);
    auto l1=l;
    auto f_ptr=this->begin();
    auto p_ptr=p.begin();
    while (f_ptr!=this->end() && p_ptr!=p.end())
    {
      if (f_ptr->first>=p_ptr->first)
      {
        l1->first=f_ptr->first;
        if (p_ptr->first==f_ptr->first)
        {
          l1->second=f_ptr->second+p_ptr->second;
          ++p_ptr;
          if (l1->second!=0)
            ++l1;
        }
        else{
          l1->second=f_ptr->second;
          ++l1;
        }
        ++f_ptr;
        
      }
      else{
        l1->first=p_ptr->first;
        l1->second=p_ptr->second;
        ++p_ptr;
        ++l1;
      }
    }
    if (p_ptr!=p.end())
    {
      f_ptr=p_ptr;
      p_ptr=p.end();
    }
    else
      p_ptr=this->end();
    for(;f_ptr!=p_ptr;)
    {
        l1->first=f_ptr->first;
        l1->second=f_ptr->second;
        ++f_ptr;
        ++l1;
    }
    std::size_t size1=l1-l;
    if (size1<=size*SMT_MATH_POLYNOMIAL_polynomial_shrink_bound)
    {
      auto l2=new monomial_pair[size1];
      std::move(l,l1,l2);
      delete [] l;
      l=l2;
    }
    return atomic_polynomial<Tm,Tc>(l,size1,true,true,true);
  }
  template <typename Tm,typename Tc>
  atomic_polynomial<Tm,Tc> atomic_polynomial<Tm,Tc>::operator-(const atomic_polynomial<Tm,Tc> &p) const
  {
    //std::cout<<p.str()<<std::endl;
    if (p.empty())
      return *this;
    if (this->empty())
      return -p;
    std::size_t size=this->size()+p.size();
    monomial_pair* l=new monomial_pair[size];
    //std::merge(this->begin(),this->end(),p.begin(),p.end(),l,pair_compare_max<monomial_pair>);
    auto l1=l;
    auto f_ptr=this->begin();
    auto p_ptr=p.begin();
    while (f_ptr!=this->end() && p_ptr!=p.end())
    {
      if (f_ptr->first>=p_ptr->first)
      {
        l1->first=f_ptr->first;
        if (p_ptr->first==f_ptr->first)
        {
          l1->second=f_ptr->second-p_ptr->second;
          ++p_ptr;
          if (l1->second!=0)
            ++l1;
        }
        else{
          l1->second=f_ptr->second;
          ++l1;
        }
        ++f_ptr;
      }
      else{
        l1->first=p_ptr->first;
        l1->second=-p_ptr->second;
        ++p_ptr;
        ++l1;
      }
      
    }
    if (p_ptr!=p.end())
    {
      f_ptr=p_ptr;
      p_ptr=p.end();
      for(;f_ptr!=p_ptr;)
      {
          l1->first=f_ptr->first;
          l1->second=-f_ptr->second;
          ++f_ptr;
          ++l1;
      }
    
    }
    else
    {
      p_ptr=this->end();
      for(;f_ptr!=p_ptr;)
      {
          l1->first=f_ptr->first;
          l1->second=f_ptr->second;
          ++f_ptr;
          ++l1;
      }
    }
    std::size_t size1=l1-l;
    if (size1<=size*SMT_MATH_POLYNOMIAL_polynomial_shrink_bound)
    {
      auto l2=new monomial_pair[size1];
      std::move(l,l1,l2);
      delete [] l;
      l=l2;
    }
    return atomic_polynomial<Tm,Tc>(l,size1,true,true,true);
  } 
  template <typename Tm,typename Tc>
  atomic_polynomial<Tm,Tc> atomic_polynomial<Tm,Tc>::power(unsigned i) const
  {
    atomic_polynomial<Tm,Tc> p(*this);
    atomic_polynomial<Tm,Tc> newp(Tc(1));
    while (i!=0)
    {
      if (i%2!=0)
        newp=newp*p;
      i>>=1;
      if (i!=0)
        p=p*p;
    }
    return newp;
  }
  template<typename Tm,typename Tc>
  std::pair<Tm,Tc>* atomic_polynomial<Tm,Tc>::find(const Tm& m)const
  {
    if (this->empty())
      return this->end();
    monomial_pair  val(m,0);
    monomial_pair* low=
    std::lower_bound(this->begin(),this->end(),val,pair_compare_max<monomial_pair>);
    if (low->first!=m)
      return this->end();
    return low;
  }
  template<typename Tm,typename Tc>
  const Tc& atomic_polynomial<Tm,Tc>::operator[](const Tm& m)const
  {
    monomial_pair* low=this->find(m);
    if (low==this->end())
      return __zero;
    else
      return low->second;
  }
  template<typename Tm,typename Tc>
  bool atomic_polynomial<Tm,Tc>::operator==(const atomic_polynomial<Tm,Tc> &p) const{
    if (this->__size!=p.__size)
      return false;
    for(std::size_t i=0;i<this->__size;++i)
      if (this->__coeffs[i]!=p.__coeffs[i])
        return false;
    return true;
  }
  template<typename Tm,typename Tc>
  bool atomic_polynomial<Tm,Tc>::operator<(const atomic_polynomial<Tm,Tc> &p) const{
    std::size_t n=std::min(this->__size,p.__size);
    for(std::size_t i=0;i<n;++i)
      if (this->__coeffs[i]!=p.__coeffs[i])
        return this->__coeffs[i]<p.__coeffs[i];
    if (p.__dim>n)
        return true;
    return false;
  }
  template <typename Tc>
  inline std::string atomic_polynomial_str(const atomic_polynomial<monomial,Tc> &p)
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
        ss<<i->first.str();
      }
      else
        ss<<i->second;
    }
    return ss.str();
  }
  template <typename Tm,typename Tc>
  inline std::string atomic_polynomial_str(const atomic_polynomial<Tm,Tc> &p, const polynomialring& R,std::string x, bool var_str)
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
        ss<<monomial_str(i->first,x,var_str,R);
      }
      else
        ss<<i->second;
    }
    return ss.str();
  }
  template <typename Tm1,typename Tm2,typename Tc>
  inline std::string atomic_polynomial_str(const atomic_polynomial<Tm1,atomic_polynomial<Tm2,Tc>> &p, const polynomialring& R,std::string x, bool var_str)
  {
    if (p.size()==0)
      return "0";
    std::stringstream ss;
    for(auto i=p.begin();i!=p.end();++i)
    {
      if (i!=p.begin())
        ss<<"+";
      if (!monomial_empty(i->first)){
        ss<<"("<<i->second.str(R)<<")"<<"*";
        ss<<monomial_str(i->first,x,var_str,R);
      }
      else
        ss<<"("<<i->second.str(R)<<")";
    }
    return ss.str();
  }
  template <typename Tm1,typename Tm2,typename Tc>
  inline std::string atomic_polynomial_str(const atomic_polynomial<Tm1,atomic_polynomial<Tm2,Tc>> &p, const polynomialring& R1,const polynomialring& R2,std::string x, bool var_str)
  {
    if (p.size()==0)
      return "0";
    std::stringstream ss;
    for(auto i=p.begin();i!=p.end();++i)
    {
      if (i!=p.begin())
        ss<<"+";
      if (!monomial_empty(i->first)){
        ss<<"("<<i->second.str(R2)<<")"<<"*";
        ss<<monomial_str(i->first,x,var_str,R1);
      }
      else
        ss<<"("<<i->second.str(R2)<<")";
    }
    return ss.str();
  }
  /*
  template<typename Tc>
  inline std::string atomic_polynomial_str(const atomic_polynomial<uint64_t,Tc> &p,const polynomialring& R,std::string x, bool var_str) 
  {
    if (p.size()==0)
      return "0";
    std::stringstream ss;
    for(auto i=p.begin();i!=p.end();++i)
    {
      if (i!=p.begin() && i->second>0)
        ss<<"+";
      if (i->first!=0){
        if (i->second!=1)
          ss<< i->second<<"*";
        ss<<monomial_transform(i->first,R).str(x,var_str);
      }
      else
        ss<<i->second;
    }
    return ss.str();
  }
  */
  template <typename Tm,typename Tc>
  std::string atomic_polynomial<Tm,Tc>::str(const polynomialring& R,std::string x, bool var_str) const
  {
    return atomic_polynomial_str(*this,R,x,var_str);
  }
  
  template<class Tm,class ptr1,class ptr2>
  struct PHC{
    Tm mono;
    ptr1 p1_ptr;
    ptr2 p2_ptr;
    PHC<Tm,ptr1,ptr2>* next;
  };
  template<class Tm,class ptr1,class ptr2>
  inline PHC<Tm,ptr1,ptr2>* PHC_init(PHC<Tm,ptr1,ptr2>* H,ptr1 p1_ptr,ptr2 p2_ptr)
  {
    H->mono=p1_ptr->first+p2_ptr->first;
    H->p1_ptr=p1_ptr;
    H->p2_ptr=p2_ptr;
    H->next=nullptr;
    return H;
  }
  template<class Tm,class ptr1,class ptr2>
  inline void PHC_extract(PHC<Tm,ptr1,ptr2>** heap,std::size_t & heap_size)
  {
    std::size_t i,j,s;
    s = --heap_size;
    for (i=0, j=1; j < s; i=j, j=(j<<1)+1) {
      j = (heap[j]->mono > heap[j+1]->mono) ? j : j+1;
      if (heap[j]->mono>heap[s]->mono)
        heap[i] = heap[j];
      else{
        break;
        
      }
    }
    heap[i] =heap[s];
    
  }
  template<class Tm,class ptr1,class ptr2>
  inline void PHC_insert(PHC<Tm,ptr1,ptr2>** heap,std::size_t & heap_size,PHC<Tm,ptr1,ptr2>* newphc)
  {
    std::size_t i,j,i1;
    if (heap_size==0)
        {
          heap_size++;
          heap[0]=newphc;
          newphc->next=nullptr;
          //return void();
        }
        else
          if (newphc->mono==heap[0]->mono){
            newphc->next=heap[0];
            heap[0]=newphc;
            //return void();
          }
          else
            if (newphc->mono>heap[0]->mono){
              for (i=heap_size++, j=(i-1)>>1; i>0; heap[i]=heap[j], i=j, j=(j-1)>>1);
              heap[0]=newphc;newphc->next=nullptr;
              //return void();
            }
            else{
              for(i1=(heap_size-1)>>1;newphc->mono > heap[i1]->mono;i1=(i1-1)>>1);
              if (newphc->mono == heap[i1]->mono)
              {
                  newphc->next=heap[i1];
                  heap[i1]=newphc;
              }
              else{
                for (i=heap_size++, j=(i-1)>>1; j!=i1; heap[i]=heap[j], i=j, j=(j-1)>>1);
                heap[i]=newphc;newphc->next=nullptr;
              }
            }
  }

  template <class Type>
  inline void addmul(Type & op,const Type&op1,const Type&op2)
  {
    op+=op1*op2;
  }
  // inline void addmul(mpz_class &op,const mpz_class &op1,const mpz_class&op2)
  // {
  //   mpz_addmul(op.get_mpz_t(),op1.get_mpz_t(),op2.get_mpz_t());
  // }
  template <class Type>
  inline void submul(Type & op,const Type&op1,const Type&op2)
  {
    op-=op1*op2;
  }
  // inline void submul(mpz_class &op,const mpz_class &op1,const mpz_class&op2)
  // {
  //   mpz_submul(op.get_mpz_t(),op1.get_mpz_t(),op2.get_mpz_t());
  // }
  template<class Tm,class Tc>
  inline void v_push_add(std::vector<std::pair<Tm,Tc>> &v,Tm &m,Tc &k)
  {
    v.push_back(std::make_pair(std::move(m),std::move(k)));
  }
  template<class Tm,class Tc>
  inline void v_push_add(std::vector<std::pair<Tm,Tc>> &v,Tm &&m,Tc &&k)
  {
    v.push_back(std::make_pair(std::move(m),std::move(k)));
  }
  
  template<class Tm,class Tc>
  inline void l_push_add(std::list<std::pair<Tm,Tc>> &v,Tm &&m,Tc &&k)
  {
    v.push_back(std::make_pair(std::move(m),std::move(k)));
  }
  /*
  template<class Tm>
  inline void v_push_add(std::vector<std::pair<Tm,mpz_class>> &v,Tm &m,mpz_class &k)
  {
    v.push_back(std::make_pair(std::move(m),mpz_class()));
    swap(v.back().second,k);
  }
  template<class Tm>
  inline void l_push_add(std::list<std::pair<Tm,mpz_class>> &v,Tm &m,mpz_class &k)
  {
    v.push_back(std::make_pair(std::move(m),mpz_class()));
    swap(v.back().second,k);
  }
  */
  template<class Tm,class Type>
  void op_mult(const atomic_polynomial<Tm,Type> &p1,const atomic_polynomial<Tm,Type> &p2,std::vector<std::pair<Tm,Type>> &v)
  {
    if (p1.size()>p2.size()){
      std::cout<<"error: op_mult p1.size()>p2.size()\n";
      return void();
    }
    if (p1.size()==0)
      return void();
    Tm m;
    Type k;
    if (p2.size()==1)
    {
      m=p1.begin()->first+p2.begin()->first;
      k=p1.begin()->second*p2.begin()->second;
      v_push_add(v,m,k);
      return void();
    }
    PHC<Tm,std::pair<Tm,Type>*,std::pair<Tm,Type>*>* heap[p1.size()];
    PHC<Tm,std::pair<Tm,Type>*,std::pair<Tm,Type>*>* lin[p1.size()];
    std::size_t reset=1;
    PHC<Tm,std::pair<Tm,Type>*,std::pair<Tm,Type>*> *lout;
    std::size_t lin_size=0;
    auto p1_ptr=p1.begin();
    auto p2_begin=p2.begin();
    auto p2_end=p2.end();
    for(std::size_t i=0;i!=p1.size();++i)
      heap[i]=PHC_init(new PHC<Tm,std::pair<Tm,Type>*,std::pair<Tm,Type>*>,p1_ptr++,p2_begin);
    std::size_t heap_size=1;
    std::size_t i, j, s,i1;
    while (heap_size!=0)
    {
      //lin=nullptr;
      //lin_size=0;
      k=0;
      //if (k!=0 || lin_size!=0)
      //  std::cout<<k<<" "<<lin_size<<std::endl;
      m=std::move(heap[0]->mono);
      do{
        while(heap[0]!=nullptr){
          //k+=heap[0]->p1_ptr->second*heap[0]->p2_ptr->second;
          addmul(k,heap[0]->p1_ptr->second,heap[0]->p2_ptr->second);
          if (heap[0]->p2_ptr==p2_begin&& reset!=p1.size()){
            lin[lin_size++]=heap[reset++];
          }
          if (++(heap[0]->p2_ptr)!=p2_end){
            lin[lin_size++]=heap[0];
            heap[0]->mono=heap[0]->p1_ptr->first+heap[0]->p2_ptr->first;
            heap[0]=heap[0]->next;  
          }
          else{
            lout=heap[0];
            delete lout;
            heap[0]=heap[0]->next;
          }
        }
        PHC_extract(heap,heap_size);
        //end:
      }while (heap_size>0 && heap[0]->mono==m);
      if (k!=0){
        v_push_add(v,m,k);
        //v.push_back(std::make_pair(std::move(m),std::move(k)));
        //std::move(k)));
        //swap(v.back().second,k);
      }
      while(lin_size>0)
        PHC_insert(heap,heap_size,lin[--lin_size]);
    }  
  }
  
  template <class Tm,class Tz,class Tq>
  void polynomial_div(const atomic_polynomial<Tm,Tz> &f,const atomic_polynomial<Tm,Tz> &g, atomic_polynomial<Tm,Tq> &q,atomic_polynomial<Tm,Tq> &r,const polynomialring &R)
  {
    if (g.empty())
    {
      std::cout<<"polunomial_div g=0\n";
      return void();
    }
    if (f.empty())
    {
      q.clear();r.clear();
      return void();
    }
    std::size_t nq,nr;
    nq=0;nr=0;
    std::list<std::pair<Tm,Tq>> vq,vr;
    
    op_div(f,g,vq,vr,nq,nr,R);
    //q=atomic_polynomial<Tm,Tq>(vq.data(),vq.size(),false,true,true);
    std::pair<Tm,Tq>* cs=new std::pair<Tm,Tq>[nq];
    std::move(vq.begin(),vq.end(),cs);
    q=atomic_polynomial<Tm,Tq>(cs,nq,true,true,true);
    cs=new std::pair<Tm,Tq>[nr];
    std::move(vr.begin(),vr.end(),cs);
    r=atomic_polynomial<Tm,Tq>(cs,nr,true,true,true);


    //r=atomic_polynomial<Tm,Tq>(vr.data(),vr.size(),false,true,true);
    
  }
  template <class Tm,class Tz>
  inline void op_div(const atomic_polynomial<Tm,Tz> &f,const atomic_polynomial<Tm,Tz> &g, std::list<std::pair<Tm,Tz>> &vq,std::list<std::pair<Tm,Tz>> &vr,
              std::size_t &nq,std::size_t &nr,const polynomialring &R)
  {
    op_div_1(f,g,vq,vr,nq,nr,R);
  }
  template <class Tm,class Tz,class Tq>
  inline void op_div(const atomic_polynomial<Tm,Tz> &f,const atomic_polynomial<Tm,Tz> &g, std::list<std::pair<Tm,Tq>> &vq,std::list<std::pair<Tm,Tq>> &vr,
              std::size_t &nq,std::size_t &nr,const polynomialring &R)
  {
    op_div_2(f,g,vq,vr,nq,nr,R);
  }
  template <class Tm,class Tz,class Tq>
  void op_div_1(const atomic_polynomial<Tm,Tz> &f,const atomic_polynomial<Tm,Tz> &g, std::list<std::pair<Tm,Tq>> &vq,std::list<std::pair<Tm,Tq>> &vr,
              std::size_t &nq,std::size_t &nr,const polynomialring &R)
  
  {
    std::size_t heap_size=0;
    std::size_t lin_size=0;
    std::size_t nreset=g.size();
    
    auto f_ptr=f.begin();
    PHC<Tm,typename std::list<std::pair<Tm,Tq>>::iterator,std::pair<Tm,Tz>*>* heap[nreset],* lin[nreset],*gh[nreset];
    Tm m,m_div;
    Tq k;
    auto g_end=g.begin();
    typename std::list<std::pair<Tm,Tq>>::iterator vq_ptr;
    //std::pair<Tm,Tq>* q_ptr;
    for(auto i=0;i!=nreset;++i)
    {
      gh[i]=new PHC<Tm,typename std::list<std::pair<Tm,Tq>>::iterator,std::pair<Tm,Tz>*>;
      gh[i]->p2_ptr=g_end++;
    }
    --nreset;
    while (heap_size!=0 || f_ptr!=f.end())
    {
      if (heap_size==0 || (f_ptr!=f.end() && f_ptr->first>heap[0]->mono))
      {
        m=f_ptr->first;
        k=(f_ptr++)->second;
      }
      else{
        if(f_ptr!=f.end()&&f_ptr->first==heap[0]->mono)
          k=(f_ptr++)->second;
        else
          k=0;
        //std::cout<<k<<std::endl;
        m=std::move(heap[0]->mono);
        do{
          while(heap[0]!=nullptr){
            //k-=heap[0]->p1_ptr->second*heap[0]->p2_ptr->second;
            submul(k,heap[0]->p1_ptr->second,heap[0]->p2_ptr->second);
            //std::cout<<k<<std::endl;
            if ((++(heap[0]->p1_ptr))!=vq.end()){
              lin[lin_size++]=heap[0];
              heap[0]->mono=heap[0]->p1_ptr->first+heap[0]->p2_ptr->first;
              heap[0]=heap[0]->next;  
            }
            else{
              heap[0]=heap[0]->next;
              ++nreset;
            }
          }
          PHC_extract(heap,heap_size);
        }while (heap_size>0 && heap[0]->mono==m);
        
      }
      
      //std::cout<<monomial_transform(m,R)<<" "<<k<<std::endl;
      if (k!=0)
        if (monomial_divisible(m,g.begin()->first,m_div,R)){
          ++nq;
          k/=g.begin()->second;
          l_push_add(vq,std::move(m_div),std::move(k));
          //std::cout<<monomial_transform(m_div,R)<<std::endl;
          vq_ptr=vq.end();
          --vq_ptr;
          while (nreset>=1){
            lin[lin_size++]=gh[nreset];
            gh[nreset]->p1_ptr=vq_ptr;
            gh[nreset]->mono=gh[nreset]->p1_ptr->first+gh[nreset]->p2_ptr->first;
            --nreset;
          }
        }
        else{
          ++nr;
          l_push_add(vr,std::move(m),std::move(k));
        }
      while(lin_size>0)
      {
        --lin_size;
        PHC_insert(heap,heap_size,lin[lin_size]);
      }



    }
    for (std::size_t i=0;i<g.size();++i)
      delete gh[i];
    
  }
  template<class Tz>
  inline void op_mult(Tz &c,const Tz &c1,const Tz& c2)
  {
    c=c1*c2;
  }
  // inline void op_mult(mpz_class &c,const mpz_class &c1,const mpz_class & c2)
  // {
  //   mpz_mul(c.get_mpz_t(),c1.get_mpz_t(),c2.get_mpz_t());
  // }
  // inline boost::multiprecision::mpz_int denominator(const boost::multiprecision::mpq_rational & a)
  // {
  //   return boost::multiprecision::denominator(a);
  // }
  // inline boost::multiprecision::mpz_int numerator(const boost::multiprecision::mpq_rational & a)
  // {
  //   return boost::multiprecision::numerator(a);
  // }
  template<class Tz,class Tq>
  inline void submul(Tz &c,const Tz &s,const Tq &op1,const Tz &op2)
  {
    if (s==denominator(op1))
      c-=numerator(op1)*op2;  
    else
      c-=op2*numerator(op1)*s/denominator(op1);
  }
  // inline void submul(mpz_class &c,const mpz_class &s,const mpq_class &op1,const mpz_class &op2)
  // {
  //   mpz_t a;
  //   if (mpz_cmp(s.get_mpz_t(),op1.get_den_mpz_t())==0)
  //   {
  //     mpz_submul(c.get_mpz_t(),op1.get_num_mpz_t(),op2.get_mpz_t());
  //   }
  //   else{
  //     mpz_init(a);
  //     mpz_cdiv_q(a,s.get_mpz_t(),op1.get_den_mpz_t());
  //     mpz_mul(a,a,op1.get_num_mpz_t());
  //     mpz_submul(c.get_mpz_t(),a,op2.get_mpz_t());
  //     mpz_clear(a);
  //   }

  // }
  
  // inline void mulqz(mpz_class& c,mpz_class & s,const mpz_class & z)
  // {
  //   mpz_t a;
  //   if (mpz_divisible_p(c.get_mpz_t(),z.get_mpz_t()))
  //     mpz_cdiv_q(c.get_mpz_t(),c.get_mpz_t(),z.get_mpz_t());
  //   else{
  //     mpz_init(a);
  //     mpz_gcd(a,c.get_mpz_t(),z.get_mpz_t());
  //     mpz_cdiv_q(c.get_mpz_t(),c.get_mpz_t(),a);
  //     mpz_cdiv_q(a,z.get_mpz_t(),a);
  //     mpz_mul(s.get_mpz_t(),a,s.get_mpz_t());
  //     mpz_clear(a);
  //   }
  // }
  template <class Tm,class Tz,class Tq>
  void op_div_2(const atomic_polynomial<Tm,Tz> &f,const atomic_polynomial<Tm,Tz> &g, std::list<std::pair<Tm,Tq>> &vq,std::list<std::pair<Tm,Tq>> &vr,
              std::size_t &nq,std::size_t &nr,const polynomialring &R)
  
  {
    std::size_t heap_size=0;
    std::size_t lin_size=0;
    std::size_t nreset=g.size();
    Tz s=1;
    Tz c;
    auto f_ptr=f.begin();
    PHC<Tm,typename std::list<std::pair<Tm,Tq>>::iterator,std::pair<Tm,Tz>*>* heap[nreset],* lin[nreset],*gh[nreset];
    Tm m,m_div;
    //Tq k;
    auto g_end=g.begin();
    typename std::list<std::pair<Tm,Tq>>::iterator vq_ptr;
    //std::pair<Tm,Tq>* q_ptr;
    for(auto i=0;i!=nreset;++i)
    {
      gh[i]=new PHC<Tm,typename std::list<std::pair<Tm,Tq>>::iterator,std::pair<Tm,Tz>*>;
      gh[i]->p2_ptr=g_end++;
    }
    --nreset;
    while (heap_size!=0 || f_ptr!=f.end())
    {
      if (heap_size==0 || (f_ptr!=f.end() && f_ptr->first>heap[0]->mono))
      {
        m=f_ptr->first;
        //k=f_ptr->second;
        if (s==1) 
          c=(f_ptr++)->second;
        else
          op_mult(c,(f_ptr++)->second,s);
      }
      else{
        if(f_ptr!=f.end()&&f_ptr->first==heap[0]->mono)
          if (s==1) 
            c=(f_ptr++)->second;
          else
            op_mult(c,(f_ptr++)->second,s);
        else
          c=0;
        //std::cout<<k<<std::endl;
        m=std::move(heap[0]->mono);
        do{
          while(heap[0]!=nullptr){
            //k-=heap[0]->p1_ptr->second*heap[0]->p2_ptr->second;
            //submul(k,heap[0]->p1_ptr->second,heap[0]->p2_ptr->second);
            submul(c,s,heap[0]->p1_ptr->second,heap[0]->p2_ptr->second);
            //std::cout<<k<<std::endl;
            if ((++(heap[0]->p1_ptr))!=vq.end()){
              lin[lin_size++]=heap[0];
              heap[0]->mono=heap[0]->p1_ptr->first+heap[0]->p2_ptr->first;
              heap[0]=heap[0]->next;  
            }
            else{
              heap[0]=heap[0]->next;
              ++nreset;
            }
          }
          PHC_extract(heap,heap_size);
        }while (heap_size>0 && heap[0]->mono==m);
        
      }
      
      //std::cout<<monomial_transform(m,R)<<" "<<k<<std::endl;
      if (c!=0)
        if (monomial_divisible(m,g.begin()->first,m_div,R)){
          ++nq;
          //op_div_q(c,g.begin()->second);
          mulqz(c,s,g.begin()->second);
          //k/=g.begin()->second;
          l_push_add(vq,std::move(m_div),Tq(c,s));
          //std::cout<<monomial_transform(m_div,R)<<std::endl;
          vq_ptr=vq.end();
          --vq_ptr;
          while (nreset>=1){
            lin[lin_size++]=gh[nreset];
            gh[nreset]->p1_ptr=vq_ptr;
            gh[nreset]->mono=gh[nreset]->p1_ptr->first+gh[nreset]->p2_ptr->first;
            --nreset;
          }
        }
        else{
          ++nr;
          l_push_add(vr,std::move(m),Tq(c,s));
        }
      while(lin_size>0)
      {
        --lin_size;
        PHC_insert(heap,heap_size,lin[lin_size]);
      }



    }
    for (std::size_t i=0;i<g.size();++i)
      delete gh[i];
    
  }
  
  inline void lc(const uint64_t &mono,uint64_t &m,std::uint64_t &deg,const std::size_t &index,const  polynomialring& R)
  {
    if (index>=R.size()){
      std::cout<<"lc error index>=R.size\n";
      exit(0);
    }
    if (R.size()==1)
    {
      deg=mono;m=0;
    }
    else{
      deg=mono & (R.data64_range()<<((R.data64_size()-index-2)*R.data64_len()));
      m=mono-deg-(deg<<((index+1)*R.data64_len()));
      deg>>=((R.data64_size()-index-2)*R.data64_len());
      //std::cout<<"deg:"<<deg<<" "<<m<<" "<<mono<< std::endl;
      
    }
    //deg=mono && (R.data64_range()<<((R.data64_size()-index-1)*R.data64_len()));
    //std::cout<<"deg:"<<deg<< std::endl;
    //m=mono-deg-(deg<<(index*R.data64_len()));
    //deg>>=((R.size()-index-1)*R.data64_len());
  }
  template <class Tm,class Tc>
  polynomial_64<atomic_polynomial<Tm,Tc>> poly_to_uni(const atomic_polynomial<Tm,Tc> & f,const std::size_t &index,const polynomialring& R)
  {
    std::vector<std::pair<uint64_t,typename atomic_polynomial<Tm,Tc>::iterator>> v(f.size());
    std::size_t min_deg=SIZE_MAX;
    std::size_t max_deg=0;
    auto j=v.begin();
    for(auto i=f.begin();i!=f.end();++i,++j)
    {
      lc(i->first,j->second.first,j->first,index,R);
      j->second.second=i->second;
      if (max_deg<j->first)
        max_deg=j->first;
      if (min_deg>j->first)
        min_deg=j->first;
    }
    if ((max_deg-min_deg)<=f.size()*SMT_MATH_POLYNOMIAL_polynomial_to_uni_bound)
    {
      std::vector<std::vector<typename atomic_polynomial<Tm,Tc>::iterator>> uv(max_deg-min_deg+1);
      for(auto &i:v)
        uv[max_deg-i.first].push_back(i.second);
      std::vector<typename polynomial_64<atomic_polynomial<Tm,Tc>>::iterator> vp;
      for(std::size_t i=0;i<max_deg-min_deg+1;++i)
        if (!uv[i].empty())
          vp.push_back(typename polynomial_64<atomic_polynomial<Tm,Tc>>::iterator(max_deg-i,atomic_polynomial<Tm,Tc>(uv[i].data(),uv[i].size(),false,true,true)));
      return polynomial_64<atomic_polynomial<Tm,Tc>>(vp.data(),vp.size(),false,true,true);
    }
    else
    {
      std::map<uint64_t,std::vector<typename atomic_polynomial<Tm,Tc>::iterator>> dct;
      typename std::map<uint64_t,std::vector<typename atomic_polynomial<Tm,Tc>::iterator>>::iterator it;
      for(auto &i:v)
        if ((it=dct.find(i.first))!=dct.end())
          dct[i.first].push_back(i.second);
        else
        {
          dct[i.first]=std::vector<typename atomic_polynomial<Tm,Tc>::iterator>();
          dct[i.first].push_back(i.second);
        }
      std::vector<typename polynomial_64<atomic_polynomial<Tm,Tc>>::iterator> vp(dct.size());
      auto j1=vp.begin();
      for (auto i=dct.rbegin();i!=dct.rend();++i,++j1)
      {
        j1->first=i->first;
        j1->second=atomic_polynomial<Tm,Tc>(i->second.data(),i->second.size(),false,true,true);
      }
      return polynomial_64<atomic_polynomial<Tm,Tc>>(vp.data(),vp.size(),false,true,true);
      //uv[max_deg-i->first].push_back(i->second);
    }

  }
  template <class Tm,class Tc>
  inline atomic_polynomial<Tm,Tc> poly_to_uni_back(const std::list<typename polynomial_64<atomic_polynomial<Tm,Tc>>::iterator> & Ll,const std::size_t &nlr, const std::size_t &index,const polynomialring& R)
  {
    uint64_t nm,n;
    monomial_gen(nm,index,R);
    std::vector<typename atomic_polynomial<Tm,Tc>::iterator> vc;
    for (auto &Ll_1:Ll)
    {
      n=nm*(Ll_1.first);
      //cs1->first=nm*(Ll_ptr->first)+Ll_ptr->second.first;
      //cs1->second=Ll_ptr->second.second;
      for(auto &i:Ll_1.second)
        vc.push_back(std::pair<Tm,Tc>(i.first+n,i.second));
    }
    return atomic_polynomial<Tm,Tc>(vc.data(),vc.size());
  }
  template <class Tm,class Tc>
  inline atomic_polynomial<Tm,Tc>  prem(const atomic_polynomial<Tm,Tc> & f,const atomic_polynomial<Tm,Tc> & g,const std::size_t &index,const polynomialring& R)
  {
    if (index>=R.size())
      std::cout<<"prem error: index>dim/n";
    if (R.size()==0)
      return atomic_polynomial<Tm,Tc>();
    std::size_t nlq=0;
    std::size_t nlr=0;  
    if (R.size()==1)
    {
      std::list<std::pair<Tm,Tc>> lQ,lR;
      prem_uni(f,g,lQ,lR,nlq,nlr);
      std::pair<Tm,Tc>* cs=new std::pair<Tm,Tc>[nlr];
      std::move(lR.begin(),lR.end(),cs);
      return atomic_polynomial<Tm,Tc>(cs,nlr,true,true,true);
    
    }
    auto F1=poly_to_uni(f,index,R);
    auto G1=poly_to_uni(g,index,R);
    std::list<typename polynomial_64<atomic_polynomial<Tm,Tc>>::iterator> lQ,lR;
    prem_uni(G1,F1,lQ,lR,nlq,nlr);
    return poly_to_uni_back<Tm,Tc>(lR,nlr,index,R);
    //return prm_mul(f,g,index,R);
    //return f;
  }
  template <class Tm,class Tc>
  void  prem_uni(const atomic_polynomial<Tm,Tc> & f,const atomic_polynomial<Tm,Tc> & g,std::list<std::pair<Tm,Tc>> &lQ,std::list<std::pair<Tm,Tc>> &lR,std::size_t & nlq,std::size_t & nlr)
  {
    lQ.clear();lR.clear();nlq=0;nlr=0;
    if (f.begin()->first<g.begin()->first || f.size()==0 || g.size()==0)
      {
        for(auto &i:f){
          lR.push_back(i);
          ++nlr;
        }
        return void();
      }
    auto f_ptr=f.begin();
    typename std::list<std::pair<Tm,Tc>>::iterator lQ_ptr,lQ_ptr_1;
    Tc Iq(1);
    Tc k;  
    PHC<Tm,std::pair<Tm,Tc>*,typename std::list<std::pair<Tm,Tc>>::iterator>* heap[g.size()],*gh[g.size()],*lin[g.size()];
    std::size_t nreset=g.size()-1;
    
    auto phc1=gh;
    auto g_end=g.begin();
    for(;g_end!=g.end();++g_end,++phc1)
    {
      *phc1=new PHC<Tm,std::pair<Tm,Tc>*,typename std::list<std::pair<Tm,Tc>>::iterator>;
      (*phc1)->p1_ptr=g_end;
    }
    std::size_t heap_size=0;
    std::size_t lin_size=0;
    Tm m;
    while (f_ptr!=f.end() || heap_size!=0)
    {
      if (heap_size==0 || (f_ptr!=f.end() && f_ptr->first>heap[0]->mono))
      {
        m=f_ptr->first;
        if  (Iq==1)
          k=(f_ptr++)->second;
        else{
          //std::cout<<Iq.str(R)<<" "<<f_ptr->second.str(R)<<std::endl;
          op_mult(k,Iq,(f_ptr++)->second);
        }
      }
      else{
        if(f_ptr!=f.end()&&f_ptr->first==heap[0]->mono)  
          if  (Iq==1)
            k=(f_ptr++)->second;
          else
            op_mult(k,Iq,(f_ptr++)->second);
        else
          k=0;
        //std::cout<<k<<std::endl;
        m=std::move(heap[0]->mono);
        do{
          while(heap[0]!=nullptr){
            //k-=heap[0]->p1_ptr->second*heap[0]->p2_ptr->second;
            //submul(k,heap[0]->p1_ptr->second,heap[0]->p2_ptr->second);
            //op_mult(k1,heap[0]->p2_ptr->second->first,heap[0]->p2_ptr->second->second);
            //std::cout<<heap[0]->p1_ptr->second.str(R)<<" "<<heap[0]->p2_ptr->second.str(R)<<std::endl;
            submul(k,heap[0]->p1_ptr->second,heap[0]->p2_ptr->second);
            //std::cout<<k<<std::endl;
            if ((++(heap[0]->p2_ptr))!=lQ.end()){
              lin[lin_size++]=heap[0];
              heap[0]->mono=heap[0]->p1_ptr->first+heap[0]->p2_ptr->first;
              heap[0]=heap[0]->next;  
            }
            else{
              heap[0]=heap[0]->next;
              ++nreset;
            }
          }
          PHC_extract(heap,heap_size);
        }while (heap_size>0 && heap[0]->mono==m);
        
      }
      
      //std::cout<<monomial_transform(m,R)<<" "<<k<<std::endl;
      if (k!=0)
        if (m>=g.begin()->first){

          ++nlq;
          l_push_add(lQ,m-g.begin()->first,std::move(k));
          //std::cout<<monomial_transform(m_div,R)<<std::endl;
          lQ_ptr=lQ.end();
          --lQ_ptr;
          while (nreset>=1){
            lin[lin_size++]=gh[nreset];
            gh[nreset]->p2_ptr=lQ_ptr;
            gh[nreset]->mono=gh[nreset]->p1_ptr->first+gh[nreset]->p2_ptr->first;
            --nreset;
          }
          if (nlq>1)
          {
            //lQ_ptr_1=lQ_ptr;
            for(;lQ_ptr!=lQ.begin();(--lQ_ptr)->second*=g.begin()->second);
          }
          Iq*=g.begin()->second;
        }
        else{
          ++nlr;
          l_push_add(lR,std::move(m),std::move(k));

        }
        
      while(lin_size>0)
      {
        --lin_size;
        PHC_insert(heap,heap_size,lin[lin_size]);
      }


      
    }
  
    
    for (std::size_t i=0;i<g.size();++i)
      delete gh[i];
    //
  }  
  
  
    
  
  
}

#endif
