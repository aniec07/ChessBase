#include <iostream>
#include <math.h>

using namespace std;

typedef struct{
    int row;
    char col;
}cell;


double returnSlope(cell cfinal, cell cinitial){
    double num = cfinal.row - cinitial.row;
    double den = cfinal.col - cinitial.col;
    return num/den;
}

double returnDist(cell cfinal, cell cinitial){
    double dist =pow(pow((cfinal.row - cinitial.row),2)+pow((cfinal.col - cinitial.col),2),0.5);
    return dist;
}

vector<Piece> get_valid_all_pieces (map <string, Piece> piece_struct, char turn, char piecetype) {
    vector<Piece> all_pieces;   
    map <string, Piece>::iterator it;
    for ( it = piece_struct.begin() ; it = piece_struct.end() ; it++ )
        if ( (*it->first)[0] == piecetype && (*it->first[1] == turn ) )
            all_pieces.push_back(*it->second);
    return all_pieces;
}

int main(){
    cell c1,c2;
    c1.row = 5;
    c2.row = 8;
    c1.col = 'a';
    c2.col = 'g';
    cout << "Slope is " << returnSlope(c2,c1) << endl;
    cout << "Distance is " << returnDist(c2,c1) << endl;
}
    
