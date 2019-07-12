#include "sparsesos.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <list>
#include <unordered_set>
#include <sstream> 

namespace sparsesos{
    int read_data(std::string &str_file,polynomial::atomic_polynomial<polynomial::monomial,long>&p)
    {
        std::fstream fin(str_file,std::fstream::in);
        long m=0;
        int n=0;
        if (!(fin>>m>>n))
        {
            std::cout<<"read "<<str_file<<" failed.";
            exit(0);
        }
        std::cout<<m<<" "<<n<<std::endl;
        long a=1;
        //std::vector<polynomial::monomial_pair<int>> v1;
        std::pair<polynomial::monomial,long> *v1=new std::pair<polynomial::monomial,long>[m]; 
        unsigned* mono=new unsigned[n];
        polynomial::var* v=new polynomial::var[n];
        //std::map<polynomial::monomial,int> dct;
        for(int i=0;i<n;++i)
            v[i]=i;        
        for(long i=0;i<m;i++)
        {
            //std::cout<<i<<"/"<<m;//<<std::endl;
            if (i%1000==0)
                printf("read...%0.2f%%\r",i*100.0/m);
            for(int j=0;j<n;j++)
                //fin>>a;
                if (!(fin>>mono[j]))
                {
                    std::cout<<"read "<<str_file<<" failed.";
                    exit(0); 
                }
                //mono[j]=1;
            if (!(fin>>a))
            {
                    std::cout<<"read "<<str_file<<" failed.";
                    exit(0); 
            }
            //dct[mono];
            //std::cout<<" "<<a<<" "<<n<<std::endl;
            //dct.insert(std::make_pair(polynomial::monomial(v,mono,n),std::move(a)));
            v1[i]=std::make_pair(polynomial::monomial(v,mono,n),std::move(a));
            //std::cout<<a<<std::endl;
            //v1.push_back(mono);
        }
        fin.close();
        p=polynomial::atomic_polynomial<polynomial::monomial,long>(v1,m,true,false,true);
        std::cout<<"read...done.          \n";
        delete [] mono;
        delete [] v;
        return n;    
    }
    void read_point_data(std::string &str_file,std::vector<polynomial::monomial> &point)
    {
        std::fstream fin(str_file,std::fstream::in);
        long m=0;
        int n=0;
        if (!(fin>>m>>n))
        {
            std::cout<<"read "<<str_file<<" failed.";
            exit(0);
        }
        std::cout<<m<<" "<<n<<std::endl;
        int a=1;
        //std::vector<polynomial::monomial_pair<int>> v1;
        //polynomial::monomial_pair<int> *v1=new polynomial::monomial_pair<int>[m]; 
        point.resize(m);
        unsigned* mono=new unsigned[n];
        polynomial::var* v=new polynomial::var[n];
        std::map<polynomial::monomial,int> dct;
        for(int i=0;i<n;++i)
            v[i]=i;        
        for(long i=0;i<m;i++)
        {
            //std::cout<<i<<"/"<<m;//<<std::endl;
            if (i%1000==0)
                printf("read...%0.2f%%\r",i*100.0/m);
            for(int j=0;j<n;j++)
                //fin>>a;
                if (!(fin>>mono[j]))
                {
                    std::cout<<"read "<<str_file<<" failed.";
                    exit(0); 
                }
                //mono[j]=1;
            //fin>>a;
            //dct[mono];
            //std::cout<<" "<<a<<" "<<n<<std::endl;
            //dct.insert(std::make_pair(polynomial::monomial(v,mono,n),std::move(a)));
            point[i]=polynomial::monomial(v,mono,n);
        }
        fin.close();
        delete [] mono;
        delete [] v;
        std::cout<<"read...done.          \n";
        /*
        for(auto &i:point)
        {
            std::cout<<i.str()<<std::endl;
        }
        */
    }
  


