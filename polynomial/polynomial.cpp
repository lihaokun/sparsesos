
#include "polynomial.h"
#include <iostream>
#include <algorithm>
#include <set>
#include <sstream>


namespace polynomial{

  
  monomial::monomial()
  :__deg(0),__dim(0),__times(nullptr),__times_end(nullptr)
  {}
  monomial::monomial(var v)
  :__deg(1),__dim(1)
  {
    this->__times=new var_pair[1];
    this->__times[0]=std::make_pair(v,1);
    this->__times_end=this->__times+this->__dim;
  }
  monomial::monomial(var v,unsigned time):__deg(time),__dim(1)
  {
    this->__times=new var_pair[1];
    this->__times[0]=std::make_pair(v,time);
    this->__times_end=this->__times+this->__dim;
  }
  void monomial::monomial_init(var_pair* l,std::size_t size,bool is_sorted,bool is_RD)
  {
      if (!is_sorted && !std::is_sorted(l,l+size))
        std::sort(l,l+size);
      if (is_RD)
      {
          this->__dim=size;
          this->__times=l;
          this->__times_end=this->__times+this->__dim;
      }
      else
      {
        this->__dim=pair_array_RD<var_pair>(l,size);
        if (this->__dim==0){
          this->__times=nullptr;
          this->__times_end=nullptr;
          delete [] l;
        }
        else{
          this->__times=l;
          if (this->__dim<=size*SMT_MATH_POLYNOMIAL_monomial_shrink_bound)
          {
              this->__times=new var_pair[this->__dim];
              std::copy(l,l+this->__dim,this->__times);
              delete [] l;
          }
          this->__times_end=this->__times+this->__dim;
        }
      }
  }
  monomial::monomial(var* vs,unsigned* ts,std::size_t size,bool is_sorted,bool is_RD)
  :__deg(0),__dim(0),__times(nullptr),__times_end(nullptr)
  {
    if (size!=0)
    {
      var_pair* l=new var_pair[size];
      for(var i=0;i<size;++i)
      {
        l[i]=std::make_pair(vs[i],ts[i]);
      }
      monomial_init(l,size,is_sorted,is_RD);
      for(var i=0;i<this->__dim;++i)
        this->__deg+=this->__times[i].second;

    }
  }
  monomial::monomial(var_pair* ts,std::size_t size,bool is_move,bool is_sorted,bool is_RD)
  :__deg(0),__dim(0),__times(nullptr),__times_end(nullptr)
  {
    if (size!=0)
    {
      var_pair* l=ts;
      if(!is_move){
        l=new var_pair[size];
        std::copy(ts,ts+size,l);
      }
      monomial_init(l,size,is_sorted,is_RD);
      this->__deg=0;
      for(var i=0;i<this->__dim;++i)
        this->__deg+=this->__times[i].second;
    }
  }
  monomial::monomial(const monomial &m)
  :__deg(m.__deg),__dim(m.__dim)
  {
    //printf("m init 1\n");
    if (m.empty()){
      this->__times=nullptr;
      this->__times_end=nullptr;
    }
    else
    {
      this->__times=new var_pair[m.__dim];
      std::copy(m.__times,m.__times+m.__dim,this->__times);
      this->__times_end=this->__times+this->__dim;
    }
  }
  monomial::monomial(monomial&& m)
  :__deg(m.__deg),__dim(m.__dim),__times(m.__times),__times_end(m.__times_end)
  {
    //printf("m init 2\n"); 
    m.__times=nullptr;
    m.__times_end=nullptr;
    m.__deg=0;
    m.__dim=0;
  }
  void monomial::clear()
  {
    if (this->__times){
      delete [] this->__times;
      this->__times=nullptr;
      this->__times_end=nullptr;
    }
    this->__deg=0;
    this->__dim=0;
  }
  monomial::~monomial()
  {
    //std::cout<<this<<"\n";
    this->clear();
  }
  monomial monomial::operator+(const monomial &m )const
  {
    if (m.empty())
      return monomial(*this);
    if (this->empty())
      return monomial(m);
    var size=this->size()+m.size();
    var_pair* l=new var_pair[size];
    std::merge(this->begin(),this->end(),m.begin(),m.end(),l);
    //return monomial(l,size,true,true,false);
    monomial newm;
    newm.monomial_init(l,size,true,false);
    newm.__deg=this->__deg+m.deg();
    return newm;
  }
  monomial& monomial::operator=(const monomial &m)
  {
    //printf("m = 1\n");
    if (m.empty())
    {
      this->clear();
      return *this;
    }
    if (this->__dim!=m.__dim){
      if (this->__times)
        delete [] this->__times;
      this->__dim=m.__dim;
      this->__times=new var_pair[this->__dim];
      this->__times_end=this->__times+this->__dim;
    }
    this->__deg=m.__deg;
    std::copy(m.begin(),m.end(),this->__times);
    return *this;
  }
  monomial& monomial::operator=(monomial &&m)
  {
    //printf("m = 2\n");
    this->clear();
    this->__times=m.__times;
    this->__deg=m.__deg;
    this->__dim=m.__dim;
    this->__times_end=m.__times_end;
    m.__times=nullptr;
    m.__times_end=nullptr;
    m.__deg=0;
    m.__dim=0;
    return *this;
  }
  bool monomial::operator<(const monomial &m) const
  {
    if (this->__deg!=m.__deg)
      return this->__deg<m.__deg;
    std::size_t n=std::min(this->__dim,m.__dim);
    for(std::size_t i=0;i<n;++i)
      if (this->__times[i]!=m.__times[i])
        return (this->__times[i].first>m.__times[i].first ||
                (this->__times[i].first==m.__times[i].first && 
                 this->__times[i].second<m.__times[i].second));
    
    if (m.__dim>n)
        return true;
    return false;
  }
  bool monomial::operator==(const monomial &m) const
  {
    if (this->__deg!=m.__deg)
      return false;
    if (this->__dim!=m.__dim)
      return false;
    for(std::size_t i=0;i<this->__dim;++i)
      if (this->__times[i]!=m.__times[i])
        return false;
    return true;
  }
  monomial monomial::power(unsigned n) const
  {
    monomial newm(*this);
    //std::cout<<newm.__dim<<this->__dim<<std::endl;
    newm.__deg*=n;
    for(std::size_t i=0;i<this->__dim;++i)
      newm.__times[i].second*=n;
    return newm;
  }
  unsigned monomial::re_deg(){
    var size=pair_array_RD<var_pair>(this->__times,this->__dim);
    if (size<=this->__dim*SMT_MATH_POLYNOMIAL_monomial_shrink_bound){
      var_pair* l=new var_pair[size];
      std::copy(this->__times,this->__times+size,l);
      if (this->__times)
        delete [] this->__times;
      this->__times=l;
      this->__dim=size;
      this->__times_end=this->__times+this->__dim;
    }
    this->__deg=0;
    for(auto& i:*this)
      this->__deg+=i.second;
    return this->__deg;
  }
  var_pair* monomial::find(var i) const{
        if (this->empty())
          return this->end();
        var_pair  val(i,0);
        var_pair* low=std::lower_bound(this->begin(),this->end(),val);
        if (low->first!=i)
          return this->end();
        return low;

  }
  
