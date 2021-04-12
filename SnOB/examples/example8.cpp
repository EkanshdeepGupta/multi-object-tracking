#include "SnFunction.hpp"
#include "SnFourierTransform.hpp"
#include <iostream>

main(){
  
  Sn G(4);

  Sn::Function f(G);
  f.randomize();
  cout<<f.str()<<endl;

  Sn::FourierTransform* F=f.FFT();
  cout<<F->str()<<endl;

  Sn::Function* fdash=F->iFFT();
  cout<<fdash->str()<<endl;

}
