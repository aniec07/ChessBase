#include <iostream>
#include <math.h>
#include <vector>
#include <map>
#include <fstream>

using namespace std;

typedef struct{
	int row;
	char column;
	string type;
	char color;
	bool isAlive;
}Piece;

typedef struct{
	bool is_chkmate;
	bool is_castling;
	char conflict;
	int row;
	char column;
	string type;
	char color;
	bool isAlive;
	bool is_capture;
}data_from_input;

typedef struct{
    int row;
    char column;
}cell;

map <string, Piece> update_map (map <string, Piece> piece_map, string input, int turn);
data_from_input convert_str2data(string move, int turn);
char fetch_conflict(string move);
map <string,Piece> capture(map<string,Piece> MainMap, cell destination);
map<string, Piece> do_castling(map<string, Piece> init_map, string type, char color);
vector<Piece> get_valid_pawn_pieces (map <string, Piece> piece_struct, cell destination, char turn);
vector<Piece> get_valid_all_pieces (map <string, Piece> piece_struct, char turn, char piecetype);
vector<Piece> find_matches(vector<Piece> init_list, char ch);
vector<Piece> find_matches(vector<Piece> init_list, int i);
vector<Piece> filterSlope(vector<Piece> candidates, cell destination, char type);
vector<Piece> filterDistance(vector<Piece> candidates, cell destination, char type);
double returnDist(Piece cfinal, cell cinitial);
map <string,Piece> returnMap ();
#include "FEN_signature"
#include "FEN_generator.cpp"

int main(int argc, char *argv[]){
	#include "initial_map.cpp"
    map <string,Piece> piece_map = init_map;
	char active_color;
	int t_count = 1;
	string move;
	ifstream infile;
	infile.open(argv[1]);
	cell c;
    c.row = 1;
    c.column = 'c';

	while(infile.good()) {
		getline(infile, move);
		data_from_input raw;
		raw = convert_str2data(move,t_count);
		if ( t_count % 2 )
			active_color = 'b';
		else
			active_color = 'w';
		piece_map = update_map(piece_map, move, t_count);	
		cout <<  FEN_generator(piece_map,active_color,"Kqkq",c,4,5);
		t_count++;
	}
	infile.close();
	string FEN_state;
/*
    piece_map = update_map (piece_map, "c3", 1);
    piece_map = update_map (piece_map, "d6", 2);
    piece_map = update_map (piece_map, "Na3", 3);
    piece_map = update_map (piece_map, "c6", 4);
    piece_map = update_map (piece_map, "b3", 5);
    piece_map = update_map (piece_map, "g6", 6);
    piece_map = update_map (piece_map, "Bb2", 7);
    piece_map = update_map (piece_map, "h6", 8);
    piece_map = update_map (piece_map, "Qc2", 9);
    piece_map = update_map (piece_map, "g5", 10);
    piece_map = update_map (piece_map, "O-O-O", 11);
    //piece_map = update_map (piece_map, "Rh6", 12);
    //piece_map = update_map (piece_map, "N3xe5", 13);
    //piece_map = update_map (piece_map, "h4", 14);
    piece_map = update_map (piece_map, "Nd6+", 15);
    piece_map = update_map (piece_map, "Rxd6", 16);
    piece_map = update_map (piece_map, "f3", 17);
    piece_map = update_map (piece_map, "h3", 18);
    piece_map = update_map (piece_map, "gxh3", 19);
    piece_map = update_map (piece_map, "g6", 20);
    piece_map = update_map (piece_map, "Bg2", 21);
    piece_map = update_map (piece_map, "Nc6", 22);
    piece_map = update_map (piece_map, "O-O-O", 23);*/
    //piece_map = update_map (piece_map, "Nd6+", 15);
//cout << "in main" << piece_map["Pw3"].row << piece_map["Pw3"].column << endl;
    map <string, Piece>::iterator it;
	int count = 0;
   for ( it = piece_map.begin() ; it != piece_map.end() ; it++) {
		if ((it)->second.isAlive) {
			count++;
        	cout << "(" <<(it)->second.row << "," << (it)->second.column << ") :" << (it)->second.type << (it)->second.color << endl;
		}
	}
	cout << "Alive count=" << count<<endl;
}

void print_vector(vector<Piece> candidates) {
    vector <Piece>::iterator it;
	cout << "prinitng candidates" << endl;
    for ( it = candidates.begin() ; it != candidates.end() ; it++)
        cout << "(" <<(it)->row << "," << (it)->column << ") :" << (it)->type << (it)->color << endl;
	
}

