#include "SnIrreducible.hpp"
#include <iostream>

main(){
  
  Sn::Sn G(5);
  Sn::Irreducible& rho=*G.irreducibles[2];
  Partition lambda(2,2,1,NULL); 
  Partition mu(3,1,1,NULL);
  Sn::Element sigma(2,3,1,4,5,NULL);
      
  cout<<"Irreducible: "<<rho.str()<<endl<<endl;
  cout<<"Degree: "<<rho.degree<<endl<<endl;
  cout<<"Tableaux: "<<endl<<endl;
  for(int i=0; i<rho.degree; i++) 
    cout<<rho.tableau(i)->str()<<endl;
  cout<<"Character at mu: "<<rho.character(mu)<<endl<<endl;
  cout<<"Representation matrix at sigma: "<<endl<<endl<<rho.rho(sigma)->str()<<endl;

}

