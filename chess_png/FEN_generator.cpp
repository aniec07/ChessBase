#include<iostream>
#include<math.h>
#include<map>
#include<string>
#include<vector>
#include<sstream>

using namespace std;

#include "FEN_signature"
int get_index(char c);
string intToString(int);

/*int main(){
	Piece one,two,three;
	three.row = 6 ;
	three.column = 'a';
	three.type = "Pwd";
	three.color = 'b';
	three.isAlive = true;
	one.row = 1;
	one.column = 'a';
	one.type = "N2w";
	one.color = 'w';
	one.isAlive = true;
	two.row = 2;
	two.column = 'b';
	two.type = "K2w";
	two.color = 'b';
	two.isAlive = true;
	map<string,Piece> board_state;
	board_state["kbe"] = one;
	board_state["ges"] = two;
	board_state["dsf"] = three;
	string res;
	cell c1;
	c1.row = -1;
	c1.column = 'b';
	res = FEN_generator(board_state,'w',"KQ-q",c1,4,5);
	cout << res;
	return 0;


}*/

string FEN_generator(map<string,Piece> board_state,char active_color,string castling_availability,cell enpassant_target, int halfMoves, int fullMoves){
	string FEN_state;
	map<string,Piece>::iterator it = board_state.begin();
	vector<vector<char> > board(8,vector<char>(8));
	for ( int i = 0 ; i < 8 ; i++){
		for ( int j = 0 ; j < 8 ;j++){
			board[i][j] = '0';
		}
	}
	for ( it = board_state.begin() ; it != board_state.end() ; it++){
		if(it->second.isAlive){
			int col = get_index(it->second.column);
			int row = it->second.row;
			string p_type = it->second.type;
			if ( it->second.color == 'w')
				board[col][row-1] = p_type.at(0);
			if ( it->second.color == 'b')
				board[col][row-1] = tolower(p_type.at(0));
		}
	}
	for ( int i = 0 ; i < 8 ; i++)
		for ( int j = 0 ; j < 8 ; j++)
			cout << board[i][j] << " ";
	for ( int i = 0 ; i < 8 ; i++){
		bool inBlank = false;
		int count = 0;
		for ( int j = 0 ; j < 8 ; j++){
			if ( board[i][j] == '0' && inBlank == false){
				inBlank = true;
				count++;
			}
			else if ( board[i][j] == '0' && inBlank == true){
				count++;
			}
			else if ( board[i][j] != '0' && inBlank == false)
				FEN_state += board[i][j];
			else if ( board[i][j] != '0' && inBlank == true){
				string blank_count = intToString(count);
				FEN_state += blank_count;
				FEN_state += board[i][j];
				count = 0;
				inBlank = false;
			}
		}
		if ( count != 0 ){
			string blank_count = intToString(count);
			FEN_state += blank_count;
		}
		FEN_state += '/';
	}
	string enp_row = intToString(enpassant_target.row);
	string half_moves = intToString(halfMoves);
	string full_moves = intToString(fullMoves);
	FEN_state += active_color ;
	FEN_state += " " + castling_availability + " " ;
	if ( enpassant_target.row != -1)
		FEN_state += enpassant_target.column + enp_row + " " ;
	else
		FEN_state += "-" ;
	FEN_state += " " + half_moves + " " + full_moves;
	return FEN_state;
		
}

string intToString(int x){
	ostringstream convert;
	convert << x;
	string str = convert.str();
	return str;
}


int get_index(char c){
	char alpha[8] = { 'a','b','c','d','e','f','g','h'};
	for ( int i = 0 ; i < 8 ; i++){
		if ( alpha[i] == c )
			return i ;
	}
}