    inline bool monomial_is_even(const polynomial::monomial &v)
    {
        for(auto& i:v)
            if (i.second %2 !=0)
                return false;
        return true;
    }
//     bool get_half(polynomial::atomic_polynomial<polynomial::monomial,long>&p,std::vector<polynomial::monomial> &point,bool is_check)
//     {
//         if (p.empty())
//             return true;
//         int n=p.dim();
//         std::cout<<n<<std::endl;
//         polynomial::var_pair v1[n];
//         polynomial::atomic_polynomial<polynomial::monomial,long>check;
// //        monomial_map check;
//         for (auto it=p.begin();it!=p.end();++it)
//         {
//             if (monomial_is_even(it->first))
//             {
//                 for (std::size_t i=0;i<it->first.size();i++){
//                     v1[i]=*((it->first).at(i));
//                     v1[i].second/=2;
//                 }
//                 point.push_back(polynomial::monomial(v1,it->first.size()));
//             //    std::cout<<true;
//             }
//             //std::cout<<std::endl;
// //            if (is_check)
// //                check[it->first]=true;
//         }
//         //for(auto& i:point)
//         //{
//         //    for(auto& j:i)
//         //        std::cout<<j<<" ";
//         //    std::cout<<std::endl;
//         //}
//         auto it=check.begin();
//         if (is_check)
//         {
//             check=p;
//             int s=0;
//             int N=(point.size()*(point.size()-1))/2;
//             for(auto i=point.begin();i!=point.end();i++)
//                 for(auto j=i;j!=point.end();j++){
//                     s++;
//                     if (s % 10000==0)
//                         printf("check...%0.2f%%\r",s*100.0/N);
//                     auto it=check.find((*i)*(*j));
//                     //std::cout<<v1.str()<<" "<<(it!=check.end());//<<std::endl;
//                     //auto it=check.find(v1);d
//                     if (it!=check.end())
//                     {
//                         (it->second)=false;
//                     //    std::cout<<" "<<check[v1]<<std::endl;
//                     }
//                 }
//             for(auto it=check.begin();it!=check.end();++it)
//                 if (it->second)
//                 {
//                     std::cout<<"error "<<it->first.str()<<std::endl;
//                     std::cout<<"check...done. :False.\n";
//                     return false;
//                 }
//             std::cout<<"check...done. :Success.\n";
//         }
//         return true;
//     }
    void com_connect_dfs
    (polynomial::var point,polynomial::atomic_polynomial<polynomial::monomial,long>&p,std::vector<polynomial::monomial> &points,std::list<polynomial::var> &pot,std::vector<polynomial::var> &v,int & sn)
    {
        int n=v.size();
        //polynomial::monomial_pair it;
        polynomial::monomial mono;
        auto i=pot.begin();
        while (i!=pot.end()){
            mono=points[*i]*points[point];
            if (monomial_is_even(mono) || p.find(mono)!=p.end())
            {
                ++sn;
                //printf("%d\n",sn);
                if (sn%10==0)
                    printf("com_connect...%0.2f%%\r",sn*100.0/points.size());
                v.push_back(*i);
                i=pot.erase(i);
            }
            else
                ++i;
        }
        int m=v.size();
        for(;n<m;++n)
            com_connect_dfs(v[n],p,points,pot,v,sn);
    }
    void com_connect
    (polynomial::atomic_polynomial<polynomial::monomial,long>&p,std::vector<polynomial::monomial> &points,std::vector<std::vector<polynomial::var>> &L)
    {
        std::list<polynomial::var> pot;
        //std::list<polynomial::var> lst;
        
        polynomial::var point;
        printf("com_connect...   \r");
        polynomial::monomial mono;
        int sn=0;
        for (polynomial::var i=0;i<points.size();++i)
            pot.push_back(i);// pot
        while(!pot.empty()){
            point=pot.front();
            pot.pop_front();
            L.push_back(std::vector<polynomial::var>());
            L.back().push_back(point);
            ++sn;
            com_connect_dfs(point,p,points,pot,L.back(),sn);
            
        }
        printf("com_connect...done.   \n");
        std::sort(L.begin(),L.end(),[](const std::vector<polynomial::var> &v1,
                                    const std::vector<polynomial::var> &v2){return v1.size()>v2.size();});
    }
    void get_graph
    (polynomial::atomic_polynomial<polynomial::monomial,long>&p,std::vector<polynomial::monomial> &points,std::vector<polynomial::var> &v,std::vector<std::unordered_set<polynomial::var>> &G)
    {
        polynomial::monomial mono; 
        for(auto i=v.begin();i!=v.end();++i)
            for(auto j=i;j!=v.end();++j){
                mono=points[*i]*points[*j];
                if (monomial_is_even(mono) || p.find(mono)!=p.end()){
                    G[*i].insert(*j);
                    if (i!=j)
                        G[*j].insert(*i);
                
                }
                //if (*i==110)
                //    std::cout<<*j;            
            }
        //std::cout<<G[110].size()<<std::endl;
    }

