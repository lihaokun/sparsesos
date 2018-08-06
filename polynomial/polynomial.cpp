
#include "polynomial.h"
#include <iostream>
#include <algorithm>
#include <set>
//#include <string>
#include <sstream>
//#include <utility>
//#include <time.h>

namespace polynomial{

  
  monomial::monomial()
  :__deg(0),__dim(0),__times(nullptr)
  {}
  monomial::monomial(var v)
  :__deg(1),__dim(1)
  {
    this->__times=new var_pair[1];
    this->__times[0]=std::make_pair(v,1);
  }
  monomial::monomial(var v,unsigned time):__deg(time),__dim(1)
  {
    this->__times=new var_pair[1];
    this->__times[0]=std::make_pair(v,time);
  }
  void monomial::monomial_init(var_pair* l,std::size_t size,bool is_sorted,bool is_RD)
  {
      if (!is_sorted && !std::is_sorted(l,l+size))
        std::sort(l,l+size);
      if (is_RD)
      {
          this->__dim=size;
          this->__times=l;
      }
      else
      {
        this->__dim=pair_array_RD<var_pair>(l,size);
        if (this->__dim==0)
          this->__times=nullptr;
        else{
          this->__times=l;
          if (this->__dim<=size*SMT_MATH_POLYNOMIAL_monomial_shrink_bound)
          {
              this->__times=new var_pair[this->__dim];
              std::copy(l,l+this->__dim,this->__times);
              delete [] l;
          }
        }
      }
  }
  monomial::monomial(var* vs,unsigned* ts,std::size_t size,bool is_sorted,bool is_RD)
  :__deg(0),__dim(0),__times(nullptr)
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
  :__deg(0),__dim(0),__times(nullptr)
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
    if (m.empty())
      this->__times=nullptr;
    else
    {
      this->__times=new var_pair[m.__dim];
      std::copy(m.__times,m.__times+m.__dim,this->__times);
    }
  }
  monomial::monomial(monomial&& m)
  :__deg(m.__deg),__dim(m.__dim),__times(m.__times)
  {
    //printf("m init 2\n"); 
    m.__times=nullptr;
    m.__deg=0;
    m.__dim=0;
  }
  void monomial::clear()
  {
    if (this->__times)
      delete [] this->__times;
    this->__times=nullptr;
    this->__deg=0;
    this->__dim=0;
  }
  monomial::~monomial()
  {
    //std::cout<<this<<"\n";
    this->clear();
  }
  monomial monomial::operator*(const monomial &m )const
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
      if (this->__times)
        delete [] this->__times;
      this->__dim=0;this->__deg=0;
      return *this;
    }
    if (this->__dim!=m.__dim){
      if (this->__times)
        delete [] this->__times;
      this->__dim=m.__dim;
      this->__times=new var_pair[this->__dim];
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
    m.__times=nullptr;
    m.__deg=0;
    m.__dim=0;
    return *this;
  }
  bool monomial::operator<(const monomial &m) const
  {
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
      std::get<1>(newm.__times[i])*=n;
    return newm;
  }
  unsigned monomial::re_deg(){
    var size=pair_array_RD<var_pair>(this->__times,this->__dim);
    if (this->__dim!=size){
      var_pair* l=new var_pair[size];
      std::copy(this->__times,this->__times+size,l);
      if (this->__times)
        delete [] this->__times;
      this->__times=l;
      this->__dim=size;
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
      if (is_var_str && s.length()==this->__dim)
        ss<<s[i];
      else
        ss<<s<<this->at(i)->first;
      if (this->at(i)->second!=1)
        ss<<"^"<<(this->at(i)->second);
    }
    return ss.str();
  }



  /*template <class T1>
  inline void array_copy(T1*l1,T1*l2,std::size_t size)
  {
    for(var i=0;i<size;++i)
      l1[i]=l2[i];
  }*/
  
}
