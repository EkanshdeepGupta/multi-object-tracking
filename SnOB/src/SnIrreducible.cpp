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


#include "SnIrreducible.hpp"

#include <sstream>
#include <math.h>

Sn::Irreducible::Irreducible(const Sn* _group, const Partition& _partition):
  group(_group),
  partition(_partition),
  n(_group->n),
  tableauxComputed(0),
  YORComputed(0)
{
  if(n==1){
    tableauV.push_back(StandardTableau(partition));
    tableauxComputed=1;
    degree=1;
    return;
  }

  degree=0;
  vector<Partition> subpartitions=partition.restrictions();
  for(vector<Partition>::iterator it=subpartitions.begin(); it!=subpartitions.end(); it++){
    int subrepresentationindex; // warning: this is return argument of following function
    Irreducible* subrepresentation=group->subgroup->irreducible(*it,subrepresentationindex);
    eta.push_back(subrepresentation);
    etaindex.push_back(subrepresentationindex);
    degree+=subrepresentation->degree;
  }
}




////////////////////////////////////////////////////////////////////////// TABLEAU STUFF



StandardTableau* Sn::Irreducible::tableau(const int T) const{
  if(tableauxComputed) return new StandardTableau(tableauV[T]);
  StandardTableau* result;
  int t=T;
  for(int etaix=0; etaix<eta.size(); etaix++){
    Sn::Irreducible* etap=eta[etaix];
    t-=etap->degree;
    if(t<0){
      result=etap->tableau(t+etap->degree);
      result->growTo(partition);
      break;
    }
  }
  return result;
}



void Sn::Irreducible::computeTableaux(){
  if(tableauxComputed) return;
  for(int etaix=0; etaix<eta.size(); etaix++){
    Sn::Irreducible* etap=eta[etaix];
    etap->computeTableaux();
    for(vector<StandardTableau>::iterator it=etap->tableauV.begin(); it!=etap->tableauV.end(); it++){
      StandardTableau t(*it);
      tableauV.push_back(t.growTo(partition));
    }
  }
  tableauxComputed=1;
}




//////////////////////////////////////////////////////////////////////// YOUNG ORTHOGONAL STUFF 


int Sn::Irreducible::YoungOrthogonalCoefficients(const int tau, const int T, double& c1, double& c2) const{
  int index=T*(n-1)+tau-1; 
  if (YORComputed){
    c1=coeff1[index]; 
    c2=coeff2[index]; 
    return tdash[index];
  }
  int result;
  // if(!tableauxComputed) computeTableaux();
  if(tableauxComputed){
    StandardTableau Tdash=tableauV[T];
    int distance;
    // Below Tdash is changed and distance is passed by reference and changed 
    if(Tdash.applyTransposition(tau,distance)){ // Case 1: result is linear combination 
      for(int i=0; i<degree; i++) // find the index of Tdash...
	if(Tdash==tableauV[i]){ 
	  result=i; break;}
      c1=1.0/distance;
      c2=sqrt(1-pow(c1,2));
    }else{ // case 2: result is just rescaling
      result=-1;
      c1=1.0/distance;
    }
    return result;
  }
  // duplicated above for speed 
  StandardTableau* Tdash=tableau(T);
  int distance;
  // Below Tdash is changed and distance is passed by reference and changed 
  if(Tdash->applyTransposition(tau,distance)){ // Case 1: result is linear combination 
    for(int i=0; i<degree; i++){ // find the index of Tdash...
      StandardTableau* Ti=tableau(i);
      if(*Tdash==*Ti){ 
	result=i; delete Ti; break;}
      delete Ti;
    }
    c1=1.0/distance;
    c2=sqrt(1-pow(c1,2));
  }else{ // case 2: result is just rescaling
    result=-1;
    c1=1.0/distance;
  }
  delete Tdash;
  return result;
}



void Sn::Irreducible::computeYOR(){
  computeTableaux();
  tdash=new int[degree*(n-1)];
  coeff1=new double[degree*(n-1)];
  coeff2=new double[degree*(n-1)];
  for(int T=0; T<degree; T++){ // for each tableau...
    for(int j=1; j<n; j++){ // for each transposition (j,j+1)...
      const int index=T*(n-1)+j-1;
      tdash[index]=YoungOrthogonalCoefficients(j, T, coeff1[index], coeff2[index]);
    }
  }
  YORComputed=1;
}



///////////////////////////////////////////////////////////////////////// MATRIX STUFF



Matrix<FIELD >* Sn::Irreducible::rho(const Sn::Element& p){ 
  Matrix<FIELD >* result= new Matrix<FIELD >(degree);
  int v[n]; for(int i=1; i<=n; i++) v[i-1]=i;
  for(int m=n; m>0; m--){
    int j=p.iaction(m);
    applyCycleL(v[j-1],*result,m,1); // get v[j-1] into place at end 
    for(int i=j+1; i<=n; i++) v[i-1]--;
  }
  return result;
}



