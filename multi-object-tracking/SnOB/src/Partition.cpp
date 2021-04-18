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


#include "Partition.hpp"

#include <iostream>
#include <sstream>
#include <stdarg.h>

Partition::Partition(int a, ... ){
  va_list params;
  int p=a;
  va_start(params,a);
  while(p!=0){
    push_back(p);
    p=va_arg(params, int);
  }
  va_end(params);
}


vector<Partition> Partition::restrictions() const{
  vector<Partition> result;
  for(int i=0; i<size(); i++){
    if(i==size()-1 || at(i)>at(i+1)){
      Partition p(*this); p[i]--;
      if(p[i]==0) p.pop_back();
      result.push_back(p);
    }
  }
  return result;
}


string Partition::str() const{
  ostringstream result;
  result<<"("<<at(0);
  for(int i=1; i<size();i++)
    result<<","<<at(i);
  return result.str()+")";
}


