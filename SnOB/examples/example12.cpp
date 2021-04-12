#include "SnFtree.hpp"

main(int argc, char** argv){
  
  int m=5;
  if (argc>=2) sscanf(argv[1],"%d",&m);

  Sn G(5);
  Sn::Function f(G);
  f[Sn::Element(1,2,3,4,5,NULL)]=9;
  f[Sn::Element(1,2,4,3,5,NULL)]=3;
  f[Sn::Element(2,3,1,4,5,NULL)]=7;

  Sn::Ftree fsparse(f);
  for(int i=0; i<G.irreducibles.size() && i<m; i++)
    fsparse.Iindex.push_back(i);

  cout<<"The function:"<<endl<<endl;
  cout<<fsparse.str()<<endl;

  fsparse.FFT();
  cout<<"The sparse partial Fourier transform:"<<endl<<endl;
  cout<<fsparse.str()<<endl;

  fsparse.iFFT();
  cout<<"The result of the inverse transform:"<<endl<<endl;  
  cout<<fsparse.str()<<endl;
  
}

