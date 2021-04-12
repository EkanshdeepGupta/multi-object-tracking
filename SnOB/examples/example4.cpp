#include "Sn.hpp"
#include "SnIrreducible.hpp"
#include <iostream>

main(){
  
  Sn G(5);

  for(int i=0; i<G.irreducibles.size(); i++)
    cout<<G.irreducibles[i]->str()<<endl;
      
}
