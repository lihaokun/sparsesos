#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include "fusion.h"
#include "polynomial/polynomial.hpp"
#include "sos_mosek.hpp"
#include "sos.hpp"
#ifndef  mosek_v
    #define mosek_v 9
#endif
using namespace std;
using namespace polynomial;
using namespace mosek::fusion;
using namespace monty;

namespace sparsesos{
    vector<monomial>  sos_support_mosek(atomic_polynomial<polynomial::monomial,long>&p,std::size_t polydim,int numthreads)
    {
        
        if (p.empty())
        {
            return vector<monomial>();
        }
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
        for (auto &i:deg_minv)
            i=-1;
        for(auto &i:p){
            //std::cout<<i.first.str()<<" "<<i.first.deg()<<"\n";
            if ((int)(i.first.deg()/2)>degmax)
                degmax=i.first.deg()/2;
            if ((int)(i.first.deg()/2)<degmin || degmin<0)
                degmin=i.first.deg()/2;
            //std::cout<<((i.first.deg()/2))<<" "<<(degmax)<<" "<<degmin<<"\n";
            // for(auto &j:i.first){
            //     dd=double(j.second)/2;
            //     (*A1)[n1*(m+1)+j.first]=dd;
            //     if (dd>deg_maxv[j.first])
            //         deg_maxv[j.first]=dd;
            //     if (dd<deg_minv[j.first] || deg_minv[j.first]<0)
            //         deg_minv[j.first]=dd;
            // }
            auto tmp_ptr=i.first.begin();
            for (size_t j=0;j<m;++j)
            {
                if (tmp_ptr!=i.first.end() && tmp_ptr->first==j)
                {
                    dd=double((tmp_ptr++)->second)/2;
                    (*A1)[n1*(m+1)+j]=dd;
                }
                else
                {
                    dd=0;
                }
                if (dd>deg_maxv[j])
                    deg_maxv[j]=dd;
                if (dd<deg_minv[j] || deg_minv[j]<0)
                    deg_minv[j]=dd;
            }
            // for (auto &j:deg_minv)
            //     std::cout<< j <<" ";
            // std::cout<<std::endl;
            // for (auto &j:deg_maxv)
            //     std::cout<< j <<" ";
            // std::cout<<std::endl;
            
            (*A1)[n1*(m+1)+m]=-1; 
            ++n1;
        }
        auto A=Matrix::dense(n,m+1,A1);
        // for (auto &i:deg_maxv)
        //     std::cout<<i<<" ";
        // std::cout<<degmax<<std::endl;
        // for (auto &i:deg_minv)
        //     std::cout<<i<<" ";
        // std::cout<<degmin<<std::endl;
        if (degmax==-1)
        {
            return vector<monomial>();
        }
        vector<int> num(m);//,0);//={0};
        //;
        //num[m]=-1;
        //num[m-1]=-1;

        int snum=0;
        //num[snum++]=0;num[snum++]=0;num[snum++]=0;num[snum++]=0;num[snum++]=0;
        //num[snum++]=1;num[snum++]=1;num[snum++]=0;num[snum++]=0;num[snum++]=1;
        
        bool b=num_init(num,snum,deg_maxv,deg_minv,degmax,degmin);
        #if mosek_v==8
            auto C=new_array_ptr<double, 1>(m+1);
            (*C)[m]=0;
            vector<int> num1(m,0);
        #endif
        auto Co=new_array_ptr<double, 1>(m+1);
        (*Co)[m]=-1;
        var_pair* mono=new var_pair[m];
        for(int i=0;i<m;++i)
        {
            mono[i].first=i;
            (*Co)[i]=0;
            #if mosek_v==8
                (*C)[i]=0;
            #endif
        }
        vector<monomial> monos;
        int num_mono=0;
        auto M=new Model();//auto _M = finally([&]() { M->dispose(); });
        if (numthreads)
            M->setSolverParam("numThreads",numthreads);
        auto X=M->variable( m+1, Domain::unbounded() );//new_array_ptr<int,1>({m+1,1})
        M->constraint("c",Expr::mul(A,X),Domain::lessThan(0,n));
        auto c1=M->constraint("c1",Expr::dot(Co,X),Domain::greaterThan(0.0));
        //auto y=Expr::dot(C,X);
        cout<<"Initially poly...\n";
        while (b){
            // std::cout<<"num:";
            // for  (auto &i:num)
            //     std::cout<<i<<" ";
            // std::cout<<std::endl;
            ++num_mono;
            //if ((num_mono % 100)==0)
            //    std::cout<<"Initially "<<num_mono<<" monomials.\r";
            for(n1=0;n1<m;++n1)
                if (num[n1]>=0)
                {
                    #if mosek_v==8
                        (*C)[n1]=num[n1]-num1[n1];
                    #endif
                    (*Co)[n1]=num[n1];
                    mono[n1].second=num[n1];
                }
                else{
                    #if mosek_v==8
                        (*C)[n1]=0-num1[n1];
                    #endif
                    (*Co)[n1]=0;
                    mono[n1].second=0;
                }
            #if mosek_v==8
                num1=num;
                c1->add(Expr::dot(C,X));
            #else
                c1->update(Expr::dot(Co,X));
            #endif
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
    bool   SOS_solver_mosek(polynomial::atomic_polynomial<polynomial::monomial,long>&p,std::vector<polynomial::monomial> &points,std::vector<std::vector<polynomial::var>> &L,
                                                        std::vector<std::vector<double>> & ans,int numthreads)
    {
        std::size_t size=L.size();
        std::size_t tmp_size;
        std::vector<monty::rc_ptr<mosek::fusion::Variable>> X(size);
        std::map<polynomial::monomial,std::size_t> dct;
        auto tmp_it=dct.begin();
        std::vector<monty::rc_ptr<mosek::fusion::Expression>> exp;
        std::vector<double> value; 
        polynomial::monomial mono;
        auto M=new Model();
        if (numthreads)
            M->setSolverParam("numThreads",numthreads);
        M->setLogHandler([  ](const std::string & msg) { std::cout << msg << std::flush; } );
        //auto exp_obj=Expr::zeros();
        for(std::size_t l=0;l<size;++l)
            {
                tmp_size=L[l].size();
                X[l]=M->variable("X"+std::to_string(l),Domain::inPSDCone(tmp_size));
                for(std::size_t i=0;i!=tmp_size;++i )
                    for(std::size_t j=i;j!=tmp_size;++j)
                    {
                        mono=points[L[l][i]]*points[L[l][j]];
                        tmp_it=dct.find(mono);
                        if (tmp_it==dct.end())
                        {
                            if (i==j)
                                exp.push_back(X[l]->index(i,j)->asExpr());
                            else
                                exp.push_back(Expr::add(X[l]->index(i,j),X[l]->index(j,i)));
                            dct[mono]=exp.size()-1;
                            value.push_back(p[mono]);
                        }
                        else
                            if (i==j)
                                exp[tmp_it->second]=Expr::add(exp[tmp_it->second],X[l]->index(i,j));
                            else
                                exp[tmp_it->second]=Expr::add(exp[tmp_it->second],
                                                                Expr::add(X[l]->index(i,j),X[l]->index(j,i)));
                    }
            }
        for(std::size_t i=0;i!=exp.size();i++)
        {
            M->constraint(exp[i],Domain::equalsTo(value[i]));
        }
        M->objective("obj",ObjectiveSense::Minimize,Expr::zeros(1));
        M->solve();
        //std::vector<std::vector<double>> ans(size);
        bool tmp_b=(M->getPrimalSolutionStatus()==SolutionStatus::Optimal);
        if (tmp_b)
        {
            ans.clear();
            ans.resize(size);
            for  (int i=0;i<size;++i)
            {
                tmp_size=L[i].size();
                ans[i].resize(tmp_size*tmp_size);
                auto tmp_x=X[i]->level();
                for (int k1=0;k1<tmp_size*tmp_size;++k1)
                    ans[i][k1]=(*tmp_x)[k1];
            }
            bool tmp_b=(M->getPrimalSolutionStatus()==SolutionStatus::Optimal);
        }
        M->dispose();
        return tmp_b;
    }
}