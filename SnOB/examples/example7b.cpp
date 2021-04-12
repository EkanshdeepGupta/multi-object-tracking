#include "SnIrreducible.hpp"
#include <iostream>

main(){
  
  Sn G(5);
  Sn::Irreducible& rho=*G.irreducibles[2];

  Sn::Element sigma1(1,2,4,3,5,NULL);
  Sn::Element sigma2(2,3,1,4,5,NULL);


  cout<<rho.rho(*(sigma2*sigma1))->str()<<endl;

  cout<<((*rho.rho(sigma2))*(*rho.rho(sigma1)))->str()<<endl;

}
