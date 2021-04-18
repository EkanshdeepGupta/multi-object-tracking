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
#include <stdarg.h>



Sn::Element::Element(const Sn::Element& o):
  n(o.n){
  p=new int[n]; for(int i=0; i<n; i++) p[i]=o.p[i];
  pinv=new int[n]; for(int i=0; i<n; i++) pinv[i]=o.pinv[i];
}



Sn::Element::Element(int a1, int a2, ...){
  va_list params;
  int arg=a2;
  vector<int> v;
  v.push_back(a1);
  va_start(params,a2);
  while(arg!=0){
    v.push_back(arg);
    arg=va_arg(params, int);
  }
  va_end(params);
  n=v.size();
  p=new int[n]; pinv=new int[n];
  for(int i=0; i<n; i++) p[i]=v[i];
  for(int i=0; i<n; i++) pinv[p[i]-1]=i+1;
}


// Creates a coset representative
Sn::Element::Element(const int _n, const vector<int> fixed):n(_n){
  p=new int[n]; pinv=new int[n];
  for(int i=0; i<n; i++) p[i]=i+1;
  for(int j=0; j<fixed.size(); j++){
    for(int i=0; i<n; i++)
      if(p[i]==fixed[j]){
	p[i]=p[n-1-j];
	p[n-1-j]=fixed[j];
	break;
      }
  }
  for(int i=0; i<n; i++) pinv[p[i]-1]=i+1;
}


Sn::Element::Element(const vector<int>& factorization, const int _n):n(_n){
  p=new int[n]; pinv=new int[n];
  for(int i=0; i<n; i++) p[i]=i+1;
  for(int j=0; j<factorization.size(); j++){
    // cout<<":"<<factorization[j];
    int t=p[n-1-j];
    p[n-1-j]=p[factorization[j]-1];
    p[factorization[j]-1]=t;
  }
  cout<<endl;
  for(int i=0; i<n; i++) pinv[p[i]-1]=i+1;
}



bool Sn::Element::operator==(const Sn::Element& o){
  if(n!=o.n) return 0;
  for(int i=0; i<n; i++)
    if(p[i]!=o.p[i]) return 0;
  return 1;
}



Sn::Element& Sn::Element::CcycleL(int j, int q){
  int t=p[q-1];
  for(int i=j+1; i<=q; i++) p[i-1]=p[i-2];
  p[j-1]=t;
  for(int i=0; i<n; i++) pinv[p[i]-1]=i+1;
  return *this;
}



Sn::Element& Sn::Element::CcycleR(int j, int q){
  int t=pinv[j-1];
  for(int i=j; i<=q-1; i++) pinv[i-1]=pinv[i+1-1];
  pinv[q-1]=t;
  for(int i=0; i<n; i++) p[pinv[i]-1]=i+1;
  return *this;
}


