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


#include "SnFtree.hpp"

#include <algorithm>
#include <stdarg.h>

Sn::Ftree::~Ftree(){
  clean();
  deletedescendants();
}



Sn::Ftree::Ftree(const Sn& _group, const int _left, const int _right):
  group(&_group), n(_group.n), left(_left), right(_right), protect(0),addto(0){
  if(left==-1) left=n+1;
  if(right==-1) right=n+1;
  //handedness=0;
}


Sn::Ftree::Ftree(const Sn& _group, const vector<int>& _Iindex, const int _left, const int _right):
  group(&_group), n(_group.n), left(_left), right(_right), Iindex(_Iindex), protect(0),addto(0){
  if(left==-1) left=n+1;
  if(right==-1) right=n+1;
  //handedness=0;
}



Sn::Ftree::Ftree(const Sn* _group, const vector<int>& _Iindex, const vector<Ftree*>& _child):
  group(_group), n(_group->n), Iindex(_Iindex),protect(0),addto(0),
  child(_child){
  left=n+1; right=n+1;
  //handedness=0;
}



Sn::Ftree::Ftree(const Function& f):
  group(f.group), n(f.group->n), left(0), right(0), protect(0), addto(0){
  const int suborder=group->order/n;
  if(n>1)
    for(int i=1; i<=n; i++){
      Sn::Ftree* subtree=new Sn::Ftree(*group->subgroup,i,f,(n-i)*suborder);
      if(subtree->child.size()>0 || subtree->matrix.size()>0) child.push_back(subtree);
      else delete subtree;
    }
  else 
    if(f.f[0]!=0){
      Iindex.push_back(0);
      matrix.push_back(new Matrix<FIELD>(1,1,f.f[0]));
    }
}



Sn::Ftree::Ftree(const Sn& _group, const int _left, const Function& f, const int offset):
  group(&_group), n(_group.n), left(_left), right(_group.n+1), protect(0),addto(0){
  const int suborder=group->order/n;
  if(n>1)
    for(int i=1; i<=n; i++){
      Sn::Ftree* subtree=new Sn::Ftree(*group->subgroup,i,f,offset+(n-i)*suborder);
      if(subtree->child.size()>0 || subtree->matrix.size()>0) child.push_back(subtree);
      else delete subtree;
    }
  else 
    if(f.f[offset]!=0){
      Iindex.push_back(0);
      matrix.push_back(new Matrix<FIELD>(1,1,f.f[offset]));
    }
}


Sn::Ftree::Ftree(const FourierTransform& F, int l1, int l2, ...):
  group(F.group), n(F.group->n), left(0), right(0), protect(0), addto(0){
  Iindex.push_back(l1);
  int arg=l2;
  va_list params;
  va_start(params,l2);
  while(arg!=0){
    Iindex.push_back(arg);
    arg=va_arg(params,int);
  }
  for(int i=0; i<Iindex.size(); i++)
    matrix.push_back(new Matrix<FIELD>(*F.matrix[Iindex[i]]));
}



Sn::Ftree::Ftree(const FourierTransform& F, const vector<int>& _Iindex):
  group(F.group), n(F.group->n), left(0), right(0), Iindex(_Iindex), protect(0), addto(0){
  for(int i=0; i<Iindex.size(); i++)
    matrix.push_back(new Matrix<FIELD>(*F.matrix[Iindex[i]]));  
}



void Sn::Ftree::clean(){
  for(int i=0; i<matrix.size(); i++) delete matrix[i];
  matrix.clear();
}



void Sn::Ftree::dirty(){
  clean();
  for(int rhoix=0; rhoix<Iindex.size(); rhoix++){
    int degree=group->irreducibles[Iindex[rhoix]]->degree;
    matrix.push_back(new Matrix<FIELD >(degree,degree,0));
  }
}



void Sn::Ftree::deletedescendants(){
  for(int i=0; i<child.size(); i++) delete child[i];
  child.clear();
}



void Sn::Ftree::copy(const Ftree& f){
  clean();
  for(int rhoix=0; rhoix<Iindex.size(); rhoix++){
    int rhoixd=f.component(Iindex[rhoix]);
    if(rhoixd>=0) matrix.push_back(new Matrix<FIELD >(*f.matrix[rhoixd]));
    else{
      int degree=group->irreducibles[Iindex[rhoix]]->degree;
      matrix.push_back(new Matrix<FIELD >(degree,degree,0));
    }
  }
}



void Sn::Ftree::tcopy(const Ftree& f){  // Very dangerous!!!!
  clean();
  for(int rhoix=0; rhoix<Iindex.size(); rhoix++){
    int rhoixd=f.component(Iindex[rhoix]);
    if(rhoixd>=0) matrix.push_back(f.matrix[rhoixd]);
    else{
      int degree=group->irreducibles[Iindex[rhoix]]->degree;
      matrix.push_back(new Matrix<FIELD >(degree,degree,0));
    }
  }
}



/////////////////////////////////////////////////////////////////////////////////////////////// SCOUTING


