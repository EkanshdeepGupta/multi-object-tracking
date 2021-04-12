#include "SnFtree.hpp"

main(){
  
  Sn::Sn G(4);
  Sn::Function f(G);
  f[Sn::Element(1,2,4,3,NULL)]=3;
  f[Sn::Element(2,3,1,4,NULL)]=7;

  cout<<"The dense Fourier transform:"<<endl; 
  cout<<Sn::FourierTransform(f).str()<<endl;

  Sn::Ftree fsparse(f);
  for(int i=0; i<G.irreducibles.size(); i++)
    fsparse.Iindex.push_back(i);

  fsparse.FFT();
  cout<<"The sparse Fourier transform:"<<endl; 
  cout<<fsparse.str()<<endl;
  
}
