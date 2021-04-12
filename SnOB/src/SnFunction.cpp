/* -----------------------------------------------------------------------------

  SnOB - An FFT toolkit for the symmetric group
              development version

  Copyright (C) 2006  Imre Risi Kondor


  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the 

  Free Software Foundation, Inc., 
  51 Franklin Street, Fifth Floor, 
  Boston, MA  02110-1301, USA.

  This software is provided for educational and research purposes. 
  Commercial use is prohibited. 

  See the accompanying LICENSE for details

----------------------------------------------------------------------------- */


#include "SnFunction.hpp"
#include "SnIrreducible.hpp"

Sn::Function::Function(const Sn& _group):
  group(&_group){
  n=group->n;
  order=group->order;
  f=new FIELD[group->order];
  for(int i=0; i<order; i++) f[i]=0;
}



Sn::Function::Function(const Sn::FourierTransform& F):
  group(F.group){
  n=group->n;
  order=group->order;
  f=new FIELD[group->order]; 
  for(int i=0; i<order; i++) f[i]=0;
  F.ifft(this,0);
}



Sn::FourierTransform* Sn::Function::FFT() const {
  return new FourierTransform(*this);
}



void Sn::Function::randomize(){
  for(int i=0; i<order; i++) f[i]=((double) rand())/((double)RAND_MAX+1);
}



FIELD& Sn::Function::operator[](const Sn::Element& p){
  int t=0;
  int fact=order;
  int v[n]; for(int i=1; i<=n; i++) v[i-1]=i;
  for(int m=n; m>0; m--){
    fact/=m;
    int j=p.action(m);
    t+=(m-v[j-1])*fact;
    for(int i=j+1; i<=n; i++) v[i-1]--;
  }
  return f[t];
}



string Sn::Function::str() const{
  ostringstream result;
  for(int i=0; i<order; i++)
    result<<(*group)[i]->str()<<" : "<<f[i]<<endl;
  return result.str();
}



Sn::Function* Sn::Function::convolve(const Sn::Function& o) const{
  Sn::Function* result=new Sn::Function(*group);
  for(int i=0; i<order; i++)
    for(int j=0; j<order; j++){
      Sn::Element* z=(*(*group)[j])*(*(*group)[i]);
      (*result)[*z]+=o.f[i]*f[j];
      delete z;
    }
  return result;
}




void Sn::Function::diffuse(const double beta){
  FourierTransform* F=FFT();
  for(int rhoix=0; rhoix<group->irreducibles.size(); rhoix++){
    Sn::Irreducible* rho=group->irreducibles[rhoix];
    Matrix<FIELD > M(rho->degree);
    rho->applyTransposition(n-1,M);
    double alpha=((double)1-M.trace()/rho->degree)*n*(n-1)/2.0;
    (*F->matrix[rhoix])*=exp(alpha*beta);
  }
  Function* fdash=F->iFFT();
  for(int i=0; i<order; i++) f[i]=fdash->f[i]; // would be much faster without copy
}

