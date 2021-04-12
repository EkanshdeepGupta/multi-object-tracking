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


#include "StandardTableau.hpp"

#include <sstream>
#include <stdarg.h>


StandardTableau::StandardTableau(const Partition& _p){
  // p=Partition(_p);
  // n=p.N();
  // nrows=_p.size();
  int c=1;
  for(int i=0; i<_p.size(); i++){
    vector<int> s; 
    for(int j=0; j<_p[i]; j++) s.push_back(c++);
    push_back(s);
  }
}


StandardTableau::StandardTableau(int y1, ...){
  va_list params;
  int y=y1;
  va_start(params,y1);
  int i=1;
  while(y!=0){
    for(int j=size()+1; j<=y; j++) 
      push_back(vector<int>());
    (*this)[y-1].push_back(i++);
    y=va_arg(params, int);
  }
  va_end(params);
}



Partition StandardTableau::shape() const{
  Partition result;
  for(int i=0; i<size(); i++)
    result.push_back((*this)[i].size());
  return result;
}



StandardTableau& StandardTableau::growTo(const Partition& p){
  int n=p.n();
  for(int i=0; i<size(); i++)
    if((*this)[i].size()+1==p[i]){
      (*this)[i].push_back(n);
      return *this;
    }
  vector<int> v; v.push_back(n);
  push_back(v);
  // nrows++;
  return *this;
}



bool StandardTableau::applyTransposition(int t, int& distance){
  int ai=-1; // Find t in tableau...
  int aj;
  for(int i=0; ai<0 && i<size(); i++)
    for(int j=0; j<(*this)[i].size(); j++)
      if((*this)[i][j]==t){ ai=i; aj=j; break;}

  int bi=-1; // Find t in tableau...
  int bj;
  for(int i=0; bi<0 && i<size(); i++)
    for(int j=0; j<(*this)[i].size(); j++)
      if((*this)[i][j]==t+1){ bi=i; bj=j; break;}
  
  distance=(bj-bi)-(aj-ai); // a very weird signed distance returned here...

  if( ((ai==bi)&&(aj+1==bj)) || ((aj==bj)&&(ai+1==bi)) ) return false; 
  // These are the only two ways that applying the transpisition could yield a non-standard tableau.
  else{(*this)[bi][bj]=t; (*this)[ai][aj]=t+1; return true;} // Corrected!
}



string StandardTableau::str(){
  ostringstream result;
  for(vector<vector<int> >::iterator it=begin(); it!=end(); it++){
    for(vector<int>::iterator it2=it->begin() ;it2!=it->end();it2++) result<<*it2<<" ";
    result<<endl;
  }
  return result.str();
}