  unsigned monomial::operator[](var i) const
  {
    var_pair* j=this->find(i);
    if (j!=this->end())
      return j->second;
    else
      return 0;
  }
  /*
  unsigned monomial::set_time(var i,unsigned j)
  {
    if (i>=this->__dim)
    {
      std::cout<<"monomial []  error:var is greater than the upper bound.\n";
      i=0;  
    }
    else
    {
      this->__deg+=j-this->__times[i];
      this->__times[i]=j;   
    }
    return this->__times[i];
  }
  */    
  std::string monomial::str(std::string s,bool is_var_str) const
  { 
    std::stringstream ss;
    if (this->empty())
      return "1";
    for(std::size_t i=0;i<this->__dim;++i)
    {
      if (i!=0)
        ss<<"*";
      if (is_var_str && 1==this->__dim)
        ss<<s;
      else
        ss<<s<<this->at(i)->first;
      if (this->at(i)->second!=1)
        ss<<"^"<<(this->at(i)->second);
    }
    return ss.str();
  }
  /*
  polynomialring::polynomialring():__vars(nullptr),__vars_end(nullptr),__size(0)
  {}
  polynomialring::polynomialring(var* vs1,std::size_t size,bool is_move,bool is_sorted,bool is_RD)
  :__vars(nullptr),__vars_end(nullptr),__size(0)
  {
    if (size){
      var* vs=vs1;
      if (!is_move)
      {
        vs=new var[size];
        std::copy(vs1,vs1+size,vs);
      }
      if (!is_sorted && !std::is_sorted(vs,vs+size))
        std::sort(vs,vs+size);
      if (is_RD)
      {
        this->__vars=vs;this->__vars_end=vs+size;
        this->__size=size;
      }
      else{
        std::size_t i=0;
        std::size_t size1=1;
        for(++i;i<size;++i)
          if (vs[i]!=vs[size1-1]){
            vs[size1]=vs[i];
            ++size1;
          }
        this->__size=size1;
        if (size1<size*SMT_MATH_POLYNOMIAL_monomial_shrink_bound){
          this->__vars=new var[size1];
          std::copy(vs,vs+size1,this->__vars);
          delete [] vs;
        }
        else
          this->__vars=vs;
        this->__vars_end=this->__vars+this->__size;              
      
      }
    }
  }
  polynomialring::polynomialring(const polynomialring& R)
  :__size(R.__size)
  {
    this->__vars=new var[this->__size];
    std::copy(R.begin(),R.end(),this->__vars);
    this->__vars_end=this->__vars+this->__size;
  }
  polynomialring::polynomialring(polynomialring && R)
  :__size(R.__size),__vars(R.__vars),__vars_end(R.__vars_end)
  {
    R.__size=0;
    R.__vars=nullptr;R.__vars_end=nullptr;
  }
  void polynomialring::clear()
  {
    if (this->__vars){
      delete [] this->__vars;
      this->__vars=nullptr;
      this->__vars_end=nullptr;
    }
    this->__size=0;
  }
  polynomialring::~polynomialring()
  {
    this->clear();
  }
  polynomialring& polynomialring::operator=(const polynomialring &R){
    this->clear();
    if (R.__size)
    {
      this->__size=R.__size;
      this->__vars=new var[R.__size];
      this->__vars_end=this->__vars+this->__size;
      std::copy(R.__vars,R.__vars_end,this->__vars);
    }
    return *this;
  }
  polynomialring& polynomialring::operator=(polynomialring &&R)
  {
    this->clear();
    this->__size=R.__size;
    R.__size=0;
    this->__vars=R.__vars;
    R.__vars=nullptr;
    this->__vars_end=R.__vars_end;
    R.__vars_end=nullptr;
    return *this;
  }
  polynomialring polynomialring::operator+(const polynomialring &R) const
  {
    if(this->__size+R.__size!=0){
      var* vs=new var[this->__size+R.__size];
      var* it=std::set_union(this->begin(),this->end(),R.begin(),R.end(),vs);
      std::size_t size=it-vs;
      polynomialring newR;
      newR.__size=size;
      if(size<(this->__size+R.__size)*SMT_MATH_POLYNOMIAL_monomial_shrink_bound){
        newR.__vars=new var[size];
        newR.__vars_end=newR.__vars+size;
        std::copy(vs,vs+size,newR.__vars);
        delete [] vs;
      }
      else
        newR.__vars=vs;
      newR.__vars_end=newR.__vars+size;
      return newR;
    }
    else
      return polynomialring();
  }
  bool polynomialring::operator==(const polynomialring& R) const
  {
    if (this->__size!=R.__size)
      return false;
    for(std::size_t i=0;i!=this->__size;++i)
      if (this->__vars[i]!=R.__vars[i])
        return false;
    return true;
  }
  uint64_t monomial_transform(const monomial & m,const polynomialring &R)
  {
    if (R.size()==0)
      return 0;
    if (R.size()==1)
      return m.deg();
    auto m_ptr=m.begin();
    auto r_ptr=R.begin();
    uint64_t a(m.deg());
    std::size_t len=64/(R.size()+1);
    //std::cout<<a<<" ";
    while (r_ptr!=R.end() )
    {
      //std::cout<<a<<" ";
      a<<=len;
      if (m_ptr!=m.end() && *r_ptr==m_ptr->first){
        a+=m_ptr->second;
        ++m_ptr;
      }
      ++r_ptr;
    }
    if (m_ptr!=m.end())
      std::cout<<"monomial_transform error:m_ptr!=m.end().\n";
    return a;
  }
  monomial monomial_transform(uint64_t m,const polynomialring &R)
  {
    if (R.size()==0)
      return monomial();
    if (R.size()==1)
      return monomial(*R.begin(),m);
    std::size_t len=64/(R.size()+1);
    uint64_t len2=(1<<len)-1;
    //std::cout<<len2<<" ";
    monomial newm;
    //var_pair * vs=new var_pair[R.size()];
    if (R.size()==1){
      newm.__times=new var_pair[1];
      newm.__times[0].second=m;
      newm.__times[0].first=*(R.begin());
      newm.__dim=R.size();
      newm.__times_end=newm.__times+1;
      newm.__deg=m;
      return newm;
    }
    var_pair * vs=new var_pair[R.size()];
      
    auto r_ptr=R.begin();
    for(std::size_t i=0;i<R.size();++i)
    {
      vs[i].first=*r_ptr;
      ++r_ptr;
      vs[R.size()-1-i].second=m&len2;
      m>>=len;
    }
    newm.monomial_init(vs,R.size(),true,false);
    newm.__deg=m;
    return newm;
  }*/
}
