#include "SnElement.hpp"
#include <iostream>

main(){

  Sn::Element p1(1,2,4,3,5,NULL);
  Sn::Element p2(2,3,1,4,5,NULL);

  Sn::Element* a=p2*p1;
  Sn::Element* b=p2.inverse();
  Sn::Element* c=(*b)*p1;
  
  cout<<"p1          : "<<p1.str()<<endl;
  cout<<"p2          : "<<p2.str()<<endl;
  cout<<"p2*p1       : "<<a->str()<<endl;
  cout<<"p2^{-1}     : "<<b->str()<<endl;
  cout<<"(p2^{-1})*p1: "<<c->str()<<endl;
  
  delete a,b,c;
}
