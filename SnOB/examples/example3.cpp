#include "Sn.hpp"
#include "SnElement.hpp"
#include <iostream>

main(){
  
  Sn G(3);

  for(int i=0; i<G.order; i++)
    cout<<G[i]->str()<<endl;
      
}
