#include <sstream>
#include <algorithm>


namespace polynomial{

  template <class T1>
  std::size_t pair_array_RD(T1* l1,std::size_t size);
  
  template <typename Type> 
  atomic_polynomial<Type>::atomic_polynomial()
  :__deg(0),__size(0),__coeffs(nullptr)
  {
    this->__vars.second=true;
  }

  template <typename Type> 
  atomic_polynomial<Type>::atomic_polynomial(const monomial &m)
  :__deg(m.deg()),__size(1)
  {
    for(auto& i:m)
      this->__vars.first.insert(i.first);
    this->__vars.second=true;
    //this->size=1;
    this->__coeffs=new monomial_pair<Type>[1];
    this->__coeffs[0]=std::make_pair(m,1);   
  }
  template <typename Type> 
  atomic_polynomial<Type>::atomic_polynomial(const Type &a)
  :__deg(0),__size(1)
  {
    this->__coeffs=new monomial_pair<Type>[1];
    this->__coeffs[0]=std::make_pair(monomial(),a);   
    this->__vars.second=true;
  }
  template <typename Type> 
  atomic_polynomial<Type>::atomic_polynomial(Type &&a)
  :__deg(0),__size(1)
  {
    this->__coeffs=new monomial_pair<Type>[1];
    this->__coeffs[0]=std::make_pair(monomial(),a);
    this->__vars.second=true;   
  }
  template <typename Type> 
  atomic_polynomial<Type>::atomic_polynomial(const monomial &m,const Type &coeff)
  :__deg(m.deg()),__size(1)
  {
    this->__coeffs=new monomial_pair<Type>[1];
    this->__coeffs[0]=std::make_pair(m,coeff);
    for(auto& i:this->__coeffs[0].first)
      this->__vars.first.insert(i.first);
    this->__vars.second=true;   
  }
    template <typename Type> 
  atomic_polynomial<Type>::atomic_polynomial(monomial&& m,Type &&coeff)
  :__deg(m.deg()),__size(1)
  {
    this->__coeffs=new monomial_pair<Type>[1];
    this->__coeffs[0]=std::make_pair(m,coeff);
    for(auto& i:this->__coeffs[0].first)
      this->__vars.first.insert(i.first);    //this->size=1;
    this->__vars.second=true;   
    
  }
  template <typename Type> 
  atomic_polynomial<Type>::atomic_polynomial(const atomic_polynomial<Type> &p)
  :__deg(p.__deg),__size(p.__size)
  {
    //std::cout<<"init 1\n";
    this->__vars=p.__vars;
    if (p.__size!=0){
      this->__coeffs=new monomial_pair<Type>[p.__size];
      std::copy(p.begin(),p.end(),this->__coeffs);
    }
  }
  template <typename Type> 
  atomic_polynomial<Type>::atomic_polynomial(atomic_polynomial<Type> &&p)
  :__deg(p.__deg),__size(p.__size),__coeffs(p.__coeffs)
  {
    //std::cout<<"init 2\n";
    if (p.__vars.second)
      this->__vars.first=std::move(p.__vars.first);
    this->__vars.second=p.__vars.second;
    p.__vars.second=true;
    p.__deg=0;
    p.__size=0;
    p.__coeffs=nullptr;
  }
  
