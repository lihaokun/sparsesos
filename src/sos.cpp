#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include "polynomial/polynomial.hpp"
#include "sos.hpp"
using namespace std;
using namespace polynomial;

namespace is_sos{
    bool num_init(vector<int> &num,int &snum,const vector<int> &deg_maxv,const vector<int> &deg_minv,const int &degmax,const int &degmin)
    {
        int m=num.size();
        int i=0;
        //num[m]=-1;
        snum=0;
        for(;i<m;++i)
        {
            num[i]=deg_minv[i];
            if (num[i]>=0)
                snum+=num[i];
        }  
        if (snum>degmax)
            return false;
        if (snum<degmin)
            return num_next(num,snum,deg_maxv,deg_minv,degmax,degmin);
        return true;
        
    }
    bool num_next(vector<int> &num,int &snum,const vector<int> &deg_maxv,const vector<int> &deg_minv,const int &degmax,const int &degmin)
    {
        int m=num.size();
        int i=m-1;
        if (snum>degmax)
            return false;
        if (snum<degmin)
        {
            while (i>=0)
            {
                if (num[i]>=0)
                if (degmin-snum<=deg_maxv[i]-num[i])
                {
                    
                    num[i]+=degmin-snum;
                    snum=degmin;
                    return true;
                }
                else{
                    snum+=deg_maxv[i]-num[i];
                    num[i]=deg_maxv[i];
                    
                }
                --i;

            }
            return false;
        }
        else
        {
            while (i>=0)
            {
                if (num[i]>=0)
                if(snum<degmax && num[i]<deg_maxv[i])
                {
                    ++num[i];
                    ++snum;
                    if (snum>=degmin)
                        return true;
                    else
                        return num_next(num,snum,deg_maxv,deg_minv,degmax,degmin);
                }
                else{
                    snum+=deg_minv[i]-num[i];
                    num[i]=deg_minv[i];
                }
                --i;
                
            }
            return false;
        }

        
    }
    

    bool sos_support_check(atomic_polynomial<polynomial::monomial,long> check,vector<monomial> &point)
    {   
            int s=0;
            int N=(point.size()*(point.size()-1))/2;
            for(auto i=point.begin();i!=point.end();i++)
                for(auto j=i;j!=point.end();j++){
                    s++;
                    if (s % 10000==0)
                        printf("check...%0.2f%%\r",s*100.0/N);
                    auto it=check.find((*i)*(*j));
                    //std::cout<<v1.str()<<" "<<(it!=check.end());//<<std::endl;
                    //auto it=check.find(v1);d
                    if (it!=check.end())
                    {
                        (it->second)=false;
                    //    std::cout<<" "<<check[v1]<<std::endl;
                    }
                }
            for(auto it=check.begin();it!=check.end();++it)
                if (it->second)
                {
                    //std::cout<<"error "<<it->first.str()<<std::endl;
                    std::cout<<"check...done. :False.\n";
                    return false;
                }
            std::cout<<"check...done. :Success.\n";
            return true;
    }
    inline bool is_num_char(char c)
    {
        return (c>='0' && c<='9');
    }
    inline bool is_abc_char(char c)
    {
        return ((c>='a' && c<='z')||(c>='A' && c<='Z'));
    }
    inline bool is_op_char(char c)
    {
        return (c=='*' || c=='+' || c=='^' || c=='-' || c=='(' || c==')');
    }
    inline bool is_poly_char_using(char c)
    {
        return (is_num_char(c) || is_abc_char(c) || is_op_char(c));
    }
    inline bool is_blank_char(char c)
    {
        return (c==' ' || c=='\n');
    }
    inline bool is_poly_char(char c)
    {
        return (is_blank_char(c) || is_poly_char_using(c));
    }
    inline atomic_polynomial<monomial,long> str_to_var(string s,unordered_map<std::string,int> &varmap,vector<std::string> &varname)
    {
        auto b=varmap.find(s);
        if (b==varmap.end())
        {
            varname.push_back(s);
            varmap[s]=varname.size()-1;
            return atomic_polynomial<monomial,long>(monomial(varname.size()-1),1);
        }
        else
        {
            return atomic_polynomial<monomial,long>(monomial(b->second),1);
        }
    }
    inline char read_op(fstream &fin)
    {
        char c;
        while (fin>>c)
            if (!is_blank_char(c))
            {
                if (is_op_char(c))
                    return c;
                else
                    if (is_poly_char(c))
                        throw 2;
                    else
                        throw 1;
            }
        return ' ';
    }
    void read_poly_a(fstream &fin,char & op,vector<char> &cst)
    {
        char c;
        int status=0;
        while (fin>>c)
        {
            if (is_blank_char(c))
                continue;
            if (is_num_char(c))
            {
                if (status==0)
                    status=1;
                cst.push_back(c);
            }
            else
            if (is_abc_char(c))
            {
                if (status==1)
                    throw 2;
                if (status==0)
                    status=2;
                cst.push_back(c);
            }
            else if (is_op_char(c))
            {
                op=c;
                if (status==0)
                {
                    if (c=='(')
                    {
                        return void();
                    }
                    else
                        throw 2;
                }
                return void();
            }
            else{
                throw 1;
            }
            
        }
        if (status==0)
            throw 2;
        op=' ';
        return void();
    }
    // typename atomic_polynomial<monomial,long>::monomial_pair  read_monomial_pair(fstream &fin,char &op,unordered_map<std::string,int> &varmap,vector<std::string> &varname)
    // {
    //     vector<var_pair> vps;
    //     vector<char> cst;

