#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <cassert>
#include <limits>
#include <random>

using namespace std;


#define vec2d vector< vector<int> >
#define str2d vector< vector<char> >

string proteinSequence="HHHPPH";
vector< vector<int> > direction={{0,1},{0,-1},{1,0},{-1,0},{1,1},{-1,-1},{1,-1},{-1,1}};
vector< vector<int> > proteinPos = {{0,0}};
vector< vector<char> > proteinPlot ;

//For adding two vectors
template <typename T>
vector<T> operator+(const vector<T>& a, const vector<T>& b){
    assert(a.size() == b.size()); 

    vector<T> result;
    result.reserve(a.size());

    transform(a.begin(), a.end(), b.begin(), 
                   back_inserter(result), plus<T>());
    return result;
}


//For printing a "matrix" directly to the output stream
template <typename T>
ostream& operator<<(ostream& s, vector< vector <T> > mat){
  s << "[\n";
  for(auto row:mat){
    for(auto e:row){
      s<<e<<" ";
    }
    s<<"\n";
  }
    
  return s << "]\n";
}


void findCorners(vector< vector<int> > positions, int &maxX, int &minX, int &maxY, int &minY){
  maxX=numeric_limits<int>::min();
  maxY=maxX;
  minX=numeric_limits<int>::max();
  minY=minX;

  for(auto pos:positions){
    if(pos[0]>maxX)
      maxX=pos[0];
    if(pos[1]>maxY)
      maxY=pos[1];
    if(pos[0]<minX)
      minX=pos[0];
    if(pos[1]<minY)
      minY=pos[1];
  }
}

void visualise (vec2d positions,string sequence,str2d &proteinPlot){
  int maxX,maxY,minX,minY;
  findCorners(positions,maxX,minX,maxY,minY);
  // cout<<"Max:"<<maxX<<","<<maxY<<"\nMin:"<<minX<<","<<minY<<"\n";
  int rows=maxX-minX+1,cols=maxY-minY+1;
  vector<int> minusOrigin = {-minX,-minY};
  vector<int> cPos;
  int counter=0;
  //Initialise a blank plot
  vector< vector<char> > plot(rows,vector<char>(cols,' '));
  proteinPlot = plot;

  //Test the plot
  cout<<proteinPlot;

  for(auto pos:positions){
    cPos=pos + minusOrigin;
    proteinPlot[cPos[0]][cPos[1]]=sequence[counter];    
    counter++;
  }
  //proteinPlot[0][4]='Y';
  cout<<proteinPlot;

}


int main()
{
  for(char protein:proteinSequence)    {
    proteinPos.push_back(proteinPos.back() + direction[4]);
  }
  cout<<"protein Sequence"<<proteinSequence<<"\n";
  //protein.push_back({1,2});
  // for(auto i:proteinPos){
  //     cout<<"("<<i[0]<<","<<i[1]<<")"<<"\n";      
  // }

  cout<<proteinPos;
  
  visualise (proteinPos,proteinSequence,proteinPlot);
  
  cout<<proteinSequence.length()<<"\nWicked\n";
  return 0;
}
