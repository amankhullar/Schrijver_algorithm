//
//  main.cpp
//  Schrijver
//
//  Created by Aman Khullar on 6/6/17.
//  Copyright © 2017 Aman Khullar. All rights reserved.
//

#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include "oracle.h"
#include "graph.h"
                                                                    //Correct the code by taking the calls by reference
node *temp_graph;
int intersection(int*,int*,int,int);
void insert(int*,int,int);
void generate_extreme_bases(int*,int*,int);
//node *generate_graph(int*,int*,int);
node* graph_position;
node* node_distance;
float lambda_1[10],lambda_unit_1[10];//lambda_product=1;
struct set_extreme_base
{
    int extreme_base_value[10];
    int ordering_value[10];
    float lambda_value;
};
class base
{
    int dimension=0,ground_set[10],ordering_intial[10],ordering_new[10],extreme_base[10],extreme_base_new[10]/*,P[10],N[10]*/,U[10],s=0,t=0,alpha=0,Matrix[10][10],Matrix_transpose[10][10],/*delta,*/CHI_s[10],CHI_t[10],CHI[10],size_U=0,eb_final_size=0,eb_size=0;
    float delta,y_base_polyhedra[10],x_base_polyhedra[10];
    std::vector<double> lambda,lambda_unit;
    set_extreme_base eb[10],eb_final[1000];
    //int n_P=0,n_N=0;
    //node* graph_position;

public:
    base()
    {
        std::cout<<"enter the ground set dimension\n";
        std::cin>>dimension;
        std::cout<<"enter the ground set\n";
        for(int i=0;i<dimension;i++)
            std::cin>>ground_set[i];
        std::cout<<"enter the initial ordering\n";
        for(int i=0;i<dimension;i++)
        {
            std::cin>>ordering_intial[i];
            ordering_new[i]=ordering_intial[i];
        }
        graph_position=NULL;
    }

