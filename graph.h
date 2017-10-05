//
//  graph.h
//  Schrijver
//
//  Created by Aman Khullar on 6/6/17.
//  Copyright © 2017 Aman Khullar. All rights reserved.
//

#ifndef graph_h
#define graph_h
#include<queue>
#include<string>

struct node{
    int distance=0;
    int data=0;
    std::string color;
    node *next;
    node *next_node;
};

int search(int*,int,int);
node* search_graph(node*,int,int);
bool search_adjacency(node *,int,int);
void graph_distance_initialization(node**,int,int*,int,int*,int);

class graph
{
    node *graph_1;
    node *PTR;
public:
    graph(int *,int*,int);
    void modify_graph(int*,int*,int);
    node *getGraph()
    {
        return graph_1;
    }
};
graph::graph(int *base_array,int *ordering,int dimension)
{
    graph_1=new node;
    node *PTR_START=graph_1;
    int position=0;
    for(int i=0;i<dimension;i++)
    {
        position=0;
        PTR_START->data=(*(base_array+i));
        PTR_START->distance=0;
        PTR_START->color="white";
        PTR=PTR_START;
        position=search(ordering, dimension, PTR_START->data);
        position++;
        while(position<dimension)
        {
            PTR->next=new node;
            PTR=PTR->next;
            PTR->data=ordering[position++];
        }
        PTR->next=NULL;
        PTR_START->next_node=new node;
        PTR_START=PTR_START->next_node;
    }
    PTR_START=NULL;
}
int search(int *array,int dimension,int value)
{
    for(int i=0;i<dimension;i++)
    {
        if((*(array+i))==value)
            return i;
    }
    return -1;
}

node* search_graph(node **array,int dimension,int value)
{
    node *PTR=(*array);
    for(int i=0;i<dimension;i++)
    {
        if(PTR->data==value)
            return PTR;
        PTR=PTR->next_node;
    }
    
    return NULL;
}
void modify_graph(node **base_graph,int *ordering,int dimension)
{
    node *PTR_START=NULL;
    node *PTR=NULL;
    bool position=false;
    for(int i=0;i<dimension;i++)
    {
        PTR=search_graph(base_graph, dimension, ordering[i]);
        PTR_START=PTR;
        for(int j=i+1;j<dimension;j++)
        {
            position=search_adjacency(PTR_START, dimension, ordering[j]);
            if(position==false)
            {
                while(PTR->next!=NULL)
                    PTR=PTR->next;
                PTR->next=new node;
                PTR=PTR->next;
                PTR->data=ordering[j];
                PTR->next=NULL;
            }
        }
    }
}
void graph_distance_initialization(node **base_graph,int dimension,int *P,int P_dimension,int *N,int N_dimension)
{
    node *PTR=(*base_graph),*temp=NULL,*temp_start=NULL;
    int data=0;
    std::queue<int> QUEUE;
    for(int i=0;i<P_dimension;i++)
    {
        QUEUE.push(P[i]);
        temp=search_graph(base_graph, dimension, P[i]);
        temp->distance=0;
        temp->color="gray";
    }
    while(!QUEUE.empty())
    {
        data=QUEUE.front();
        QUEUE.pop();
        PTR=search_graph(base_graph, dimension, data);
        temp_start=PTR;
        PTR=PTR->next;
        while(PTR!=NULL)
        {
            temp=search_graph(base_graph, dimension, PTR->data);
            if(temp->color=="white")
            {
                temp->color="gray";
                temp->distance=temp_start->distance+1;
                QUEUE.push(temp->data);
            }
            temp_start->color="black";
            PTR=PTR->next;
        }
    }
}
bool search_adjacency(node *array,int dimension,int value)
{
    node *PTR=array->next;
    while(PTR!=NULL)
    {
        if(PTR->data==value)
            return true;
        PTR=PTR->next;
    }
    return false;
}
#endif /* graph_h */
/*
 graph::graph(int *base_array,int *ordering,int dimension)
 {
 node *PTR_START=graph_1;
 int position=0;
 for(int i=0;i<dimension;i++)
 {
 position=0;
 graph_1[i]->data=(*(base_array+i));
 graph_1[i]->distance=0;
 PTR=graph_1[i];
 position=search(ordering, dimension, graph_1[i]->data);
 position++;
 while(position<dimension)
 {
 PTR->next=new node;
 PTR=PTR->next;
 PTR->data=ordering[position++];
 }
 PTR->next=NULL;
 }
 }*/