FIELD Sn::Irreducible::character(const Partition& mu){
  Matrix<FIELD > M(degree);
  int m=n;
  for(int k=0; k<mu.size(); k++){
    if(mu[k]==1) break;
    applyCycleL(m-mu[k]+1,M,m);
    m-=mu[k];
  }
  return M.trace(); 
}


void Sn::Irreducible::applyCycleL(const int j, Matrix<FIELD >& M, int m, bool inverse){
  bool done[degree];
  FIELD* array=M.array;
  if(!YORComputed) computeYOR(); 
  if(m==-1) m=n;  
  for(int p=m-1; p>=j; p--){
    int tau=p;
    if (inverse) tau=j+m-1-p;
    for(int T=0; T<degree; T++) done[T]=0; 
    for(int T=0; T<degree; T++){ // tabloid[T] goes to ...
      if(!done[T]){
	double c1,c2; // Warning: result is returned in these arguments below!!!
	const int Tdash=YoungOrthogonalCoefficients(tau,T,c1,c2);
	if(Tdash==-1)
	  for(int i=T*degree; i<T*degree+degree; i++) array[i]=c1*array[i];
	else{
	  for(int i=0; i<degree; i++){
	    FIELD temp=M(T,i);
	    M(T,i)=c1*temp+c2*M(Tdash,i);
	    M(Tdash,i)=-c1*M(Tdash,i)+c2*temp;
	  }
	  done[Tdash]=1;
	}
      }
    }
  }
}



void Sn::Irreducible::applyCycleR(const int j, Matrix<FIELD >& M, int m, bool inverse){
  bool done[degree];
  FIELD* array=M.array;
  if(!YORComputed) computeYOR(); 
  if(m==-1) m=n;  
 
  for(int p=m-1; p>=j; p--){
    int tau=p;
    if (inverse) tau=j+m-1-p;
    for(int T=0; T<degree; T++) done[T]=0; 
    for(int T=0; T<degree; T++){ // tabloid[T] goes to ...
      if(!done[T]){
	double c1,c2; // Warning: result is returned in these arguments below!!!
	const int Tdash=YoungOrthogonalCoefficients(tau,T,c1,c2);
	if(Tdash==-1)
	  for(int i=T; i<=T+degree*(degree-1); i+=degree) array[i]=c1*array[i];
	else{
	  for(int i=0; i<degree; i++){
	    FIELD temp=M(T,i);
	    M(T,i)=c1*temp+c2*M(Tdash,i);
	    M(Tdash,i)=-c1*M(Tdash,i)+c2*temp;
	  }
	  done[Tdash]=1;
	}
      }
    }
  }
}



void Sn::Irreducible::applyTransposition(const int j, Matrix<FIELD >& M){
  bool done[degree]; 
  if(!YORComputed) computeYOR();

  for(int taupre=j; taupre<=2*n-2-j; taupre++){
    int tau=taupre;
    if(tau>n-1) tau=(n-1)-(tau-(n-1));
    for(int T=0; T<degree; T++) done[T]=0; 
    for(int T=0; T<degree; T++){ // tabloid[T] goes to ...
      if(!done[T]){
	double c1,c2; // Warning: result is returned in these arguments below!!!
	const int Tdash=YoungOrthogonalCoefficients(tau,T,c1,c2);
	if(Tdash==-1){
	  for(int i=0; i<degree; i++)
	    M(T,i)=c1*M(T,i);
	}
	else{
	  for(int i=0; i<degree; i++){
	    FIELD temp=M(T,i);
	    M(T,i)=c1*temp+c2*M(Tdash,i);
	    M(Tdash,i)=-c1*M(Tdash,i)+c2*temp;
	  }
	  done[Tdash]=1;
	}
      }
    }
  }
}




void Sn::Irreducible::applyTranspositionR(const int j, Matrix<FIELD >& M){
  bool done[degree]; 
  if(!YORComputed) computeYOR();

  for(int taupre=j; taupre<=2*n-2-j; taupre++){
    int tau=taupre;
    if(tau>n-1) tau=(n-1)-(tau-(n-1));
    for(int T=0; T<degree; T++) done[T]=0; 
    for(int T=0; T<degree; T++){ // tabloid[T] goes to ...
      if(!done[T]){
	double c1,c2; // Warning: result is returned in these arguments below!!!
	const int Tdash=YoungOrthogonalCoefficients(tau,T,c1,c2);
	if(Tdash==-1){
	  for(int i=0; i<degree; i++)
	    M(i,T)=c1*M(i,T);
	}
	else{
	  for(int i=0; i<degree; i++){
	    FIELD temp=M(i,T);
	    M(i,T)=c1*temp+c2*M(i,Tdash);    
	    M(i,Tdash)=-c1*M(i,Tdash)+c2*temp;
	  }
	  done[Tdash]=1;
	}
      }
    }
  }
}


////////////////////////////////////////////////////////////////////////////// MISC


string Sn::Irreducible::str(){
  ostringstream result;
  result<<partition.str();
  return result.str();
}

