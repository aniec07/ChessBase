#include<iostream>
#include<map>
#include<string>
#include<vector>
/*
typedef struct{
	int row;
	char column;
	string type;
	char color;
	bool isAlive;
}Piece;
*/

using namespace std;


map<string,Piece> init_map;
string alpha = "abcdefgh";
vector<string> pieces;
pieces.push_back("R1");
pieces.push_back("N1");
pieces.push_back("B1");
pieces.push_back("Q1");
pieces.push_back("K1");
pieces.push_back("B2");
pieces.push_back("N2");
pieces.push_back("R2");

vector<string> wmap_pieces;
wmap_pieces.push_back("Rw1");
wmap_pieces.push_back("Nw1");
wmap_pieces.push_back("Bw1");
wmap_pieces.push_back("Qw1");
wmap_pieces.push_back("Kw1");
wmap_pieces.push_back("Bw2");
wmap_pieces.push_back("Nw2");
wmap_pieces.push_back("Rw2");

for ( int i = 0 ; i < 8 ; i++){
	Piece p;
	p.row = 1;
	p.column = alpha[i];
	p.isAlive = true;
	p.color = 'w';
	p.type = pieces[i];
	init_map[wmap_pieces[i]] = p;
}
vector<string> white_pawns;
white_pawns.push_back("Pw1");
white_pawns.push_back("Pw2");
white_pawns.push_back("Pw3");
white_pawns.push_back("Pw4");
white_pawns.push_back("Pw5");
white_pawns.push_back("Pw6");
white_pawns.push_back("Pw7");
white_pawns.push_back("Pw8");
string nums = "12345678";

for ( int i = 0 ; i < 8 ; i++){
     Piece p;
     p.row = 2;
     p.column = alpha[i];
     p.isAlive = true;
     p.color = 'w';
	 char n = nums.at(i);
	 string t = "P";
	 t += n;
     p.type = t ;
     init_map[white_pawns[i]] = p;
}

vector<string> bmap_pieces;
bmap_pieces.push_back("Rb1");
bmap_pieces.push_back("Nb1");
bmap_pieces.push_back("Bb1");
bmap_pieces.push_back("Qb1");
bmap_pieces.push_back("Kb1");
bmap_pieces.push_back("Bb2");
bmap_pieces.push_back("Nb2");
bmap_pieces.push_back("Rb2");
 for ( int i = 0 ; i < 8 ; i++){
     Piece p;
     p.row = 8;
     p.column = alpha[i];
     p.isAlive = true;
     p.color = 'b';
     p.type = pieces[i];
     init_map[bmap_pieces[i]] = p;
  }
vector<string> black_pawns;
black_pawns.push_back("Pb1");
black_pawns.push_back("Pb2");
black_pawns.push_back("Pb3");
black_pawns.push_back("Pb4");
black_pawns.push_back("Pb5");
black_pawns.push_back("Pb6");
black_pawns.push_back("Pb7");
black_pawns.push_back("Pb8");


for ( int i = 0 ; i < 8 ; i++){
      Piece p;
      p.row = 7;
      p.column = alpha[i];
      p.isAlive = true;
      p.color = 'b';
      char n = nums.at(i);
      string t = "P";
      t += n;
      p.type = t ;
      init_map[black_pawns[i]] = p;
}


		