map <string, Piece> update_map (map <string, Piece> piece_map, string input, int turn) {
	cell destination;
	data_from_input final_piece = convert_str2data (input, turn);
	destination.row = final_piece.row;
	destination.column = final_piece.column;
	
	// Handle Capture
	if (final_piece.is_capture)
		piece_map = capture(piece_map, destination);

	// Handle Castling
	if (final_piece.is_castling) {
		return do_castling(piece_map, final_piece.type, final_piece.color);
	}

	char turn_char;
	if (turn % 2 == 1)
		turn_char = 'w';
	if (turn % 2 == 0)
		turn_char = 'b';

	vector <Piece> candidates;

	// Getting candidate pieces
	if (final_piece.type[0] == 'P')
		candidates = get_valid_pawn_pieces (piece_map, destination, turn_char);
	else
		candidates = get_valid_all_pieces(piece_map, turn_char, final_piece.type[0]);
	
	// Filtering candidates in case of an conflict
	if (candidates.size() > 1) {
		if (final_piece.conflict >= '1' && final_piece.conflict <= '8')
			candidates = find_matches( candidates, final_piece.conflict - '1' + 1);
		else if (final_piece.conflict >= 'a' && final_piece.conflict <= 'h')
			candidates = find_matches( candidates, final_piece.conflict);
	}
	
	// Filtering candidates on the basis of slope
	if (candidates.size() > 1)
		candidates = filterSlope(candidates, destination, final_piece.type[0]);

	// Filtering candidates on the basis of distance
	if (candidates.size() > 1)
		candidates = filterDistance(candidates, destination, final_piece.type[0]);

	if (candidates.size() < 1) 
		return piece_map;
	Piece moved_piece = *(candidates.begin());
	string color = "";
	color += final_piece.color;

	moved_piece.type.insert(1, color);
	piece_map[moved_piece.type].row = destination.row;
	piece_map[moved_piece.type].column = destination.column;
	return piece_map;
}

data_from_input convert_str2data(string move, int turn) {
    data_from_input chpiece;
    chpiece.is_castling = 0;
    chpiece.is_chkmate = 0;
    if ((move[0] >= 'a' && move[0] <= 'h') || move[0] == 'x') {
        chpiece.type = "P";
    }
    else if (move[0] != 'O') {
        chpiece.type = move[0];
    }
    else if (move[0] == 'O') {
        chpiece.is_castling = 1;
        chpiece.conflict = 'n';
    }

    if (turn % 2 == 0)
        chpiece.color = 'b';
    else
        chpiece.color = 'w';

    if(move.compare("O-O") == 0) {
        chpiece.type = 'K';
        return chpiece;
    }
    else if (move.compare("O-O-O") == 0) {
        chpiece.type = 'Q';
        return chpiece;
    }

    int length = move.length();

    if(move[length - 1] == '+') {
        chpiece.is_chkmate = 1;
        chpiece.column = move[length - 3];
        chpiece.row = move[length - 2] - '1' + 1;
    }
    else if(move[0] != 'O') {
        chpiece.column = move[length - 2];
        chpiece.row = move[length - 1] - '1' + 1;
    }
    chpiece.isAlive = 1;
    if (chpiece.is_castling == 0)
        chpiece.conflict = fetch_conflict(move);
        size_t found;
        found = move.find("x");
        if(found != string::npos)
        chpiece.is_capture = 1;
    return chpiece;
}

char fetch_conflict(string move) {
    int length = move.length();
    if(move[0]>='A' && move[0] <='Z')
        move = move.substr(1,move.length()-1);
    if(move[move.length() - 1] == '+')
        move.erase(move.length()-1, 1);
    size_t found;
    found = move.find("x");
    if(found != string::npos)
        if (move.length() == 4)
            return move[0];
        else
            return 'n';
    else
        if(move.length() == 3)
            return move[0];
        else
            return 'n';
}

map <string,Piece> capture(map<string,Piece> MainMap, cell destination){
    map<string,Piece>::iterator it;
    Piece temp;
    for ( it = MainMap.begin() ; it != MainMap.end() ; it++){
        temp = (*it).second;
        if ( temp.column == destination.column && temp.row == destination.row)
            (*it).second.isAlive = false;
        }
    return MainMap;
}