void Sn::Ftree::scout(const bool zerobottom){ // fill in which irreducibles need to be computed
  if(child.size()>0){

    for(vector<Ftree*>::iterator kid=child.begin(); kid!=child.end(); kid++){ // down sweep...
      if((*kid)->child.size()>0 || zerobottom){
	group->branching(Iindex,(*kid)->Iindex);
	(*kid)->scout(zerobottom);
      }
    }

    if(!zerobottom && !protect){
      vector<int> newIindex;
      for(int rhoix=0; rhoix<Iindex.size(); rhoix++){ // up sweep...
	const Sn::Irreducible* rho=group->irreducibles[Iindex[rhoix]];
	bool found=0;
	for(int etai=0; etai<rho->etaindex.size(); etai++){
	  int etaindex=rho->etaindex[etai];
	  for(vector<Ftree*>::iterator kid=child.begin(); kid!=child.end(); kid++)
	    if((*kid)->component(etaindex)>=0){found=1;break;}
	  if(found) break;
	}
	if(found) newIindex.push_back(Iindex[rhoix]);
      }  
      Iindex=newIindex;
    }
  }
}

  

void Sn::Ftree::unscout(){
  for(vector<Ftree*>::iterator kid=child.begin(); kid!=child.end(); kid++){
    if((*kid)->child.size()>0){
      (*kid)->Iindex.clear();
      (*kid)->unscout();
    }
  }
}



////////////////////////////////////////////////////////////////////////////////////////// COLLECT & DISTRIBUTE



void Sn::Ftree::collect(){ // Recursive function for moving up the tree by FT, depth first

  for(int kidix=0; kidix<child.size(); kidix++){
    Ftree& kid=*child[kidix];
    if(kid.child.size()>0){
      // group->branching(Iindex,kid.Iindex); // no scouting needed WHY NOT?????
      kid.collect();
    }
  }

  if(!addto){
    clean();
    dirty();
  }

  //cout<<"Collect level "<<n<<", irreducibles: ";
  //for(int i=0; i<Iindex.size(); i++) cout<<Iindex[i]<<" ";
  //cout<<endl;

  for(int rhoix=0; rhoix<Iindex.size(); rhoix++){
    Sn::Irreducible* rho=group->irreducibles[Iindex[rhoix]];
    const int degree=rho->degree;
    //Matrix<FIELD >* M=matrix[rhoix];
      //new Matrix<FIELD >(degree,degree,0);
 
    for(int kidix=0; kidix<child.size(); kidix++){
      Ftree& kid=*child[kidix]; 

      bool nonzero=0; // check for case that kid does not contribute at all
      vector<int> subix;
      for(int etai=0; etai<rho->etaindex.size(); etai++){
	int s=kid.component(rho->etaindex[etai]);
	subix.push_back(s);
	if(s>=0) nonzero=1;
      }

      if(nonzero){
	Matrix<FIELD > tildef(degree,degree);
	int offset=0;
	int completed=0;
	for(int etai=0; etai<rho->etaindex.size(); etai++){
	  int subdegree=group->subgroup->irreducibles[rho->etaindex[etai]]->degree;
	  if(subix[etai]>=0){
	    FIELD* mx=kid.matrix[subix[etai]]->array;
	    FIELD* mx2=tildef.array;
	    for(int i=0; i<subdegree; i++){
	      for(int j=0; j<offset; j++) mx2[completed+j]=0;
	      for(int j=0; j<subdegree; j++) mx2[completed+offset+j]=mx[i*subdegree+j];
	      for(int j=offset+subdegree; j<degree; j++) mx2[completed+j]=0;
	      completed+=degree;
	    }
	  }
	  else{
	    for(int i=0;i<degree*subdegree;i++) tildef.array[completed+i]=0;
	    completed+=degree*subdegree;
	  }
	  offset+=subdegree;
	}
	rho->applyTransposition(kid.left,tildef);
	rho->applyTranspositionR(kid.right,tildef);
	//if(!handedness) rho->applyTransposition(kid.left,tildef);
	//else rho->applyTranspositionR(kid.right,tildef);
	(*matrix[rhoix])+=tildef;
      }

    }

    //if(handedness) rho->applyTransposition(child[0]->left,*M);
    //else rho->applyTranspositionR(child[0]->right,*M);
    //matrix.push_back(M);
  }
    
  for(vector<Ftree*>::iterator kid=child.begin(); kid!=child.end(); kid++)
    if((*kid)->child.size()>0) (*kid)->clean();
}



