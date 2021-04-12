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


#ifndef _StandardTableau
#define _StandardTableau

#include <vector>
#include <string>

class StandardTableau;

#include "Partition.hpp"


using namespace std;

// Standard tableau are Young tableau where the numerals in each row 
// and each column are increasing. The set of standard tableau of shape p 
// label the basis vectors of the irreducible representation of shape p. 


class StandardTableau : public vector<vector<int> >{
public:

  // StandardTableau(vector<vector<int> > _r); 
  // Initialize tableau with explicitly given entries 

  StandardTableau(const Partition& _p); 
  // Return default tableau of shape p with entries increasing numerically
  
  StandardTableau(int y1, ...);

  //~StandardTableau(); 

  // bool operator==(const StandardTableau& t) const {return r==t.r;} // Test for identical entries 
  // bool shapeequiv(const StandardTableau& t) const {return p==t.p;} // Test for identical shape 
  // bool shapeequiv(const Partition& _p) const {return p==_p;}       // Test if tableau is of shape _p

  Partition shape() const;
  
  StandardTableau& growTo(const Partition& p); 
  // Add the entry n+1 to the tableau in such a way that it reaches shape p.
  // Needed by Irreducible to generate basis from basis of subrepresentations. 

  bool applyTransposition(int t, int& distance);
  // Apply transposition (t,t+1) to tableau. If result is a standard tableau, return true. 
  // Change distance to the special signed distance needed for constructing 
  // Young's orthogonal representation. Result returned in argument!!!

  string str();


private:

  // int n;
  // int nrows;
  // Partition p;
  // vector<vector<int> > r; // The actual entries in the tableau

};

#endif
