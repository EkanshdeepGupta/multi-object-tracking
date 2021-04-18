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


#ifndef _Matrix
#define _Matrix

#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include "Sn.h"

using namespace std;

template <class TYPE>
class Matrix{
public:

  Matrix(const int _n):n(_n),m(_n)
  {array=new TYPE[n*m]; for(int i=0; i<n*m; i++) if(i%(n+1)==0) array[i]=1; else array[i]=0;}

  Matrix(const int _n, const int _m):n(_n),m(_m){array=new TYPE[n*m];};

  Matrix(const int _n, const int _m, const TYPE a):n(_n),m(_m)
  {array=new TYPE[n*m]; for(int i=0; i<n*m; i++) array[i]=a;};

  Matrix(const TYPE** a, const int _n, const int _m):n(_n),m(_m){
    array=new TYPE[n*m];
    for(int i=0; i<n; i++)
      for(int j=0; j<m; j++)
	array[i*m+j]=a[i][j];
  }

  Matrix(const Matrix<TYPE>& o):n(o.n),m(o.m)
  {array=new TYPE[n*m]; for(int i=0; i<n*m; i++) array[i]=o.array[i];};

  Matrix(const int _n, const vector<Matrix<TYPE>*>& mlist);
  // Construct matrix as direct sum of square matrices 
  
  Matrix(const string filename);

  ~Matrix(){delete[] array;}

  TYPE& at(const int i, const int j){
    return array[i*m+j];};

  TYPE& operator()(const int i, const int j){
    return array[i*m+j];};

  Matrix<TYPE>* operator*(const TYPE& o);
  Matrix<TYPE>* operator*(Matrix<TYPE>& o);
  Matrix<TYPE>* operator+(const Matrix<TYPE>& o);

  Matrix<TYPE>& operator+=(const Matrix<TYPE>& o);
  Matrix<TYPE>& operator*=(const TYPE& mul);

  Matrix<TYPE>& operator=(const Matrix<TYPE>& o);

  Matrix<TYPE>& fill(const TYPE& x){ for(int i=0; i<n*m; i++) array[i]=x; }

  // Matrix<TYPE>* tensorproduct(const Matrix<TYPE>& o) const;
  // Matrix<TYPE>* tensorproduct(const int tn, const int tm) const;

  TYPE trace() const{TYPE t=0; for(int i=0; i<n; i++) t+=array[i*(n+1)]; return t;};
  // TYPE schur2(const Matrix<TYPE>& o) const;
  TYPE norm2() const {double result; for(int i=0; i<n*m; i++) result+=array[i]*array[i]; return result;}


  Matrix<TYPE>* pow(const int p);

  string str() const;

  int save(const string filename);

  //private: (const removed!)
  int n;
  int m;

  TYPE* array;

};


template <class TYPE>
Matrix<TYPE>::Matrix(const int _n, const vector<Matrix<TYPE>*>& mlist):n(_n),m(_n){
  array=new TYPE[n*m];
  const int Nsummands=mlist.size();
  int total=0;
  for(int i=0; i<Nsummands; i++) total+=mlist[i]->n;
  int filled=0; 
  for(int summand=0; summand<Nsummands; summand++){
    const int size=mlist[summand]->n;
    const int offset=filled*m;
    for(int i=0; i<size; i++){
      for(int j=0; j<filled; j++) array[offset+i*m+j]=0;
      for(int j=0; j<size; j++) array[offset+i*m+filled+j]=mlist[summand]->at(i,j);
      for(int j=filled+size; j<m; j++) array[offset+i*m+j]=0;
    }
    filled+=size; 
  }
}


template <class TYPE>
string Matrix<TYPE>::str() const{
  ostringstream result; 
  result.precision(STR_PRECISION);
  for(int i=0; i<n; i++){
    for(int j=0; j<m; j++) 
      result<<array[i*m+j]<<" ";
      result<<"\n";}
  return result.str();
}

template <class TYPE>
Matrix<TYPE>* Matrix<TYPE>::operator*(const TYPE& o){
  Matrix* result;
  result=new Matrix(n,m);
  for(int i=0; i<n; i++)
    for(int j=0; j<m; j++)
      result->at(i,j)=o*at(i,j);
  return result;
}

