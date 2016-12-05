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
#define vec1d vector<int>
#define str2d vector< vector<char> >
#define str1d vector<char>
string proteinSequence="HHHAHHAAH";
char charBlank='`';
int score;
vector< vector<int> > direction={{1,0},{0,1},{0,-1},{-1,0}};//,{1,1},{-1,-1},{1,-1},{-1,1}};
vector<char>  directionChar={'|','-','-','|'};			     
const int ndir=4;
int iterations=0;
int bestScore=0;
vector<int> bestDirString;
vector< vector<int> > proteinPos;
vector< vector<char> > proteinPlot ;
vector< vector<char> > proteinPlotNum ;
vector< vector<char> > proteinPlotPretty;
vector< vector<char> > bestProteinPlotPretty;
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
    if(pos[1]>maxX)
      maxX=pos[1];
    if(pos[0]>maxY)
      maxY=pos[0];
    if(pos[1]<minX)
      minX=pos[1];
    if(pos[0]<minY)
      minY=pos[0];
  }
}

//Calculates the visualization
//Returns 1 if there's an intersection and aborts
//Returns 0 if it thinks everything went smooth
int visualise (vec2d positions,string sequence,str2d &proteinPlot,str2d &proteinPlotPretty, vec1d &dirString, vec2d &direction, str1d &directionChar,str2d &proteinPlotNum,int &score){
  int maxX,maxY,minX,minY;
  findCorners(positions,maxX,minX,maxY,minY);
  // cout<<"Max:"<<maxX<<","<<maxY<<"\nMin:"<<minX<<","<<minY<<"\n";
  int cols=maxX-minX+1,rows=maxY-minY+1;
  vector<int> minusOrigin = {-minY,-minX};
  vector<int> cPos;
  int counter;
  counter=0;
  //Initialise a blank plot & the score
  vector< vector<char> > plot(rows,vector<char>(cols,charBlank));
  vector< vector<char> > plotInit(rows,vector<char>(cols,charBlank));
  vector< vector<char> > plotPrettyInit(rows*2, vector<char> (cols*2, ' '));
  vector< vector<char> > plotTest;
  proteinPlot = plot;
  proteinPlotNum = plot;
  proteinPlotPretty = plotPrettyInit;
  score = 0;

  // cout<<"NEW ITERATION\n";
  //Construct the plot, calculate the score and check for validity on the fly
  for(auto pos:positions){
    plotTest=plotInit; //initalize this; for debugging only
    cPos=pos + minusOrigin; 
    // cout<<"counter check:"<<counter<<"\n";

    //Condition for a valid cPos is not necessary because the origin shifting and size of array should automatically take this into consideration
    
    //Condition for collisions
    if(proteinPlot[cPos[0]][cPos[1]] != charBlank){
    	return 1;
    }
    
    proteinPlot[cPos[0]][cPos[1]]=sequence[counter];
    proteinPlotPretty[cPos[0]*2][cPos[1]*2]=sequence[counter];
    if(counter>0){
	int x=cPos[1]*2 - direction[dirString[counter-1]][1];
	int y=cPos[0]*2 - direction[dirString[counter-1]][0];
	proteinPlotPretty[y][x]=directionChar[dirString[counter-1]];	
	}
    proteinPlotNum[cPos[0]][cPos[1]]='A'+counter;
    
    //Now calculate the score
    //
    if(sequence[counter]=='H' && counter>0){
      int x=0,y=0;

      //Lazy to do this properly :(
      //For logical consistency, swap x and y.
      
      x=cPos[0]+1;
      y=cPos[1];
      if(x>=0 && x<rows && y>=0 && y<cols){
  	if(proteinPlot[x][y] == proteinPlot[cPos[0]][cPos[1]] && \
  	   (x != (positions[counter-1][0] + minusOrigin[0]) ||		\
	    y != (positions[counter-1][1] + minusOrigin[1])	)	\
  	   ){
  	  score+=1;
	  // plotTest[cPos[0]][cPos[1]]='X';
	  // plotTest[x][y]='Y';
	  // cout<<"maxRow and maxCol"<<rows<<' '<<cols<<"\n"<<plotTest;
	  // cout<<"counter:"<<counter<<"\n";
	  // cout<<proteinPlot[cPos[0]][cPos[1]]<<"\n";       
	  // cout<<"x,y:"<<x<<","<<y<<"\n";
	  // cout<<"cPos"<<cPos[0]<<","<<cPos[1]<<"\n";
  	}
      }

      x=cPos[0]-1;
      y=cPos[1];
      if(x>=0 && x<rows && y>=0 && y<cols){
  	if(proteinPlot[x][y] == proteinPlot[cPos[0]][cPos[1]] && \
  	   (x != (positions[counter-1][0] + minusOrigin[0]) ||		\
	    y != (positions[counter-1][1] + minusOrigin[1])	)	\
  	   ){
  	  score+=1;
	  // plotTest[cPos[0]][cPos[1]]='X';
	  // plotTest[x][y]='Y';
	  // cout<<"maxRow and maxCol"<<rows<<' '<<cols<<"\n"<<plotTest;
	  // cout<<"counter:"<<counter<<"\n";
	  // cout<<proteinPlot[cPos[0]][cPos[1]]<<"\n";       
	  // cout<<"x,y:"<<x<<","<<y<<"\n";
	  // cout<<"cPos"<<cPos[0]<<","<<cPos[1]<<"\n";
  	}
      }

      x=cPos[0];
      y=cPos[1]+1;
      if(x>=0 && x<rows && y>=0 && y<cols){
  	if(proteinPlot[x][y] == proteinPlot[cPos[0]][cPos[1]] && \
  	   (x != (positions[counter-1][0] + minusOrigin[0]) ||		\
	    y != (positions[counter-1][1] + minusOrigin[1])	)	\
  	   ){
  	  score+=1;
	  // plotTest[cPos[0]][cPos[1]]='X';
	  // plotTest[x][y]='Y';
	  // cout<<"maxRow and maxCol"<<rows<<' '<<cols<<"\n"<<plotTest;
	  // cout<<"counter:"<<counter<<"\n";
	  // cout<<proteinPlot[cPos[0]][cPos[1]]<<"\n";       
	  // cout<<"x,y:"<<x<<","<<y<<"\n";
	  // cout<<"cPos"<<cPos[0]<<","<<cPos[1]<<"\n";
  	}
      }

      x=cPos[0];
      y=cPos[1]-1; 
      if(x>=0 && x<rows && y>=0 && y<cols){
  	if(proteinPlot[x][y] == proteinPlot[cPos[0]][cPos[1]] && \
  	   (x != (positions[counter-1][0] + minusOrigin[0]) ||		\
	    y != (positions[counter-1][1] + minusOrigin[1])	)	\
  	   ){
  	  score+=1;
	  // plotTest[cPos[0]][cPos[1]]='X';
	  // plotTest[x][y]='Y';
	  // cout<<"maxRow and maxCol"<<rows<<' '<<cols<<"\n"<<plotTest;
	  // cout<<"counter:"<<counter<<"\n";
	  // cout<<proteinPlot[cPos[0]][cPos[1]]<<"\n";       
	  // cout<<"x,y:"<<x<<","<<y<<"\n";
	  // cout<<"cPos"<<cPos[0]<<","<<cPos[1]<<"\n";
  	}
      }
            
    }
     
    
    counter++;
  }

  // cout<<"END\n\n";
  //Test the plot
  //cout<<proteinPlot;
  //cout<<proteinPlotNum;
  return 0;
}


