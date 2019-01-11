#include <iostream>
#include <vector>
#include <mosek/fusion.h>
#include "polynomial/polynomial.h"
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
    
    vector<monomial>  sos_support_mosek(atomic_polynomial<int> &p)
    {
        
        if (p.empty())
        {
            return vector<monomial>();
        }
        //std::cout<<"poly length:"<<p.size()<<"\n";
        int m=p.dim();
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
        std::cout<<degmin<<std::endl;
        if (degmax==-1)
        {
            return vector<monomial>();
        }
        vector<int> num(m);//={0};
        vector<int> num1(m);
        //num[m]=-1;
        //num[m-1]=-1;

        int snum=0;
        //num[snum++]=0;num[snum++]=0;num[snum++]=0;num[snum++]=0;num[snum++]=0;
        //num[snum++]=1;num[snum++]=1;num[snum++]=0;num[snum++]=0;num[snum++]=1;
        
        bool b=num_init(num,snum,deg_maxv,deg_minv,degmax,degmin);
        auto C=new_array_ptr<double, 1>(m+1);
        auto Co=new_array_ptr<double, 1>(m+1);
        (*C)[m]=0;
        (*Co)[m]=-1;
        var_pair mono[m];
        for(int i=0;i<m;++i)
        {
            mono[i].first=i;
            (*C)[i]=0;
            (*Co)[i]=0;
            num1[i]=0;
        }
        vector<monomial> monos;
        int num_mono=0;
        auto M=new Model();//auto _M = finally([&]() { M->dispose(); });
        auto X=M->variable( m+1, Domain::unbounded() );//new_array_ptr<int,1>({m+1,1})
        M->constraint("c",Expr::mul(A,X),Domain::lessThan(0,n));
        auto c1=M->constraint("c1",Expr::dot(Co,X),Domain::greaterThan(0.0));
        //auto y=Expr::dot(C,X);
        cout<<"Initially...\n";
        while (b){
            ++num_mono;
            //if ((num_mono % 100)==0)
            //    std::cout<<"Initially "<<num_mono<<" monomials.\r";
            for(n1=0;n1<m;++n1)
                if (num[n1]>=0)
                {
                    (*C)[n1]=num[n1]-num1[n1];
                    (*Co)[n1]=num[n1];
                    mono[n1].second=num[n1];
                }
                else{
                    (*C)[n1]=0-num1[n1];
                    (*Co)[n1]=0;
                    mono[n1].second=0;
                }
            num1=num;
            c1->add(Expr::dot(C,X));
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
        return monos;
        
    }
}
// int main(int argc, char const *argv[])
// {
//     /* code */
//     return 0;
// }


