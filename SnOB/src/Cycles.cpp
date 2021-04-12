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


#include"Cycles.hpp"

#include <sstream>



Cycles::Cycles(const Sn::Element& p){
  const int n=p.n;
  bool* flag=new bool[n];
  for(int i=0; i<n; i++) flag[i]=false;
  for(int i=0; i<n; i++){
    if(flag[i]) continue;
    vector<int> cyc;
    cyc.push_back(i+1);
    for(int j=p.action(i+1)-1; j!=i; j=p.action(j+1)-1){
      cyc.push_back(j+1);
      flag[j]=true;
    }
    push_back(cyc);
  }
  delete flag;
}



string Cycles::str(){
  ostringstream result;
  for(vector<vector<int> >::iterator it=begin(); it!=end(); it++){
    result<<"(";
    vector<int>::iterator it1=it->begin();
    if(it1!=it->end()){
      result<<*it1++;
      for(; it1!=it->end(); it1++) result<<","<<*it1;
    }
    result<<")";
  }
  return result.str();
}
