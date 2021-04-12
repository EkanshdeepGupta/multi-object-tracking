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


#include "SnFourierTransform.hpp"
#include "SnIrreducible.hpp"

#include <iostream>

Sn::FourierTransform::FourierTransform(const Sn& _group):group(&_group),n(_group.n){
  for(int i=0; i<group->irreducibles.size(); i++){
    const int degree=group->irreducibles[i]->degree;
    matrix.push_back(new Matrix<FIELD >(degree,degree,0));
  }
};



Sn::FourierTransform::FourierTransform(const Sn& _group, const vector<Matrix<FIELD >*> matrices):
  group(&_group),n(_group.n){
  for(int i=0; i<group->irreducibles.size(); i++)
    matrix.push_back(matrices[i]);
}



Sn::FourierTransform::FourierTransform(const Function& f):
  group(f.group),
  n(f.group->n){
  fft(f,0);
}



Sn::FourierTransform::~FourierTransform(){
  for(int i=0; i<matrix.size(); i++) delete matrix[i];
}



Sn::Function* Sn::FourierTransform::iFFT() const{
  return new Function(*this);
}



void Sn::FourierTransform::fft(const Sn::Function& f, const int offset){
  int suborder=group->order/n;

  if(n==1){
    matrix.push_back(new Matrix<FIELD >(1,1));
    matrix[0]->at(0,0)=f.f[offset];
    return;
  }

  FourierTransform* F[n];
  for(int j=1; j<=n; j++){
    F[j-1]=new FourierTransform(*group->subgroup,0);
    F[j-1]->fft(f,offset+(n-j)*suborder);
  }

  for(int i=0; i<group->irreducibles.size(); i++){
    Sn::Irreducible* rho=group->irreducibles[i];
    const int degree=rho->degree;
    Matrix<FIELD >* M=new Matrix<FIELD >(degree,degree,0);
    matrix.push_back(M); // note: matrix is suposed to be empty when this function is called 

    for(int j=1; j<=n; j++){
      vector<Matrix<FIELD >*> participants;
      for(int eta=0; eta<rho->etaindex.size(); eta++)
	participants.push_back(F[j-1]->matrix[rho->etaindex[eta]]); //Hack here
      Matrix<FIELD > tildef(degree,participants);
      //rho->applyTransposition(j,tildef);
      rho->applyCycleL(j,tildef);
      (*M)+=tildef;
      
    }
  }

  for(int i=0; i<n; i++) delete F[i]; 
}



void Sn::FourierTransform::ifft(Sn::Function* target, const int _offset) const{
  int order=group->order;
  const int suborder=order/n;

  Sn::FourierTransform Fsub(*group->subgroup);
  for(int j=1; j<=n; j++){
    if(j>1) for(int i=0; i<Fsub.matrix.size(); i++) Fsub.matrix[i]->fill(0);
    for(int rhoindex=0; rhoindex<matrix.size(); rhoindex++){
      Sn::Irreducible* rho=group->irreducibles[rhoindex];
      Matrix<FIELD > M(*matrix[rhoindex]);
      //rho->applyTransposition(j,M);
      rho->applyCycleL(j,M,n,1);
      int offset=0;
      for(int i=0; i<rho->etaindex.size(); i++){
	Matrix<FIELD >* Msub=Fsub.matrix[rho->etaindex[i]];
	const int degree=Msub->n;
	FIELD multiplier=FIELD(double(rho->degree)/double(degree*n));
	for(int a=0; a<degree; a++)
	  for(int b=0; b<degree; b++)
	    (*Msub)(a,b)+=M(offset+a,offset+b)*multiplier; ///FIELD(n);
	offset+=degree;
      }
    }
    if(n>2){
      Fsub.ifft(target, _offset+(n-j)*suborder);
     }else{
      target->f[_offset+n-j]=Fsub.matrix[0]->at(0,0);
    }
  }
}



FIELD  Sn::FourierTransform::operator()(const StandardTableau& t1, const StandardTableau& t2) const{
  Partition shape=t1.shape();
  for(int i=0; i<group->irreducibles.size(); i++){
    const Sn::Irreducible* rho=group->irreducibles[i];
    if(shape==rho->partition){
      for(int j=0; j<rho->degree; j++){
	StandardTableau* T1=rho->tableau(j);
	if(t1==*T1){
	  for(int k=0; k<rho->degree; k++){
	    StandardTableau* T2=rho->tableau(k);
	    if(t2==*T2){
	      delete T2; 
	      delete T1;
	      return matrix[i]->at(j,k);
	    } 
	    delete T2;
	  }
	}
	delete T1;
      }
    }
  }
}



string Sn::FourierTransform::str() const {
  ostringstream result;
  for (int i=0; i<matrix.size(); i++)
    result<<matrix[i]->str()<<endl;
  return result.str();
}

