#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <fusion.h>
#include "polynomial/polynomial.hpp"
#include "sos.hpp"
using namespace std;
using namespace polynomial;
using namespace mosek::fusion;
using namespace monty;
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
    
    vector<monomial>  sos_support_mosek(atomic_polynomial<polynomial::monomial,long>&p,std::size_t polydim)
    {
        
        if (p.empty())
        {
            return vector<monomial>();
        }
        //std::cout<<"poly length:"<<p.size()<<"\n";
        int m=polydim;
        int n=p.size();
        std::cout<<"poly size:"<<n<<" poly dim:"<<m<<"\n";
        auto A1 = new_array_ptr<double, 1>(n*(m+1));
        for(int i=0;i<n*(m+1);++i)
            (*A1)[i]=0;
        int n1=0;
        int degmax=-1;
        int degmin=-1;
        double dd;
        vector<int> deg_maxv(m);
        for (auto &i:deg_maxv)
            i=-1;
        vector<int> deg_minv(m);
        for (auto &i:deg_maxv)
            i=-1;
        for(auto &i:p){
            //std::cout<<i.first.str()<<" "<<i.first.deg()<<"\n";
            if ((int)(i.first.deg()/2)>degmax)
                degmax=i.first.deg()/2;
            if ((int)(i.first.deg()/2)<degmin || degmin<0)
                degmin=i.first.deg()/2;
            //std::cout<<((i.first.deg()/2))<<" "<<(degmax)<<" "<<degmin<<"\n";
            for(auto &j:i.first){
                dd=double(j.second)/2;
                (*A1)[n1*(m+1)+j.first]=dd;
                if (dd>deg_maxv[j.first])
                    deg_maxv[j.first]=dd;
                if (dd<deg_minv[j.first] || deg_minv[j.first]<0)
                    deg_minv[j.first]=dd;
            }
            (*A1)[n1*(m+1)+m]=-1; 
            ++n1;
        }
        auto A=Matrix::dense(n,m+1,A1);
        //std::cout<<p.str()<<"\n";
        // for (auto &i:deg_maxv)
        //     std::cout<<i<<" ";
        // std::cout<<degmax<<std::endl;
        // for (auto &i:deg_minv)
        //     std::cout<<i<<" ";
        //std::cout<<degmin<<std::endl;
        if (degmax==-1)
        {
            return vector<monomial>();
        }
        vector<int> num(m);//={0};
        //vector<int> num1(m);
        //num[m]=-1;
        //num[m-1]=-1;

        int snum=0;
        //num[snum++]=0;num[snum++]=0;num[snum++]=0;num[snum++]=0;num[snum++]=0;
        //num[snum++]=1;num[snum++]=1;num[snum++]=0;num[snum++]=0;num[snum++]=1;
        
        bool b=num_init(num,snum,deg_maxv,deg_minv,degmax,degmin);
        //auto C=new_array_ptr<double, 1>(m+1);
        auto Co=new_array_ptr<double, 1>(m+1);
        //(*C)[m]=0;
        (*Co)[m]=-1;
        var_pair* mono=new var_pair[m];
        for(int i=0;i<m;++i)
        {
            mono[i].first=i;
            //(*C)[i]=0;
            (*Co)[i]=0;
            //num1[i]=0;
        }
        vector<monomial> monos;
        int num_mono=0;
        auto M=new Model();//auto _M = finally([&]() { M->dispose(); });
        auto X=M->variable( m+1, Domain::unbounded() );//new_array_ptr<int,1>({m+1,1})
        M->constraint("c",Expr::mul(A,X),Domain::lessThan(0,n));
        auto c1=M->constraint("c1",Expr::dot(Co,X),Domain::greaterThan(0.0));
        //auto y=Expr::dot(C,X);
        cout<<"Initially poly...\n";
        while (b){
            ++num_mono;
            //if ((num_mono % 100)==0)
            //    std::cout<<"Initially "<<num_mono<<" monomials.\r";
            for(n1=0;n1<m;++n1)
                if (num[n1]>=0)
                {
                    //(*C)[n1]=num[n1]-num1[n1];
                    (*Co)[n1]=num[n1];
                    mono[n1].second=num[n1];
                }
                else{
                    //(*C)[n1]=0-num1[n1];
                    (*Co)[n1]=0;
                    mono[n1].second=0;
                }
            //num1=num;
            c1->update(Expr::dot(Co,X));
            M->objective("obj", ObjectiveSense::Maximize, Expr::dot(Co,X));
            M->solve();

            if (M->getPrimalSolutionStatus()==SolutionStatus::Optimal && M->primalObjValue()>=-1e-5)
            {
                monos.push_back(monomial(mono,m));
                //std::cout<<monos.back().str()<<"\n";
            }
            else
                if (M->getPrimalSolutionStatus()==SolutionStatus::Undefined)
                {
                    monos.push_back(monomial(mono,m));
                    //std::cout<<monos.back().str()<<"\n";
                }
            b=num_next(num,snum,deg_maxv,deg_minv,degmax,degmin);
        } 
        std::cout<<"Initially "<<num_mono<<" monomials.\n";
        std::cout<<"Keeping "<<monos.size()<<" monomials.\n";
        M->dispose();
        delete [] mono;
        return monos;
        
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
    std::string polynomial_str(const atomic_polynomial<polynomial::monomial,long> &p,const std::vector<std::string> &varname)
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
            ss<<monomial_str(i->first,varname);
        }
        else
            ss<<i->second;
        }
        return ss.str();
    }

}
// int main(int argc, char const *argv[])
// {
//     /* code */
//     return 0;
// }


