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


#ifndef _SnFtree
#define _SnFtree

#include<sstream>
#include<vector>
#include<set>
#include<utility>

#include "Matrix.hpp"

#include "FiniteGroup.hpp"
#include "Sn.hpp"
#include "SnFunction.hpp"
#include "SnIrreducible.hpp"
#include "SnFourierTransform.hpp"

using namespace std;

class Sn::Ftree : FiniteGroup::Ftree{
public:

  Ftree(const Sn& _group, const int _left=-1, const int _right=-1);
  Ftree(const Sn& _group, const vector<int>& _Iindex, const int _left=-1, const int _right=-1);
  Ftree(const Sn* _group, const vector<int>& _Iindex, const vector<Ftree*>& _child);

  Ftree(const Function& f);
  Ftree(const FourierTransform& F, int l1, int l2, ...);
  Ftree(const FourierTransform& F, const vector<int>& _Iindex);

  ~Ftree();

  void FFT();
  void iFFT();

  FourierTransform* fourierTransform();
  Function* function();
  double max(vector<int>& result, double maxsofar);
  double norm2() const;

  void clean();
  void dirty();
  void deletedescendants();
  void copy(const Ftree& f);
  void tcopy(const Ftree& f);

  void scout(bool zerobottom=0);
  void unscout();
  void collect();
  void distribute();
  int component(const int rho) const;

  string str() const;
  void /*Sn::Ftree::*/printtree(const string indent) const;
  
private:

  void str_recurse(ostringstream& stream, Sn::Element L, Sn::Element R) const;
  Ftree(const Sn& _group, const int _left, const Function& f, const int offset);
  
public:
  
  int n;
  const Sn* group;
  int left;
  int right;
  bool protect;
  bool addto;

  vector<Ftree*> child;
  vector<int> Iindex;
  vector<Matrix<FIELD >*> matrix;
  

};

#endif

