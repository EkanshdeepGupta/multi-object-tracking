#ifndef _Sn
#define _Sn

#include <sstream>
#include <set>

#include "FiniteGroup.hpp"

class Sn;

class Partition; 

using namespace std;

class Sn: public FiniteGroup{
public:
  class Element;
  class Irreducible; 
  class Function;
  class FourierTransform;
  class PartialFT;
  class Ftree;
  class Association;

  Sn(const int _n);
  ~Sn();

  Element* operator[](const int i) const; 

  Irreducible* irreducible(const Partition& p, int& index); 

  void branching(const vector<int>& rhos, vector<int>& result) const;

  string str(); 

  const int n;
  Sn* subgroup; 
  vector<Irreducible*> irreducibles; 

};


#endif
