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


#ifndef _FourierTransform
#define _FourierTransform

#include <vector>

#include "Sn.h"
#include "Matrix.hpp"
#include <sstream>
#include "Sn.hpp"
#include "SnFunction.hpp"
#include "StandardTableau.hpp"

using namespace std;

class Sn::FourierTransform: FiniteGroup::FourierTransform{

public:

  friend class Sn::Function;
  friend class Sn::Ftree;

  FourierTransform(const Sn& _group);
  FourierTransform(const Sn& _group, int dummy):group(&_group),n(_group.n){};
  FourierTransform(const Sn& _group, const vector<Matrix<FIELD >*> matrices);
  FourierTransform(const Function& f);
  ~FourierTransform();

  Function* iFFT() const;

  FIELD operator()(const StandardTableau& t1, const StandardTableau& t2) const;

  double norm2() const {double result; for(int i=0; i<matrix.size(); i++) result+=1; return result;}

  string str() const;

  vector<Matrix<FIELD >*> matrix;


private: 

  void fft(const Sn::Function& f, const int offset);
  void ifft(Sn::Function* target, const int _offset) const;

  const int n;
  const Sn* group;

};

#endif
