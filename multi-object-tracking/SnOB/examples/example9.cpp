#include "SnFunction.hpp"
#include "SnFourierTransform.hpp"
#include <iostream>

main(){
  
  Sn G(4);

  Sn::Function f(G);
  f.randomize();

  Sn::Function g(G);
  g.randomize();

  Sn::Function* h=g.convolve(f);
  cout<<h->str()<<endl;

  Sn::Function hdash(G);

  for(int i=0; i<G.order; i++)
    for(int j=0; j<G.order; j++){
      Sn::Element* x=G[j]; 
      Sn::Element* z=G[i];
      Sn::Element* xinv=x->inverse();
      Sn::Element* y=(*z)*(*xinv); 
      hdash[*z]+=g[*y]*f[*x];
      delete x,xinv,y,z;
    }

  cout<<hdash.str()<<endl;
}