int main(int argc, char** argv)
{
  proteinSequence=argv[1];
  //Initialize the direction string with 0,0,0,...
  for(int m=0; m<proteinSequence.length()-1;m++){
    dirString.push_back(0);
  }

  //Initialize the proteinPos  
  cout<<"\n\nProtein Sequence:"<<proteinSequence<<"\n";

  while(iterations<pow(ndir,proteinSequence.length()-2) ){

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
    for(int i=1; i<=proteinSequence.length()-1;i++){
      //proteinPos.push_back(proteinPos.back() + direction[dirString[i]]);
      proteinPos.push_back(proteinPos[i-1]+direction[dirString[i-1]]);
      //i++;
    }

    //cout<<"protein Sequence:"<<proteinSequence<<"\n";
    if(visualise (proteinPos,proteinSequence,proteinPlot,proteinPlotPretty,dirString,direction,directionChar,proteinPlotNum,score) == 0){
      if(score>=bestScore){
	//cout<<proteinPos;
	// cout<<"Found something\n";
	// cout<<proteinPlot;
	// cout<<proteinPlotPretty;
	// cout<<proteinPlotNum;
	// cout<<proteinPos;
	// cout<<dirString;
	// cout<<"Score:"<<score<<"\n";
	// cout<<"---------------\n";
	bestScore=score;
	bestDirString=dirString;
	bestProteinPlotPretty=proteinPlotPretty;
      }
    }
    else
      score=0;
  }

  cout<<"Best Score:"<<bestScore<<"\nPossible Configuration:\n"<<bestProteinPlotPretty<<"\n\n";
  return 0;
}

