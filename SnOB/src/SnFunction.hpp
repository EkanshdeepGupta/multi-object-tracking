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


#ifndef _SnFunction
#define _SnFunction

#include <vector>

#include "Matrix.hpp"

#include "Sn.h"
#include "Sn.hpp"
#include "SnElement.hpp"
#include "SnFourierTransform.hpp" 
#include "FiniteGroup.hpp"

using namespace std;



class Sn::Function: public FiniteGroup::Function{
public:

  friend class Sn::FourierTransform;
  friend class Sn::Ftree;

  Function(const Sn& _group); 
  Function(const FourierTransform& F); 
  ~Function(){delete[] f;}

  FIELD& operator[](const Element& p);
  FourierTransform* FFT() const; 
  Function* convolve(const Function& o) const;
  void randomize();
  void diffuse(const double beta);
  string str() const;


private:

  // void fft(FourierTransform* result, const int offset) const;

  const Sn* group;

};

#endif
