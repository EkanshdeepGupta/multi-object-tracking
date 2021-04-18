#include "SnFunction.hpp"
#include "SnFourierTransform.hpp"
#include "SnIrreducible.hpp"
#include <iostream>

main(){
  
  Sn G(5);

  Sn::Element sigma(4,1,3,5,2,NULL);

  Sn::Function f(G);
  f[sigma]=1;
  Sn::FourierTransform F(f);

  Sn::Irreducible& rho=*G.irreducibles[3];

  cout<<F.matrix[3]->str()<<endl;

  cout<<rho.rho(sigma)->str()<<endl;

}

