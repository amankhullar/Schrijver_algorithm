//
//  oracle.h
//  Schrijver
//
//  Created by Aman Khullar on 6/6/17.
//  Copyright © 2017 Aman Khullar. All rights reserved.
//

#ifndef oracle_h
#define oracle_h
#include <vector>

class oracle
{
    //It is the number of ones*number of zeroes in the ground_set;
public:
    int oracle_value(std::vector<int>,int);
};
int oracle::oracle_value(std::vector<int> indicator,int dimension)
{
    std::vector<int>::iterator it;
    int sum=0;
    //for(it=indicator.begin();it!=indicator.end();it++)
        //sum+=(*it);
    //{
        //if(indicator.size()==1&&(*it)==3)
            //sum+=(-5);
        //else
            //sum+=1000;
   // }
    return ((dimension-(int)indicator.size())*(int)indicator.size()+sum);
}
#endif /* oracle_h */
 
