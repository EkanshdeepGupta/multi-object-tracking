#include "SnIrreducible.hpp"

main(int argc, char** argv){

  int n=5; 
  if(argc>=2) sscanf(argv[1],"%d",&n);

  Sn G(n);

  int Npartitions=G.irreducibles.size();
  Matrix<double> chartable(Npartitions,Npartitions);

  for(int i=0; i<Npartitions; i++)
    for(int j=0; j<Npartitions; j++)
      chartable(i,j)=round(G.irreducibles[i]->character(G.irreducibles[j]->partition));

  cout<<chartable.str()<<endl;

}
