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


#include "SnElement.hpp"
#include "SnIrreducible.hpp"

#include <sstream>

Sn::Sn(const int _n):n(_n){
  
  if(n==1){ // bottom of recursive sequence ...
    Partition lambda;
    lambda.push_back(1);
    irreducibles.push_back(new Irreducible(this, lambda));
    order=1;
    return;
  }

  subgroup=new Sn(n-1);
  
  for(int i=0; i<subgroup->irreducibles.size(); i++){
    Partition lambda(subgroup->irreducibles[i]->partition);
    if(lambda.size()==1 || lambda[lambda.size()-1]<lambda[lambda.size()-2]){
      lambda[lambda.size()-1]++;
      irreducibles.push_back(new Irreducible(this, lambda));
      lambda[lambda.size()-1]--;
    }
    lambda.push_back(1);
    irreducibles.push_back(new Irreducible(this, lambda));
  }  

  order=n*subgroup->order;
}



Sn::~Sn(){
  for(int i=0; i<irreducibles.size(); i++) delete irreducibles[i];
  if(n>1) delete subgroup; 
}



Sn::Element* Sn::operator[](const int perm) const{
  int v[n];
  for(int i=1; i<=n; i++) v[i-1]=i;
  int p=perm;
  for(int k=2; k<=n; k++){
    int res=p%k;
    p=(p-res)/k;
    int j=k-res;
    int t=v[k-1];
    for(int i=k-1; i>=j; i--) v[i+1-1]=v[i-1];
    v[j-1]=t;
  }
  //  int suborder=1;
  //for(int i=1; i<n; i++){
  //  suborder=suborder*i;
  //  int m=((int)(perm/suborder))%(i+1);
  //  int t=v[i];
  //  v[i]=v[i-m];
  //  v[i-m]=t;
  //}
  return new Element(n,v);
} 



Sn::Irreducible* Sn::irreducible(const Partition& p, int& index){
  index=0;
  for(vector<Irreducible*>::iterator it=irreducibles.begin(); it!=irreducibles.end(); it++){
    if((*it)->partition==p) return &(**it); // very careful here!!!!
    index++;
  }
}




string Sn::str(){
  ostringstream result;
  result<<"S("<<n<<")";
  return result.str();
}



void Sn::branching(const vector<int>& rhos, vector<int>& result) const{
  set<int> resultset;
  for(int i=0; i<rhos.size(); i++){
    const vector<int>* etaindex=&(irreducibles[rhos[i]]->etaindex);
    for(int eta=0; eta<etaindex->size(); eta++) resultset.insert((*etaindex)[eta]);
  }
  for(set<int>::iterator it=resultset.begin(); it!=resultset.end(); it++) result.push_back(*it);
}
