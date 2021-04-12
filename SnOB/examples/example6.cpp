#include "Sn.hpp"
#include "SnIrreducible.hpp"
#include <iostream>

string printAncestors(Sn::Irreducible& rho, string indenter){
  ostringstream result;
  result<<indenter<<rho.str()<<endl;
  for(int i=0; i<rho.eta.size(); i++){
    result<<printAncestors(*rho.eta[i],indenter+"  ");
  }
  return result.str();
}


main(){
  
  Sn G(5);

  for(int i=0; i<G.irreducibles.size(); i++)
    cout<<printAncestors(*G.irreducibles[i],"");
      
}