    // }
    atomic_polynomial<monomial,long>  read_poly_a(fstream &fin,char &op,unordered_map<std::string,int> &varmap,vector<std::string> &varname)
    {
        stringstream ss;
        char c;
        long a;
        int status=0;
        while (fin>>c)
        {
            if (is_blank_char(c))
                continue;
            if (is_num_char(c))
            {
                if (status==0)
                    status=1;
                ss<<c;
            }
            else
            if (is_abc_char(c))
            {
                if (status==1)
                    throw 2;
                if (status==0)
                    status=2;
                ss<<c;
            }
            else if (is_op_char(c))
            {
                op=c;
                if (status==0)
                {
                    if (c=='(')
                    {
                        atomic_polynomial<monomial,long> p=read_polynomial(fin,varmap,varname);
                        op=read_op(fin);
                        return p;
                    }
                    else if(c=='+')
                    {
                        op='*';
                        return atomic_polynomial<monomial,long>(1);
                    }
                    else if(c=='-')
                    {
                        op='*';
                        return atomic_polynomial<monomial,long>(-1);
                    }
                    else
                        throw 2;
                }
                if (status==1)
                {
                    ss>>a;
                    return atomic_polynomial<monomial,long>(a);
                }
                else
                {
                    return str_to_var(ss.str(),varmap,varname);
                }
            }
            else{
                throw 1;
            }
            
        }
        if (status==0)
            throw 2;
        op=' ';
        if (status==1)
        {
            ss>>a;
            return atomic_polynomial<monomial,long>(a);
        }
        else
        {
            return str_to_var(ss.str(),varmap,varname);
        }
    }

