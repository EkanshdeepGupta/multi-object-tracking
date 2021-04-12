#include "SnElement.hpp"
#include "Cycles.hpp"
#include <iostream>

main(){

  Sn::Element p1(1,2,4,3,5,NULL);
  Sn::Element p2(2,3,1,4,5,NULL);
  
  cout<<"v1 : "<<Cycles(p1).str()<<endl;
  cout<<"v2 : "<<Cycles(p2).str()<<endl;
  
}