template <class TYPE>
Matrix<TYPE>* Matrix<TYPE>::operator*(Matrix<TYPE>& o){
  Matrix<TYPE>* result=new Matrix(n,o.m);
  for(int i=0; i<n; i++)
    for(int j=0; j<o.m; j++){
      TYPE t=0;
      for(int k=0; k<m; k++)
	t+=at(i,k)*o.at(k,j);
      result->at(i,j)=t;
    }
  return result;
}

template <class TYPE>
Matrix<TYPE>* Matrix<TYPE>::operator+(const Matrix<TYPE>& o){
  Matrix* result;
  result=new Matrix(n,m);
  for(int i=0; i<n; i++)
    for(int j=0; j<m; j++)
      result->at(i,j)=o.at(i,j)+at(i,j);
  return result;
}


template <class TYPE>
Matrix<TYPE>& Matrix<TYPE>::operator+=(const Matrix<TYPE>& o){
  for(int i=0; i<n*m; i++) array[i]+=o.array[i];
  return *this;
}

template <class TYPE>
Matrix<TYPE>& Matrix<TYPE>::operator=(const Matrix<TYPE>& o){
  for(int i=0; i<n*m; i++) array[i]=o.array[i];
  return *this;
}

template <class TYPE>
Matrix<TYPE>& Matrix<TYPE>::operator*=(const TYPE& mul){
  for(int i=0; i<n*m; i++) array[i]*=mul;
  return *this;
}

/*
template <class TYPE>
Matrix<TYPE>* Matrix<TYPE>::tensorproduct(const Matrix<TYPE>& o) const{
  Matrix<TYPE>* result=new Matrix<TYPE>(n*o.n,m*o.m);
  const int tt=m*o.m;
  for(int a=0; a<o.n; a++)
    for(int b=0; b<o.m; b++){
      const TYPE t=o.array[a*m+b];
      for(int i=0; i<n; i++)
	for(int j=0; j<m; j++)
	  result->array[(i+n*a)*tt+j+m*b]=t*array[i*m+j];
    }
  return result;
}


template <class TYPE>
Matrix<TYPE>* Matrix<TYPE>::tensorproduct(const int tn, const int tm) const{
  Matrix<TYPE>* result=new Matrix<TYPE>(n*tn,m*tm);
  const int tt=m*tm;
  for(int a=0; a<tn; a++)
    for(int b=0; b<tm; b++)
      for(int i=0; i<n; i++)
	for(int j=0; j<m; j++)
	  result->array[(i+n*a)*tt+j+m*b]=array[i*m+j];
  return result;
}



template <class TYPE>
TYPE Matrix<TYPE>::schur2(const Matrix<TYPE>& o) const{
  TYPE result;
  for(int i=0; i<n; i++) 
    for(int j=0; j<m; j++)
      result+=array[i*m+j]*o.array[j*n+i];
  return result;
}
*/



template <class TYPE>
Matrix<TYPE>* Matrix<TYPE>::pow(const int p){
  if(p<2) return new Matrix<TYPE>(*this);
  Matrix<TYPE>* result=(*this)*(*this);
  for(int i=2; i<p; i++){
    Matrix<TYPE>* t=(*this)*(*result);
    delete result;
    result=t;
  }
  return result;
}



template <class TYPE>
Matrix<TYPE>::Matrix(const string filename){
  cout<<"Loading "<<filename<<" ..."<<endl;
  ifstream ifs(filename.c_str());
  n=0;m=0;
  TYPE b;
  while(ifs.peek()!='\n'){
    ifs>>b; m++;
  }
  while(ifs.good()){
    for(int i=0; i<m;i++) ifs>>b;
    n++;
  }
  array=new TYPE[n*m];
  ifs.close(); 
  ifstream ifs2(filename.c_str());
  for(int i=0; i<n*m; i++){
    ifs2>>array[i];
    if(i%10000==0){cout<<"*"; cout.flush();}
  }
  cout<<endl;
  ifs2.close();
}


template<class TYPE>
int Matrix<TYPE>::save(const string filename){
  cout<<"Saving "<<filename<<" ..."<<endl;
  ofstream ofs(filename.c_str());
  for(int i=0; i<n; i++){
    for(int j=0; j<m; j++) 
      ofs<<" "<<array[i*m+j];
    ofs<<"\n";
  }
  ofs.close();
}

#endif