    inline atomic_polynomial<monomial,long> op_poly(char op,atomic_polynomial<monomial,long> &p1,atomic_polynomial<monomial,long> &p2)
    {
        switch(op)
        {
            case '+':
                return p1+p2;
            case '-':
                return p1-p2;
            case '*':
                return p1*p2;
            default :
                throw 3;
        }
    }
    atomic_polynomial<polynomial::monomial,long> read_polynomial(fstream &fin,unordered_map<std::string,int> &varmap,vector<std::string> &varname)
    {
        atomic_polynomial<polynomial::monomial,long>  p2,p1,p3;
        char c;
        char o1='+';
        char o2,o3;
        long a;
        p2=read_poly_a(fin,o2,varmap,varname);
        while (true)
        {
            if (o2==' ' || o2==')')
            {
                return op_poly(o1,p1,p2);
            }
            else if (o2=='^')
            {
                fin>>a;
                p2=p2.power(a);
                o2=read_op(fin);
            }
            else if (o2=='+' || o1=='*' || o2=='-')
            {
                p1=op_poly(o1,p1,p2);
                o1=o2;
                p2=read_poly_a(fin,o2,varmap,varname);
            }
            else
            {
                p3=read_poly_a(fin,o3,varmap,varname);
                while (o3=='^')
                {
                    fin>>a;
                    p3=p3.power(a);
                    o3=read_op(fin);
                }
                p2=op_poly(o2,p2,p3);
                o2=o3;
            }
        }
    }
    atomic_polynomial<polynomial::monomial,long> read_polynomial(std::string &filename,std::unordered_map<std::string,int> &varmap,std::vector<std::string> &varname)
    {
        varmap.clear();
        varname.clear();
        std::fstream fin(filename,std::fstream::in);
        atomic_polynomial<polynomial::monomial,long> p=read_polynomial(fin,varmap,varname);
        fin.close();
        return p;
    }
    std::string  monomial_str(const monomial &m,const std::vector<std::string> &varname)
    {
        std::stringstream ss;
        if (m.empty())
        return "1";
        for(std::size_t i=0;i<m.size();++i)
        {
        if (i!=0)
            ss<<"*";
        ss<<varname[m.at(i)->first];
        if (m.at(i)->second!=1)
            ss<<"^"<<(m.at(i)->second);
        }
        return ss.str();
    }
    
    // std::string polynomial_str(const atomic_polynomial<polynomial::monomial,long> &p,const std::vector<std::string> &varname)
    // {
    //     if (p.size()==0)
    //     return "0";
    //     std::stringstream ss;
    //     for(auto i=p.begin();i!=p.end();++i)
    //     {
    //     if (i!=p.begin() && i->second>=0)
    //         ss<<"+";
    //     if (!monomial_empty(i->first)){
    //         if (i->second==-1)
    //         ss<<"-";
    //         else
    //         if (i->second!=1 )
    //         ss<< i->second<<"*";
    //         ss<<monomial_str(i->first,varname);
    //     }
    //     else
    //         ss<<i->second;
    //     }
    //     return ss.str();
    // }



    void Cholesky(double* M,std::size_t size)
    {
        for (std::size_t k=0;k<size;++k)
        {  
            M[k*size+k]=sqrt(M[k*size+k]);
            for (std::size_t i=k+1;i<size;++i)
                M[i*size+k]/=M[k*size+k];
            for (std::size_t j=k+1;j<size;++j)
                for (std::size_t i=j;i<size;++i)
                    M[i*size+j]-=M[i*size+k]*M[j*size+k];
        }
    }
    std::vector<polynomial::atomic_polynomial<polynomial::monomial,double>> sosd(std::vector<polynomial::monomial> &points,
                                                std::vector<std::vector<polynomial::var>> &L,std::vector<std::vector<double>> &M)
    {
        std::vector<polynomial::monomial>  monos; 
        std::vector<double> coeff;
        std::size_t size;
        std::vector<polynomial::atomic_polynomial<polynomial::monomial,double>>  sosd;
        for (std::size_t i=0;i<L.size();++i)
        {   
            size=L[i].size();
            monos.resize(size);
            coeff.resize(size);
            for (std::size_t j=0;j<size;++j)
                monos[j]=points[L[i][j]];
            Cholesky(M[i].data(),size);
            for (std::size_t k1=0;k1<size;++k1)
            {
                for (std::size_t k2=k1;k2<size;++k2)
                    coeff[k2-k1]=M[i][k2*size+k1];
                sosd.push_back(polynomial::atomic_polynomial<polynomial::monomial,double>(monos.data()+k1,coeff.data(),size-k1));
            }
        }
        return sosd;

    }
}