    bool MCS_M_CHECK
    (std::list<polynomial::var>::iterator i,std::list<polynomial::var> &alpha,std::vector<int> &number,std::vector<std::unordered_set<polynomial::var>> &G)
    {
        bool B;
        for(auto j=alpha.begin();j!=i;++j){
            B=true;   
            for(auto &k:G[*i])
                if (number[k]>=number[*i] && G[*j].find(k)==G[*j].end())
                {
                    //std::cout<<(*i)<<*j<<k<<"error\n ";
                    B=false;
                    break;
                }
            if (B)
                return false;
        }
        return true;
    }
    void MCS_M
    (polynomial::atomic_polynomial<polynomial::monomial,long>&p,std::vector<polynomial::monomial> &points,std::vector<std::vector<polynomial::var>> &V,std::vector<std::vector<polynomial::var>> & L)
    {
        std::vector<std::unordered_set<polynomial::var>> G(points.size());
        std::vector<int> w(points.size());
        std::vector<int> number(points.size());
        std::list<polynomial::var> alpha;
        std::unordered_set<polynomial::var> remain;
        for(int i=0;i<points.size();++i){
            w[i]=0;
            number[i]=0;
        }
        polynomial::var point;
        int deep;
        //std::unordered_set<polynomial::var> S;
        std::vector<int> S1(points.size());
        std::vector<bool> S2(points.size());
        
        int n=0;int n1=0;
        bool B=true;
        for(auto & v:V)
        {
            ++n;
            get_graph(p,points,v,G);
            //std::cout<<"block "<<n<<"....";
            alpha.clear();
            remain.clear(); 
            for(auto &i:v)
                remain.insert(i);
            for(polynomial::var i=v.size();i>0;--i){
                if (i%10==0)
                    printf("block %d..%0.2f%%\r",n,(v.size()-i)*100.0/v.size());
                point = *(remain.begin());
                for(auto &j:remain)
                    if (w[point]<w[j])
                        point=j;
                remain.erase(point);
                
                for(auto &j:remain)
                    if (G[point].find(j)!=G[point].end()){
                        S1[j]=w[j];
                        S2[j]=true;
                    }
                    else{
                        S1[j]=-1;
                        S2[j]=false;
                    }
                B=true;
                while (B){
                    B=false;
                    for(auto &j:remain)
                        if (S1[j]>=0)
                            for(auto &k:G[j])
                                if (remain.find(k)!=remain.end() && (S1[k]==-1 || (!S2[k] && S1[j]<S1[k]))){
                                    B=true;
                                    if (S1[j]<w[k]){
                                        S2[k]=true;
                                        S1[k]=w[k];
                                    }
                                    else
                                        S1[k]=S1[j];
                                    //S1[k]=std::max(S1[j],w[k]);
                                }
                }
                for(auto &j:remain)
                    if (S2[j]){
                        ++w[j];
                        G[j].insert(point);
                        G[point].insert(j);
                    }
                
            
                number[point]=i;
                alpha.push_front(point);
            }
            std::cout<<"done.                        \r";
            n1=L.size();
            for(auto i=alpha.begin();i!=alpha.end();++i)
            {
                if (MCS_M_CHECK(i,alpha,number,G)){
                    L.push_back(std::vector<polynomial::var>());
                    for(auto &j:G[*i])
                        if (number[j]>=number[*i])
                            L.back().push_back(j);
                    if (L.back().size()==0)
                        std::cout<<*i<<" "<<points[*i].str()<<" "<<(points[*i]*points[*i]).str()<<" "<< monomial_is_even(points[*i]*points[*i])<<" error \n";
                    //std::cout<<L.back().size()<<":";
                    /*B=true;
                    for(auto &j:L.back())
                        for(auto &k:L.back())
                            if (G[j].find(k)==G[j].end()){
                                B=false;
                                break;
                            }
                    std::cout<<B<<" ";*/
                }

            }
            //std::cout<<"\n";
            
        }
        std::cout<<"MCS_M ...done.\n";
    }
    void output
    (std::string &str_file,polynomial::atomic_polynomial<polynomial::monomial,long>&p,std::vector<polynomial::monomial> &points,std::vector<std::vector<polynomial::var>> &L,bool min_bool)
    {
        std::ostringstream out;
        std::map<polynomial::monomial,std::size_t> dct;
        std::vector<int> coeff;
        polynomial::monomial mono;
        polynomial::monomial mono_zero;

        auto it=dct.begin();
        
        std::size_t size=L.size();
        while (size>0 && L[size-1].size()==1)
            --size;
        std::size_t n=0;
        for(std::size_t l=0;l<size;++l)
            for(std::size_t i=0;i!=L[l].size();++i )
                for(std::size_t j=i;j!=L[l].size();++j){
                    mono=points[L[l][i]]*points[L[l][j]];
                    if(!min_bool || mono!=mono_zero){
                        it=dct.find(mono);
                        if (it==dct.end())
                        {    
                            ++n;
                            dct[mono]=n;
                            coeff.push_back(p[mono]);
                            //std::cout<<mono.str()<<std::endl;
                    
                            //auto it=p.find(mono);
                            //if (it!=p.end())
                            //    std::cout<<mono.str()<<" "<<it->first.str()<<std::endl;
                            out<<n<<" "<<l+1<<" "<<i+1<<" "<<j+1<<" 1\n";
                        }
                        else
                            out<<it->second<<" "<<l+1<<" "<<i+1<<" "<<j+1<<" 1\n";
                    }
                    else
                        if(min_bool)
                            out<<0<<" "<<l+1<<" "<<i+1<<" "<<j+1<<" -1\n";                        
                }
        for(std::size_t l=size;l<L.size();++l){
            mono=points[L[l][0]]*points[L[l][0]];
            if(!min_bool || mono!=mono_zero){
                    
                it=dct.find(mono);
                if (it==dct.end())
                {    
                    ++n;
                    dct[mono]=n;
                    coeff.push_back(p[mono]);
                    //std::cout<<mono.str()<<std::endl;
                    out<<n<<" "<<size+1<<" "<<l-size+1<<" "<<l-size+1<<" 1\n";
                }
                else
                    out<<it->second<<" "<<size+1<<" "<<l-size+1<<" "<<l-size+1<<" 1\n";
            }
            else
                if (min_bool)
                    out<<0<<" "<<size+1<<" "<<l-size+1<<" "<<l-size+1<<" -1\n";    
        }
        
        std::fstream fout(str_file,std::fstream::out);
        fout<<n<<" = mDIM\n";

        if (size<L.size())
            fout<<size+1<<" = nBLOCK\n";
        else
            fout<<size<<" = nBLOCK\n";
        for(std::size_t l=0;l<size;++l)
            fout<<L[l].size()<<" ";
        if (size<L.size())
            fout<<"-"<<L.size()-size;
        fout<<" = bLOCKsTRUCT\n";
        for(auto i=coeff.begin();i!=coeff.end();++i)
        {
            if (i!=coeff.begin())
                fout<<",";

            fout<<*i;
        }
        fout<<std::endl;
        //four<<
        fout<<out.str();
        fout.close();
    }

}