void Sn::Ftree::distribute(){ // Recursive fn for moving down the tree by iFT, depth first 

  //  cout<<"Distribute level "<<n<<", irreducibles: ";
  //for(int i=0; i<Iindex.size(); i++) cout<<Iindex[i]<<" ";
  //cout<<endl;

  for(int kidix=0; kidix<child.size(); kidix++){
    Ftree& kid=*child[kidix];

    kid.dirty();

    for(int rhoindex=0; rhoindex<Iindex.size(); rhoindex++){
      Sn::Irreducible* rho=group->irreducibles[Iindex[rhoindex]];

      bool nonzero=0; // check for case that kid does not get anything at all
      vector<int> subix;
      for(int etai=0; etai<rho->etaindex.size(); etai++){
	int s=kid.component(rho->etaindex[etai]);
	subix.push_back(s);
	if(s>=0) nonzero=1;
      }
	    
      if(nonzero){
	Matrix<FIELD > M(*matrix[rhoindex]); 
	rho->applyTransposition(kid.left,M); // could be more efficient with handedness. will take engineering
	rho->applyTranspositionR(kid.right,M);
	int offset=0;
	for(int etai=0; etai<rho->etaindex.size(); etai++){
	  if(subix[etai]>=0){
	    Matrix<FIELD >& Msub=*kid.matrix[subix[etai]];
	    const int degree=Msub.n;
	    FIELD multiplier=FIELD(double(rho->degree)/double(degree*n));
	    for(int a=0; a<degree; a++)
	      for(int b=0; b<degree; b++)
		Msub(a,b)+=M(offset+a,offset+b)*multiplier; // operate directly on array for speed!
	  }
	  offset+=group->subgroup->irreducibles[rho->etaindex[etai]]->degree;
	}
      }
    }

    if(kid.child.size()>0) kid.distribute(); else{
      //cout<<"Leaf level "<<kid.n<<", irreducibles: ";
      //for(int i=0; i<kid.Iindex.size(); i++) cout<<kid.Iindex[i]<<" ";
      //cout<<endl;
    }

  }
  if(!protect) clean();
}



////////////////////////////////////////////////////////////////////////////////////////////// FFTs

void Sn::Ftree::FFT(){
  clean();
  scout();
  collect();
  unscout();
}



void Sn::Ftree::iFFT(){
  scout();
  distribute();
  unscout();
}



///////////////////////////////////////////////////////// interface to Function and FourierTransform  



Sn::FourierTransform* Sn::Ftree::fourierTransform(){
  Sn::FourierTransform* result=new Sn::FourierTransform(*group);
  return result;
};



Sn::Function* Sn::Ftree::function(){
  Sn::Function* result=new Sn::Function(*group);
  return result;
};




////////////////////////////////////////////////////////////////////////////////////////////// MISC



int Sn::Ftree::component(const int rho) const{
  for(int i=0; i<Iindex.size(); i++)
    if (Iindex[i]==rho) return i;
  return -1;
}



double Sn::Ftree::norm2() const{
  double result=0;
  for(int i=0; i<matrix.size(); i++) result+=matrix[i]->norm2();
  return result;
}



bool paircomparison(pair<int,double> a, pair<int,double> b){
  return(a.second>b.second);
}


double Sn::Ftree::max(vector<int>& result, double maxsofar){ 

  if(n==1){
    if(norm2()>maxsofar){
      maxsofar=norm2();
      result.clear();
      result.push_back(1);
    }
    return maxsofar;
  }

  vector<int> subIindex;
  group->branching(Iindex,subIindex);
  for(int i=0; i<n; i++) child.push_back(new Ftree(*group->subgroup, subIindex,i+1));
  distribute();

  vector<pair<int, double> > norms;
  for(int i=0; i<n; i++) norms.push_back(pair<int, double>(i,child[i]->norm2()));
  sort(norms.begin(), norms.end(), paircomparison);

  for(int i=0; i<n; i++){
    vector<int> subresult;
    maxsofar=child[norms[i].first]->max(subresult,maxsofar);
    if(subresult.size()>0){
      //result=subresult;
      result.clear(); // inneficient!!!
      result.push_back(norms[i].first+1);
      for(int j=0; j<subresult.size(); j++) result.push_back(subresult[j]);
    }
    if(norms[i].second<=maxsofar) break;
  }

  deletedescendants(); 
  return maxsofar;
}


void Sn::Ftree::str_recurse(ostringstream& stream, Sn::Element L, Sn::Element R) const{
  if(matrix.size()){
    if(n>1){
      Sn::Element e(L.n);
      if(!(L==e && R==e)) stream<<"Coset "<<L.str()<<" S_"<<n<<" "<<R.str()<<endl<<endl;;
      for (int i=0; i<matrix.size(); i++)
	stream<<group->irreducibles[Iindex[i]]->partition.str()<<endl<<matrix[i]->str()<<endl;
    }else{
      Sn::Element* sigma=L*R;
      stream<<sigma->str()<<" : "<<matrix[0]->at(0,0)<<endl;
      delete sigma;
    }
  }else{
    for(int i=0; i<child.size(); i++)
      child[i]->str_recurse(stream,Sn::Element(L).CcycleR(child[i]->left,n),Sn::Element(R).CcycleL(child[i]->right,n));
  }
}

string Sn::Ftree::str() const {
  ostringstream result;
  str_recurse(result,Sn::Element(*group),Sn::Element(*group));
  return result.str(); 
}


void Sn::Ftree::printtree(const string indent) const{
  cout<<indent<<"("<<left<<","<<right<<")"<<endl;
  for(int i=0; i<matrix.size(); i++)
    cout<<matrix[i]->str();
  for(int i=0; i<child.size(); i++)
    child[i]->printtree(indent+"  ");
}
