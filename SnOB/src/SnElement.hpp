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


#ifndef _SnElement
#define _SnElement

#include <vector>


#include "Sn.hpp"

class Sn::Element;

#include "Cycles.hpp"

using namespace std;

class Sn::Element: public FiniteGroup::Element{
public:

  Element(const Sn& _group):n(_group.n){
    p=new int[n]; pinv=new int[n]; for(int i=0; i<n; i++){p[i]=i+1; pinv[i]=i+1;}}

  Element(const int _n):n(_n){
    p=new int[n]; pinv=new int[n]; for(int i=0; i<n; i++){p[i]=i+1; pinv[i]=i+1;}}

  Element(int a1, int a2, ...);

  Element(const int _n, int* v):n(_n){
    p=new int[n]; pinv=new int[n];
    for(int i=0; i<n; i++) p[i]=v[i];
    for(int i=0; i<n; i++) pinv[p[i]-1]=i+1;}

  Element(const int _n, const vector<int> fixed);

  Element(const vector<int>& factorization, const int _n);

  Element(const Element& o);

  ~Element(){delete[] p; delete[] pinv;}

  bool operator==(const Sn::Element& o);

  int action(const int i) const {return pinv[i-1];} 
  int iaction(const int i) const {return p[i-1];}

  vector<int> effect() const {vector<int> result; for(int i=0; i<n; i++) result.push_back(pinv[i]); return result;}
  vector<int> ieffect() const {vector<int> result; for(int i=0; i<n; i++) result.push_back(p[i]); return result;}

  Element* operator*(const Sn::Element& o) const {
    Element* result=new Element(n);
    for(int i=0; i<n; i++) result->pinv[i]=pinv[o.pinv[i]-1];
    for(int i=0; i<n; i++) result->p[result->pinv[i]-1]=i+1;
    return result;
  }

  Element* inverse(){return new Element(n,pinv);}

  Element& CcycleL(int j, int q);
  Element& CcycleR(int j, int q);

  string str() const {ostringstream result; result<<"[ "; for(int i=0; i<n; i++) result<<p[i]<<" "; result<<"]"; return result.str();}

  int n;

  private:
  
  int* p; // p=[\sigma^{-1}(1),....,\sigma^{-1}(n)]
  int* pinv;

};

#endif
