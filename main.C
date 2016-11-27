#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <cassert>
#include <limits>
#include <random>
#include <cmath>
using namespace std;


#define vec2d vector< vector<int> >
#define str2d vector< vector<char> >

string proteinSequence="HHHPPP";
char charBlank='`';
int score;
vector< vector<int> > direction={{1,0},{0,1},{0,-1},{-1,0}};//,{1,1},{-1,-1},{1,-1},{-1,1}};
const int ndir=4;
int iterations=0;
int bestScore=0;
vector<int> bestDirString;
vector< vector<int> > proteinPos;
vector< vector<char> > proteinPlot ;
vector< vector<char> > proteinPlotNum ;
vector<int> dirString;
//For reverse iterating
template<typename It>
class Range
{
    It b, e;
public:
    Range(It b, It e) : b(b), e(e) {}
    It begin() const { return b; }
    It end() const { return e; }
};

template<typename ORange, typename OIt = decltype(std::begin(std::declval<ORange>())), typename It = std::reverse_iterator<OIt>>
Range<It> reverse(ORange && originalRange) {
    return Range<It>(It(std::end(originalRange)), It(std::begin(originalRange)));
}


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


//For printing a "vector" directly to the output stream
template <typename T>
ostream& operator<<(ostream& s, vector<T>  row){
  s << "[";
    for(auto e:row){
      s<<e<<" ";
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

//Calculates the visualization
//Returns 1 if there's an intersection and aborts
//Returns 0 if it thinks everything went smooth
int visualise (vec2d positions,string sequence,str2d &proteinPlot,str2d &proteinPlotNum,int &score){
  int maxX,maxY,minX,minY;
  findCorners(positions,maxX,minX,maxY,minY);
  // cout<<"Max:"<<maxX<<","<<maxY<<"\nMin:"<<minX<<","<<minY<<"\n";
  int rows=maxX-minX+1,cols=maxY-minY+1;
  vector<int> minusOrigin = {-minX,-minY};
  vector<int> cPos;
  int counter=0;
  //Initialise a blank plot & the score
  vector< vector<char> > plot(rows,vector<char>(cols,charBlank));
  proteinPlot = plot;
  proteinPlotNum = plot;
  score = 0;

  //Construct the plot, calculate the score and check for validity on the fly
  for(auto pos:positions){
    cPos=pos + minusOrigin;

    //Condition for collisions
    if(proteinPlot[cPos[0]][cPos[1]] != charBlank){
    	return 1;
    }
    proteinPlot[cPos[0]][cPos[1]]=sequence[counter];
    proteinPlotNum[cPos[0]][cPos[1]]='A'+counter;

    //Now calculate the score
    //
    if(sequence[counter]=='H' && counter>0){
      int x=0,y=0;

      //Lazy to do this properly :(
      
      x=cPos[0]+1;
      y=cPos[1];
      if(x>=0 && x<cols && y>=0 && y<rows){
  	if(proteinPlot[x][y] == proteinPlot[cPos[0]][cPos[1]] && \
  	   x != positions[counter-1][0] && \
  	   y != positions[counter-1][1] \
  	   ){
  	  score+=1;
  	}
      }

      x=cPos[0]-1;
      y=cPos[1];
      if(x>=0 && x<cols && y>=0 && y<rows){
  	if(proteinPlot[x][y] == proteinPlot[cPos[0]][cPos[1]] && \
  	   x != positions[counter-1][0] && \
  	   y != positions[counter-1][1] \
  	   ){
  	  score+=1;
  	}
      }

      x=cPos[0];
      y=cPos[1]+1;
      if(x>=0 && x<cols && y>=0 && y<rows){
  	if(proteinPlot[x][y] == proteinPlot[cPos[0]][cPos[1]] && \
  	   x != positions[counter-1][0] && \
  	   y != positions[counter-1][1] \
  	   ){
  	  score+=1;
  	}
      }

      x=cPos[0];
      y=cPos[1]-1;
      if(x>=0 && x<cols && y>=0 && y<rows){
  	if(proteinPlot[x][y] == proteinPlot[cPos[0]][cPos[1]] && \
  	   x != positions[counter-1][0] && \
  	   y != positions[counter-1][1] \
  	   ){
  	  score+=1;
  	}
      }
            
    }
    
    
    counter++;
  }

  //Test the plot
  //cout<<proteinPlot;
  //cout<<proteinPlotNum;
  return 0;
}


int main()
{
  //Initialize the direction string with 0,0,0,...
  for(char protein:proteinSequence){
    dirString.push_back(0);
  }

  //Initialize the proteinPos  
  cout<<"protein Sequence:"<<proteinSequence<<"\n";

  while(iterations<pow(ndir,proteinSequence.length()) ){

    //Increment the direction string
    for (auto& d:reverse(dirString)) {
      d+=1;    
      if(d>(ndir-1))
    	d=1;      
      else 
    	break;      
    }
    iterations++;
    
    //proteinPos.clear();
    proteinPos={{0,0}};
    //for(char protein:proteinSequence)    {
    for(int i=2; i<=proteinSequence.length();i++){
      proteinPos.push_back(proteinPos.back() + direction[dirString[i]]);
      //i++;
    }

    //cout<<"protein Sequence:"<<proteinSequence<<"\n";    
    if(visualise (proteinPos,proteinSequence,proteinPlot,proteinPlotNum,score) == 0){
      if(score>=bestScore){
	//cout<<proteinPos;
	cout<<"Found something\n";
	cout<<proteinPlot;
	cout<<proteinPlotNum;
	cout<<dirString;
	cout<<"Score:"<<score<<"\n";
	cout<<"---------------\n";
	bestScore=score;
	bestDirString=dirString;
      }
    }
    else
      score=0;

  }
  return 0;
}
