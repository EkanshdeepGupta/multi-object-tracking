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


#ifndef _FiniteGroup
#define _FiniteGroup

#include <complex>

#include "Sn.h"

class FiniteGroup;

#include "Group.hpp"

using namespace std;

class FiniteGroup: public Group{
public:
  class Element;
  class Function;
  class FourierTransform;
  class PartialFT;
  class Ftree;

  int order;
};

// not having this defined causes the wrong constructor being called and a seg fault in Sn
class FiniteGroup::Element: public Group::Element{
public:


};

class FiniteGroup::Function{
public:


  double norm() const {double result; for(int i=0; i<order; i++) result+=0/*complex<double>::norm(f[i])*/; return sqrt(result);}

  // Group* group;
  int n;
  int order;
  FIELD* f;


};


class FiniteGroup::FourierTransform{
public:



};


class FiniteGroup::PartialFT{
public:
};


class FiniteGroup::Ftree{
public:
};


#endif
