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


#ifndef _Irreducible
#define _Irreducible

#include <string>

#include "Sn.h"
#include "Matrix.hpp"
#include "Partition.hpp"
#include "StandardTableau.hpp"
#include "SnElement.hpp"
#include "Sn.hpp"

using namespace std;

class Sn::Irreducible: public FiniteGroup::Irreducible{
public:

  Irreducible(const Sn* _group, const Partition& _partition); 

  ~Irreducible(){/*delete tdash[]; delete coeff1[]; delete coeff2[];*/}
  // why does this seg-fault???? because there were no []'s try again!

  Matrix<FIELD >* rho(const Sn::Element& p);
  // Return the representation matrix correspoding to group element p. 
  // The representation is given in terms of Young's orthogonal basis. 

  FIELD character(const Partition& p);

  void applyTransposition(const int j,Matrix<FIELD >& M);
  void applyTranspositionR(const int j,Matrix<FIELD >& M);
  void applyCycleL(const int j,Matrix<FIELD >& M, int m=-1, bool inverse=0);
  void applyCycleR(const int j,Matrix<FIELD >& M, int m=-1, bool inverse=0);

  bool tableauxComputed;
  StandardTableau* tableau(const int T) const;
  // List of standard tableaux for this representation. 
  // Their index is defined by their order in this list. 
  void computeTableaux();

  bool YORComputed;
  int YoungOrthogonalCoefficients(const int tau, const int T, double& c1, double& c2) const;
  // Return precomputed information on how the transposition (tau,tau+1) acts on tableau T. 
  // The result is either a linear combination of T and another tableau Tdash (whose 
  // index is returned by the function) or just a multiple of T. In the latter case, 
  // the function returns -1. All tableaux are indexed according to their index in 
  // the vector tableaux below. Results for c1 and c2 returned in argument!!!
  void computeYOR();

  string str();

  Partition partition;
  // The shape of this representation. 

  int degree; 
  // dimensionality of representation, i.e., number of standard tableaux 

  vector<int> etaindex;
  // Indices of irreducible representations of S_{n-1} which sum to this 
  // irreducible representation. 

  vector<Irreducible*> eta;
  // Pointers to irreducible representations of S_{n-1} which sum to this 
  // irreducible representation. 


private:

  int n;
  const Sn* group;
  vector<StandardTableau> tableauV;
  int* tdash; 
  double* coeff1; 
  double* coeff2; 

};


#endif