    void schrijver();
    void general_exchange_capacity();
    void reduce();
    int *get_U()
    {
        return U;
    }
    int return_size()
    {
        return size_U;
    }
};
void base::schrijver()
{
    node* PTR;
    bool first=true,flag_min=true,flag_search=false,eb_found=false;
    //node* node_distance=NULL;
    int found=0,max_N=0,max_S=0,P[10],N[10],n_N=0,n_P=0,alpha_max=0,temp=0,flag=0,k=0;                                        //k has been taken for storing values in U
    /*for(int i=0;i<dimension;i++)
    {
        ordering_intial[i]=ground_set[i];
        ordering_new[i]=ground_set[i];
    }*/
    float lambda_final,lambda_value=0.0,lambda_sum=0.0,eb_sum=0.0;
    //assuming the extreme_base to be initialized as required by the oracle call(done)
    generate_extreme_bases(ordering_intial,extreme_base,dimension);
    //x_base_polyhedra is again initialized to same as the extreme_base(done)
    for(int i=0;i<dimension;i++)
        x_base_polyhedra[i]=extreme_base[i];
    graph   G(ground_set,ordering_intial,dimension);
   // temp_graph=generate_graph(ground_set,ordering_intial,dimension);
    for(int i=0;i<dimension;i++)
    {
        if(x_base_polyhedra[i]>0)
        {
            P[n_P]=ground_set[i];
            n_P++;
        }
        else if(x_base_polyhedra[i]<0)
        {
            N[n_N]=ground_set[i];
            n_N++;
        }
    }
    temp_graph=G.getGraph();
   // node *PTR_START=temp_graph;
    graph_position=temp_graph;
    for(int i=0;i<dimension;i++)
    {
        /*graph_position=search_graph(temp_graph, dimension, P[i]);
        graph_position->distance=0; */                                        //(temp_graph+(P[i]-1))->distance=0;  O(1)
        found=0;
        PTR=graph_position->next;
        if(search(N, n_N, graph_position->data)>=0)
        {
            found=1;
            U[k++]=graph_position->data;
        }
        while(PTR!=NULL&&found==0)                                        //Check for further implementation
        {
            if(search(N,n_N,PTR->data)>=0)
            {
                U[k++]=graph_position->data;
                found=1;
                size_U=k;
            }
            PTR=PTR->next;
        }
        graph_position=graph_position->next_node;
    }
                                                                        //End of initialization
    graph_distance_initialization(&temp_graph,dimension,P,n_P,N,n_N);
    
    while(intersection(U,P,k,n_P)==1)
    {
        //lambda_product=1;
        max_N=0,max_S=0;
        for(int i=0;i<n_N;i++)
        {
            node_distance=search_graph(&temp_graph, dimension, N[i]);
            if(node_distance->distance>=max_N)
            {
                max_N=node_distance->distance;
                t=node_distance->data;
            }
        }
        node_distance=temp_graph;
        for(int i=0;i<dimension;i++)
        {
            if(node_distance->distance==max_N-1&&node_distance->distance>=max_S)
            {
                max_S=node_distance->distance;
                s=node_distance->data;
            }
            node_distance=node_distance->next_node;
        }
        if(first==true)
        {
            alpha=(search(ordering_intial, dimension, t)-search(ordering_intial, dimension, s));
            lambda_value=1;
            first=false;
        }
        else
        {
            alpha_max=0;
            for(int i=0;i<eb_final_size;i++)
            {
                temp=(search(eb_final[i].ordering_value,dimension,t)-search(eb_final[i].ordering_value, dimension, s));
                if(temp>alpha_max)
                {
                    alpha_max=temp;
                    flag=i;
                }
            }
            alpha=alpha_max;
            for(int i=0;i<dimension;i++)
                ordering_intial[i]=eb_final[flag].ordering_value[i];
            generate_extreme_bases(ordering_intial, extreme_base, dimension);
            lambda_value=eb_final[flag].lambda_value;
        }
        if(alpha==0)
        {
            return;
        }
        general_exchange_capacity();
        for(int j=0;j<eb_size;j++)
        {
            for(int i=0;i<dimension;i++)
            {
                eb_final[eb_final_size].ordering_value[i]=eb[j].ordering_value[i];
                eb_final[eb_final_size].extreme_base_value[i]=eb[j].extreme_base_value[i];
                eb_final[eb_final_size].lambda_value=eb[j].lambda_value;
            }
            eb_final_size++;
        }
        for(int i=0;i<eb_final_size;i++)
            eb_sum+=eb_final[i].lambda_value;
        for(int i=0;i<eb_final_size;i++)
            eb_final[i].lambda_value/=eb_sum;
        if((x_base_polyhedra[t-1]*(-1))/delta<1)
        {
            lambda_sum=0;
            for(int i=0;i<=eb_final_size;i++)
                lambda_sum+=eb_final[i].lambda_value;
            for(int i=0;i<=eb_final_size;i++)
                eb_final[i].lambda_value=eb_final[i].lambda_value/lambda_sum;
           // lambda_final=((-1)*x_base_polyhedra[t-1])/delta;
            lambda_final=(x_base_polyhedra[t-1]*(-1))/delta;
            flag_min=false;
        }
        else
        {
            lambda_final=1;
            //removing the problematic ordering
            for(int i=0;i<eb_final_size;i++)
            {
                flag_search=true;
                int j=0;
                while(j<dimension&&flag_search==true)
                {
                    if(eb_final[i].ordering_value[j]!=ordering_intial[j])
                        flag_search=false;
                    j++;
                }
                if(j==dimension)
                {
                    while(i<eb_final_size-1)
                    {
                        for(j=0;j<dimension;j++)
                        {
                            eb_final[i].ordering_value[j]=eb_final[i+1].ordering_value[j];
                            eb_final[i].extreme_base_value[j]=eb_final[i+1].extreme_base_value[j];
                        }
                        eb_final[i].lambda_value=eb_final[i+1].lambda_value;
                        i++;
                    }
                    eb_final_size--;
                    eb_found=true;
                }
            }
            if(eb_found==true)
            {
                float sum=0;
                for(int i=0;i<eb_final_size;i++)
                    sum+=eb_final[i].lambda_value;
                for(int i=0;i<eb_final_size;i++)
                    eb_final[i].lambda_value/=sum;
            }
        }
        //Updating the values
        for(int i=0;i<dimension;i++)
        {
            CHI[i]=0;
        }
        CHI[t-1]=1;
        CHI[s-1]=(-1);
        //if(x_base_polyhedra[s-1]<=0)        //This is not from the book
            //CHI[s-1]=0;
        for(int i=0;i<dimension;i++)
            y_base_polyhedra[i]=x_base_polyhedra[i]+lambda_value*delta*CHI[i];
        for(int i=0;i<dimension;i++)
            x_base_polyhedra[i]=(1-lambda_final)*x_base_polyhedra[i]+lambda_final*y_base_polyhedra[i];
      //These are repeated lines
        n_N=0;
        n_P=0;
        for(int i=0;i<dimension;i++)
        {
            if(x_base_polyhedra[i]>0)
            {
                P[n_P]=ground_set[i];
                n_P++;
            }
            else if(x_base_polyhedra[i]<0)
            {
                N[n_N]=ground_set[i];
                n_N++;
            }
        }
        for(int i=0;i<alpha;i++)
            modify_graph(&temp_graph, eb[i].ordering_value, dimension);
        graph_position=temp_graph;
        //These are repeated lines
        k=0;
        found=0;
        for(int i=0;i<dimension;i++)
        {
            found=0;
            PTR=graph_position->next;
            while(PTR!=NULL&&found==0)
            {
                if(search(N,n_N,PTR->data)>=0||search(N, n_N, graph_position->data)>=0)
                {
                    U[k++]=graph_position->data;
                    found=1;
                    size_U=k;
                }
                PTR=PTR->next;
            }
            graph_position=graph_position->next_node;
        }
        graph_distance_initialization(&temp_graph,dimension,P,n_P,N,n_N);
        //Proof of the Caratheodory's theorem
        if(eb_final_size>dimension+1)
        {
            while(eb_final_size>dimension+1)
                reduce();
        }
    }
    
}

