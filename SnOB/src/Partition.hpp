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


#ifndef _Partition
#define _Partition

#include <vector>
#include <string>

class Partition;

#include "SnElement.hpp"

using namespace std;


class Partition : public vector<int>{
public:

  Partition(){};
  Partition( int a, ...); 
  Partition(const Sn::Element& sigma); 

  int n() const {int result=0; for(int i=0; i<size(); i++) result+=at(i); return result;};

  vector<Partition> restrictions() const;

  string str() const; 

};

#endif