  template <typename Type>
  bool monomial_pair_Compare
  (const monomial_pair<Type>& firstElem, const monomial_pair<Type>& secondElem) {
  return firstElem.first > secondElem.first;
  }
  template <typename Type>
  void atomic_polynomial<Type>::atomic_polynomial_init
  (monomial_pair<Type>* cs,std::size_t size,bool is_sorted,bool is_RD)
  {
    if (!is_sorted && !std::is_sorted(cs,cs+size,monomial_pair_Compare<Type>))
      std::sort(cs,cs+size,monomial_pair_Compare<Type>);
    if (is_RD)
    {
      this->__size=size;
      this->__coeffs=cs;
    }
    else
    {
      this->__size=pair_array_RD(cs,size);
      if (this->__size==0)
        this->__coeffs=nullptr;
      else{
        this->__coeffs=cs;
        if (this->__size<=size*SMT_MATH_POLYNOMIAL_polynomial_shrink_bound){
          this->__coeffs=new monomial_pair<Type>[this->__size];
          std::copy(cs,cs+this->__size,this->__coeffs);
          delete [] cs;
        }
      }
      
    }
      
  }
  template <typename Type>
  atomic_polynomial<Type>::atomic_polynomial
  (monomial* m,Type* c,std::size_t size,bool is_move,bool is_sorted,bool is_RD)
  :__deg(0),__size(0),__coeffs(nullptr)
  {
    if (size!=0)
    {
      monomial_pair<Type> *cs=new monomial_pair<Type>[size];
      for(var i=0;i<size;++i)
        if (is_move)
          cs[i]=std::make_pair(std::move(m[i]),c[i]);
        else
          cs[i]=std::make_pair(m[i],c[i]);
      this->atomic_polynomial_init(cs,size,is_sorted,is_RD);
      for(auto& i:*this)
        if (i.second!=0 && this->__deg<i.first.deg())
            this->__deg=i.first.deg();
          //for(auto& j:i.first)
          //  this->__vars.insert(j.first);
      this->__vars.second=false;  

    }
    else
      this->__vars.second=true;  

  }
  template <typename Type>
  atomic_polynomial<Type>::atomic_polynomial
  (monomial_pair<Type>* cs1,std::size_t size,bool is_move,bool is_sorted,bool is_RD)
  :__deg(0),__size(0),__coeffs(nullptr)
  {
    if(size!=0)
    {
      monomial_pair<Type>* cs=cs1;
      if (!is_move)
      {
        //std::cout<<"111\n";
        cs=new monomial_pair<Type>[size];
        //for(int i=0;i<size;++i)
        //  cs[i]=std::make_pair(cs1[i].first,cs1[i].second);
        std::copy(cs1,cs1+size,cs);
      }
      this->atomic_polynomial_init(cs,size,is_sorted,is_RD);
      for(auto& i:*this)
        if (i.second!=0 && this->__deg<i.first.deg())
            this->__deg=i.first.deg();
          //for(auto& j:i.first)
          //  this->__vars.insert(j.first);
      this->__vars.second=false;  
    
    }
    else
      this->__vars.second=true;  
  }
  template <typename Type>
  void atomic_polynomial<Type>::clear()
  {
    if (this->__coeffs)
      delete [] this->__coeffs;
    this->__coeffs=nullptr;
    this->__deg=0;
    this->__size=0;  
    this->__vars.first.clear();
    this->__vars.second=true;
  }
  
  template <typename Type>
  atomic_polynomial<Type>::~atomic_polynomial()
  {
    this->clear();
  }

  template <typename Type>
  atomic_polynomial<Type>& atomic_polynomial<Type>::operator=(const atomic_polynomial<Type> &p)
  {
    //std::cout<<"= 1\n";
    this->clear();
    this->__deg=p.__deg;
    if (p.__vars.second)
      this->__vars=p.__vars;
    this->__size=p.__size;
    if (this->__size){
      this->__coeffs=new monomial_pair<Type>[this->__size];
      std::copy(p.begin(),p.end(),this->__coeffs);
    }
    return *this;
  }
  template <typename Type>
  atomic_polynomial<Type>& atomic_polynomial<Type>::operator=(atomic_polynomial<Type> &&p)
  {
    //std::cout<<"= 2\n";
    this->clear();
    this->__deg=p.__deg;
    p.__deg=0;
    this->__size=p.__size;
    p.__size=0;
    if (p.__vars.second)
      this->__vars.first=std::move(p.__vars.first);
    this->__vars.second=p.__vars.second;
    p.__vars.second=true;
    this->__coeffs=p.__coeffs;
    p.__coeffs=nullptr;
    return *this;
  }

