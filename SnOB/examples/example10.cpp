#include "SnFunction.hpp"
#include "SnFourierTransform.hpp"
#include <iostream>

main(){
  
  Sn::Sn G(4);

  Sn::Function f(G);
  f.randomize();
  cout<<f.str()<<endl;

  f.diffuse(0.1);
  cout<<f.str()<<endl;
}
