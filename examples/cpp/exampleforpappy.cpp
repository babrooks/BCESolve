#include "bcedistr.hpp"
#include <iostream>

int main()
{
  distributionArray d;
  
  // Creates a bimodal distribution with modes at 0.25 and 0.75.
  d.push_back(unimodal(1,0.25,3),0.5);
  d.push_back(unimodal(1,0.75,3),0.5);

  cout << "F(0.25,0.25) = " << d(0.25,0.25) << endl;
  cout << "PDF(0.5,0.5) with increment 0.25 = " 
       << PDF(d,0.5,0.5,0.25) << endl;

  d.clear();

  d.push_back(uniform(),0.25);
  d.push_back(vToTheAlpha(1.5),0.75);

  cout << "F(0.25,0.25) = " << d(0.25,0.25) << endl;
  cout << "PDF(0.5,0.5) with increment 0.25 = " 
       << PDF(d,0.5,0.5,0.25) << endl;

  return 0;
} // main
