#include <iostream>
#include <math.h>
#include <vector>
#include <map>

using namespace std;


typedef struct{
	int row;
	char column;
	string type;
	char color;
	bool isAlive;
}Piece;


typedef struct{
    int row;
    char column;
}cell;


vector<Piece> get_valid_all_pieces(map<string, Piece> piece_struct, char turn, char piecetype);
double returnDist(Piece cfinal, cell cinitial);
map <string,Piece> returnMap();
vector<Piece> filterDistance(vector<Piece> candidates, cell destination);
vector<Piece> filterSlope(vector<Piece> candidates, cell destination, char type);

int main(){
    cell c1;
    Piece c2;
    c1.row = 5;
    c2.row = 8;
    c1.column = 'a';
    c2.column = 'g';
    cell dest;
    dest.row = 3;
    dest.column = 'b';
    //cout << "Distance is " << returnDist(c2,c1) << endl;
    map <string,Piece> Map_Piece = returnMap();
    vector<Piece> valid_pieces = get_valid_all_pieces (Map_Piece, 'w', 'B');
    vector<Piece> Slopefilter = filterSlope (valid_pieces,dest,'B');
    vector<Piece>::iterator it;
    for ( it = Slopefilter.begin() ; it != Slopefilter.end() ; it++)
        cout<<"("<<(*it).row<<","<<(*it).column<<") :"<<(*it).type<<endl;
}


vector<Piece> filterDistance(vector<Piece> candidates, cell destination, char type){
    vector<Piece>::iterator it;
    vector<Piece> dfilt;
    double mindist = 100;
    for(it = candidates.begin() ; it != candidates.end() ; it++){
        if ( returnDist(*it,destination) < mindist ){
            mindist = returnDist(*it,destination) ;
            dfilt.push_back(*it);
        }
    }
    return dfilt;
}
    
vector<Piece> filterSlope(vector<Piece> candidates, cell destination, char type){
    vector<Piece> Slopefilter;
    vector<Piece>::iterator it;
    for ( it = candidates.begin() ; it != candidates.end() ; it++){
        if ( type == 'P' ){
            if ( abs(destination.row - (*it).row) == 1 || abs(destination.row - (*it).row) == 2 || abs(destination.column-(*it).column) == 1)
                Slopefilter.push_back(*it);
        }
        if ( type == 'K' ){ 
            if ( abs(destination.row - (*it).row) == 1 ||  abs(destination.column-(*it).column) == 1)
                Slopefilter.push_back(*it);
        }
        if ( type == 'N' ){
            if ( (abs(destination.row - (*it).row) == 1 && abs(destination.column - (*it).column) == 2) || (abs(destination.column-(*it).column) == 1 && abs(destination.row - (*it).row == 2)))
                Slopefilter.push_back(*it);
        }
        if ( type == 'B' ){
            if ( abs(destination.row - (*it).row) == abs(destination.column - (*it).column ))
                Slopefilter.push_back(*it);
        }
        if ( type == 'R' ){
            if ( destination.column - (*it).column == 0 ||  destination.row - (*it).row == 0 )
                Slopefilter.push_back(*it);
        }
        if ( type == 'Q' ){
            if ( destination.column - (*it).column == 0 ||  destination.row - (*it).row == 0 || abs(destination.row - (*it).row) == abs(destination.column - (*it).column ))
                Slopefilter.push_back(*it);
        }
    }
    return Slopefilter;
}        

double returnDist(Piece cfinal, cell cinitial){
    double dist =pow(pow((cfinal.row - cinitial.row),2)+pow((cfinal.column - cinitial.column),2),0.5);
    return dist;
}

vector<Piece> get_valid_all_pieces (map <string, Piece> piece_struct, char turn, char piecetype) {
    vector<Piece> all_pieces;   
    map <string, Piece>::iterator it;
    for ( it = piece_struct.begin() ; it != piece_struct.end() ; it++ ){
        string temp = (*it).first;
        if ( temp[0] == piecetype && temp[1] == turn && (*it).second.isAlive )
            all_pieces.push_back((*it).second);
    }
    return all_pieces;
}




map <string,Piece> returnMap (){
    map <string,Piece> Piece_Map;
    
    Piece piece_temp;
    piece_temp.row = 1;
    piece_temp.column = 'a';
    piece_temp.type = "N1";
    piece_temp.color = 'w';
    piece_temp.isAlive = true;

    Piece_Map["Nw1"] = piece_temp;
    
    piece_temp.row = 5;
    piece_temp.column = 'g';
    piece_temp.type = "N2";
    piece_temp.color = 'w';
    piece_temp.isAlive = true;

    Piece_Map["Nw2"] = piece_temp;

    piece_temp.row = 3;
    piece_temp.column = 'a';
    piece_temp.type = "R1";
    piece_temp.color = 'w';
    piece_temp.isAlive = true;

    Piece_Map["Rw1"] = piece_temp;
    
    piece_temp.row = 4;
    piece_temp.column = 'b';
    piece_temp.type = "R2";
    piece_temp.color = 'w';
    piece_temp.isAlive = true;

    Piece_Map["Rw2"] = piece_temp;

    piece_temp.row = 1;
    piece_temp.column = 'd';
    piece_temp.type = "B1";
    piece_temp.color = 'w';
    piece_temp.isAlive = true;

    Piece_Map["Bw1"] = piece_temp;
    
    piece_temp.row = 4;
    piece_temp.column = 'd';
    piece_temp.type = "B2";
    piece_temp.color = 'w';
    piece_temp.isAlive = true;

    Piece_Map["Bw2"] = piece_temp;

    
    piece_temp.row = 8;
    piece_temp.column = 'c';
    piece_temp.type = "N1";
    piece_temp.color = 'b';
    piece_temp.isAlive = true;

    Piece_Map["Nb1"] = piece_temp;
    
    piece_temp.row = 2;
    piece_temp.column = 'b';
    piece_temp.type = "N2";
    piece_temp.color = 'b';
    piece_temp.isAlive = true;

    Piece_Map["Nb2"] = piece_temp;

    piece_temp.row = 6;
    piece_temp.column = 'd';
    piece_temp.type = "R1";
    piece_temp.color = 'b';
    piece_temp.isAlive = true;

    Piece_Map["Rb1"] = piece_temp;
    
    piece_temp.row = 5;
    piece_temp.column = 'a';
    piece_temp.type = "R2";
    piece_temp.color = 'b';
    piece_temp.isAlive = true;

    Piece_Map["Rb2"] = piece_temp;

    piece_temp.row = 1;
    piece_temp.column = 'c';
    piece_temp.type = "B1";
    piece_temp.color = 'b';
    piece_temp.isAlive = true;

    Piece_Map["Bb1"] = piece_temp;
    
    piece_temp.row = 1;
    piece_temp.column = '8';
    piece_temp.type = "B2";
    piece_temp.color = 'b';
    piece_temp.isAlive = true;

    Piece_Map["Bb2"] = piece_temp;

return Piece_Map;

}