int intersection(int *U,int *P,int U_dimension,int P_dimension)
{
    for(int i=0;i<U_dimension;i++)
    {
        if(search(P,P_dimension,*(U+i))>=0)
            return 1;
    }
    return 0;
}

void base::general_exchange_capacity()
{
    int k=0,number_u=0,start=0,end=0,size=1;
    float sum_matrix,value,sum=0;
    std::vector<double>::iterator it;
    //float lambda_1[10],lambda_unit_1[10];
    for(int i=0;i<dimension;i++)
    {
        CHI_s[i]=0;
        CHI_t[i]=0;
    }
    start=search(ordering_intial,dimension,s);
    end=search(ordering_intial,dimension,t);
    CHI_s[start]=1;
    CHI_t[end]=1;
    for(int i=0;i<dimension;i++)
        CHI[i]=CHI_t[i]-CHI_s[i];
    eb_size=0;
    for(int i=start+1;i<=end;i++)
    {
        number_u++;
        for(int j=0;j<dimension;j++)
            ordering_new[j]=ordering_intial[j];
        insert(ordering_new,start,i);
        //compute the extreme_base_new from the oracle call
        generate_extreme_bases(ordering_new, extreme_base_new, dimension);
        for(int j=0;j<dimension;j++)
        {
            eb[k].extreme_base_value[j]=extreme_base_new[j];
            eb[k].ordering_value[j]=ordering_new[j];
        }
        for(int j=0;j<dimension;j++)
            Matrix[k][j]=(extreme_base_new[ordering_intial[j]-1]-extreme_base[ordering_intial[j]-1]);
        k++;
        eb_size++;
    }
    for(int i=0;i<alpha;i++)
    {
        for(int j=0;j<dimension;j++)
            Matrix_transpose[j][i]=Matrix[i][j];
    }
    //calculate lambda through back-substitution.
    value=Matrix_transpose[end][alpha-1];
    value=(1)/value;
    lambda_1[alpha-1]=value;
    it=lambda.begin();                          //lambda as a std::vector is not being implemented hence float value has been taken.
    //lambda.insert(it,value);
    for(int i=1;i<alpha;i++)
    {
        sum_matrix=0;
        it=lambda.begin();
        int j=0;
        for(;j<size;j++)
        {
            sum_matrix+=(Matrix_transpose[end-i][alpha-j-1]*lambda_1[alpha-j-1]);
        }
        size+=1;
        lambda_1[alpha-j-1]=(sum_matrix/Matrix_transpose[end-i][alpha-j-1])*(-1);
    }
    for(int i=0;i<size;i++)
        sum+=lambda_1[i];
    delta=1/sum;
    for(int i=0;i<size;i++)
    {
        lambda_unit_1[i]=lambda_1[i]/sum;
        //lambda_product*=lambda_unit_1[i];       //This is a part of hunch
        eb[i].lambda_value=lambda_unit_1[i];
    }
    //Return the new extreme bases
    //The following is a hunch
   // for(int i=0;i<dimension;i++)
       // ordering_intial[i]=ordering_new[i];
}