  template <typename Type>
  atomic_polynomial<Type> atomic_polynomial<Type>::operator*(const atomic_polynomial<Type> &p) const
  {
    if (this->size()*p.size()!=0)
    {
      monomial_pair<Type>* cs=new monomial_pair<Type>[this->size()*p.size()];
      std::size_t n=0;
      for(auto& i:*this)
        for(auto& j:p){
          cs[n]=std::make_pair(i.first*j.first,i.second*j.second);
          ++n;
        }
      atomic_polynomial<Type> newp;
      newp.atomic_polynomial_init(cs,this->size()*p.size(),false,false);
      newp.__deg=this->__deg*p.__deg;
      //newp.com_var();
      newp.__vars.second=false;
      return newp;
    }
    else
      return atomic_polynomial<Type>();
  }
  template <typename Type>
  atomic_polynomial<Type> atomic_polynomial<Type>::operator+(const atomic_polynomial<Type> &p) const
  {
    if (p.empty())
      return atomic_polynomial<Type>(*this);
    if (this->empty())
      return atomic_polynomial<Type>(p);
    std::size_t size=this->size()+p.size();
    monomial_pair<Type>* l=new monomial_pair<Type>[size];
    std::merge(this->begin(),this->end(),p.begin(),p.end(),l,monomial_pair_Compare<Type>);
    return atomic_polynomial<Type>(l,size,true,true);
  }
  template<typename Type>
  monomial_pair<Type>* atomic_polynomial<Type>::find(const monomial& m)const
  {
    if (this->empty())
      return this->end();
    monomial_pair<Type>  val(m,0);
    monomial_pair<Type>* low=
    std::lower_bound(this->begin(),this->end(),val,monomial_pair_Compare<Type>);
    if (low->first!=m)
      return this->end();
    return low;
  }
  template<typename Type>
  const Type& atomic_polynomial<Type>::operator[](const monomial& m)const
  {
    monomial_pair<Type>* low=this->find(m);
    if (low==this->end())
      return this->zero;
    else
      return low->second;
  }
  template<typename Type>
  bool atomic_polynomial<Type>::operator==(const atomic_polynomial<Type> &p) const{
    if (this->__size!=p.__size)
      return false;
    for(std::size_t i=0;i<this->__dim;++i)
      if (this->__coeffs[i]!=p.__coeffs[i])
        return false;
    return true;
  }
  template<typename Type>
  bool atomic_polynomial<Type>::operator<(const atomic_polynomial<Type> &p) const{
    std::size_t n=std::min(this->__size,p.__size);
    for(std::size_t i=0;i<n;++i)
      if (this->__coeffs[i]!=p.__coeff[i])
        return this->__coeffs[i]<p.__coeff[i];
    if (p.__dim>n)
        return true;
    return false;
  }
  template<typename Type>
  void atomic_polynomial<Type>::com_var()
  {
    this->__vars.first.clear();
    var n=0;
    for(auto& i:*this)
      if (!i.first.empty() && i.first.at(i.first.size()-1)->first>n)
        n=i.first.at(i.first.size()-1)->first;
    n=n+1;
    if (n>0)
    if (n<SMT_MATH_POLYNOMIAL_polynomial_var_bound)
    {
      bool* a=new bool[n];
      for(int i=0;i<n;++i)
        a[i]=false;
      for(auto& i:*this)
        for(auto& j:i.first)
          a[j.first]=1;
      for(var i=0;i<n;++i)
        if(a[i])
          this->__vars.first.insert(i);
      delete [] a;
    }
    else
      for(auto& i:*this)
        for(auto& j:i.first)
          this->__vars.first.insert(j.first);
    this->__vars.second=true;
  }
  
  template <class T>
  std::string atomic_polynomial<T>::str(std::string x, bool var_str) const
  {
    if (this->size()==0)
      return "0";
    std::stringstream ss;
    for(auto i=this->begin();i!=this->end();++i)
    {
      if (i!=this->begin() && i->second>0)
        ss<<"+";
      if (!i->first.empty()){
        if (i->second!=1)
          ss<< i->second<<"*";
        ss<<i->first.str(x,var_str);
      }
      else
        ss<<i->second;
    }
    return ss.str();
  }

  /*template <class T1>
  inline void array_copy(T1*l1,T1*l2,std::size_t size)
  {
    for(var i=0;i<size;++i)
      l1[i]=l2[i];
  }*/
  template <class T>
  std::size_t pair_array_RD(T* l1,std::size_t size)
  {
    var size1=0;
    bool k=true;
    for(var i=0;i<size;++i)
      if (l1[i].second!=0)
        if (k || l1[i].first!=l1[size1].first){
          if (k)
            k=false;
          else
            ++size1;
          if (size1<i)
            l1[size1]=l1[i];
        }
        else
          std::get<1>(l1[size1])+=l1[i].second;
    if (k)
      return 0;
    return size1+1;
  }
  
    

  
}