map<string, Piece> do_castling(map<string, Piece> init_map, string type, char color) {
    if (color == 'w') {
        if (type[0] == 'K') {
            for(map<string, Piece>::iterator it = init_map.begin(); it != init_map.end(); it++) {
                if( ((*it).second).row == 1 && ((*it).second).column == 'e' ) {
                    ((*it).second).column = 'g';
                }
                if( ((*it).second).row == 1 && ((*it).second).column == 'h' ) {
                                        ((*it).second).column = 'f';
                }
            }
        }
        else if(type[0] == 'Q') {
                        for(map<string, Piece>::iterator it = init_map.begin(); it != init_map.end(); it++) {
                                if( ((*it).second).row == 1 && ((*it).second).column == 'e' ) {
                                        ((*it).second).column = 'c';
                                }
                                if( ((*it).second).row == 1 && ((*it).second).column == 'a' ) {
                                        ((*it).second).column = 'd';
                                }
                        }
        }
    }

    else {
                if (type[0] == 'K') {
            		for(map<string, Piece>::iterator it = init_map.begin(); it != init_map.end(); it++) {
                            if( ((*it).second).row == 8 && ((*it).second).column == 'e' ) {
                                        ((*it).second).column = 'g';
                            }
                            if( ((*it).second).row == 8 && ((*it).second).column == 'h' ) {
                                        ((*it).second).column = 'f';
                            }
                        }
                }
                else if(type[0] == 'Q') {
            		for(map<string, Piece>::iterator it = init_map.begin(); it != init_map.end(); it++) {
                                if( ((*it).second).row == 8 && ((*it).second).column == 'e' ) {
                                        ((*it).second).column = 'c';
                                }
                                if( ((*it).second).row == 8 && ((*it).second).column == 'a' ) {
                                        ((*it).second).column = 'd';
                                }
                        }
                }
    }

    return init_map;
}

vector<Piece> get_valid_pawn_pieces (map <string, Piece> piece_struct, cell destination, char turn) {
    map <string, Piece>::iterator it;
    vector<Piece> pawn_vector;
    for (it = piece_struct.begin(); it != piece_struct.end(); it++) {
        if ((it)->second.type[0] == 'P' && (it)->second.isAlive && (it)->second.color == turn) {
            if(turn == 'w') {
                if ((it)->second.row - destination.row == -1 || (it)->second.row - destination.row == -2)
                    pawn_vector.push_back((it) -> second);
            }
            if(turn == 'b') {
                if ((it)->second.row - destination.row == 1 || (it)->second.row - destination.row == 2)
                    pawn_vector.push_back((it) -> second);
            }
        }
    }
    return pawn_vector;
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

vector<Piece> find_matches(vector<Piece> init_list, char ch) {

    vector<Piece> matches;
    for (vector<Piece>::iterator it = init_list.begin(); it != init_list.end(); it++)
        if ((*it).column == ch)
            matches.push_back(*it);
    return matches;
}

vector<Piece> find_matches(vector<Piece> init_list, int i) {

        vector<Piece> matches;
        for (vector<Piece>::iterator it = init_list.begin(); it != init_list.end(); it++)
                if ((*it).row == i)
                        matches.push_back(*it);
        return matches;

}

vector<Piece> filterSlope(vector<Piece> candidates, cell destination, char type){
    vector<Piece> Slopefilter;
    vector<Piece>::iterator it;
    for ( it = candidates.begin() ; it != candidates.end() ; it++){
        if ( type == 'P' ){
            if ( (abs(destination.row - (*it).row) == 1 || abs(destination.row - (*it).row) == 2) && abs(destination.column-(*it).column) == 0)
                Slopefilter.push_back(*it);
        }
        if ( type == 'K' ){
            if ( abs(destination.row - (*it).row) == 1 ||  abs(destination.column-(*it).column) == 1)
                Slopefilter.push_back(*it);
        }
        if ( type == 'N' ){
            if ( (abs(destination.row - (*it).row) == 1 && abs(destination.column - (*it).column) == 2) || (abs(destination.column-(*it).column) == 1 && abs(destination.row - (*it).row) == 2))
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

vector<Piece> filterDistance(vector<Piece> candidates, cell destination, char type){
    vector<Piece>::iterator it;
    vector<Piece> dfilt;
    double mindist = 100;
	Piece mindist_piece;
    for(it = candidates.begin() ; it != candidates.end() ; it++){
        if ( returnDist(*it,destination) < mindist ){
            mindist = returnDist(*it,destination) ;
            mindist_piece = (*it);
        }
    }
	dfilt.push_back(mindist_piece);
    return dfilt;
}

double returnDist(Piece cfinal, cell cinitial){
    double dist =pow(pow((cfinal.row - cinitial.row),2)+pow((cfinal.column - cinitial.column),2),0.5);
    return dist;
}