void insert(int *array,int start,int value_position)
{
    int temp=array[value_position];
    for(int i=value_position-1;i>=start;i--)
        array[i+1]=array[i];
    array[start]=temp;
}
void generate_extreme_bases(int *ordering,int *extreme_base,int dimension)
{
    std::vector<int> indicator;
    oracle ORACLE;
    int current=0,previous=0;
    for(int i=0;i<dimension;i++)
    {
        indicator.push_back(ordering[i]);
        current=ORACLE.oracle_value(indicator,dimension);
        extreme_base[ordering[i]-1]=current-previous;
        previous=current;
    }

}
/*node* generate_graph(int *ground_set,int *ordering_initial,int dimension)
{
    graph G(ground_set,ordering_initial,dimension);
    return G.getGraph();
}*/
void base::reduce()
{
    using namespace Eigen;
    MatrixXd B(dimension,eb_final_size);
    VectorXd U(eb_final_size);
    VectorXd Zero(dimension);
    float sum=0.0,alpha=0.0;
    int index=0;
    for(int i=0;i<dimension;i++)
    {
        for(int j=0;j<eb_final_size-1;j++)
        {
            B(i,j)=eb_final[j+1].extreme_base_value[i]-eb_final[0].extreme_base_value[i];
        }
        Zero(i)=0;
    }
    U=B.colPivHouseholderQr().solve(Zero);
    for(int i=0;i<eb_final_size-1;i++)
        sum+=U(i);
    for(int i=eb_final_size-1;i>0;i--)
        U(i)=U(i-1);
    U(0)=(-sum);
    for(int i=0;i<eb_final_size;i++)
    {
        if(U(i)>0&&(eb_final[i].lambda_value/U(i))<alpha)
        {
            alpha=eb_final[i].lambda_value/U(i);
            index=i;
        }
    }
    for(int i=0;i<eb_final_size;i++)
    {
        eb_final[i].lambda_value=eb_final[i].lambda_value-alpha*U(i);
    }
    for(int i=index;i<eb_final_size-1;i++)
    {
        for(int j=0;j<dimension;j++)
        {
            eb_final[i].ordering_value[j]=eb_final[i+1].ordering_value[j];
            eb_final[i].extreme_base_value[j]=eb_final[i+1].extreme_base_value[j];
        }
        eb_final[i].lambda_value=eb_final[i+1].lambda_value;
    }
    eb_final_size--;
}
int main()
{
    base B;
    B.schrijver();
    int *minimizer_set;
    std::cout<<"The minimizer set is\n";
    minimizer_set=B.get_U();
    for(int i=0;i<B.return_size();i++)
        std::cout<<minimizer_set[i]<<" ";
    std::cout<<"\n";
    return 0;
}
