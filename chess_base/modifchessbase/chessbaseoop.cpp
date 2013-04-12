#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <utility>
#include <math.h>

using namespace std;

typedef struct {
        int row;
        char column;
} cell;

typedef struct {
        string whitePlayerName;
        string blackPlayerName;
        int result;
} game;

typedef struct{
	bool is_chkmate;
	bool is_castling;
	char promotion;
	char conflict;
	int row;
	char column;
	string type;
	char color;
	bool isAlive;
	bool is_capture;
}data_from_input;

typedef struct {
        int gameId;
        string nextFEN;
}game_entry;

class Piece{
    private :
		cell position;
        string type;
        char color;
		bool isAlive;

    public:
        Piece(cell pos, string type, char color, bool isAlive);
        void move(cell destination);
        int getRow();
        char getColumn();
        string getType();
        char getColor();
		bool getIsAlive();
		void toggleIsAlive();
};

Piece::Piece(cell pos, string type_temp, char col, bool Alive) {
        position.row = pos.row;
        position.column = pos.column;
        type = type_temp;
        color = col;
		isAlive = Alive;
}


char Piece::getColor() {
        return color;
}

string Piece::getType() {
        return type;
}

int Piece::getRow() {
        return (position.row);
}

char Piece::getColumn() {
        return (position.column);
}

bool Piece::getIsAlive() {
        return isAlive;
}

void Piece::toggleIsAlive() {
        if(isAlive)
			isAlive = false;
		else isAlive = true;
}

class database {
        map<string, list<game_entry> > gamesDatabase;
        map<int, game> gamesTable;
        public :
        void update();
};

class board{
		map<string, Piece> configuration;

		public :
		board();
		void update(string input, int turn);
		string FEN_generator();
		friend void database::update();
		void display();
void tester();

		private:
		data_from_input convert_str2data(string move, int turn);
		char fetch_conflict(string move);
		void capture(cell destination);
		void do_castling(string type, char color);
		vector<Piece> get_valid_pawn_pieces (cell destination, char turn);
		vector<Piece> get_valid_all_pieces (char turn, char piecetype);
		vector<Piece> find_matches(vector<Piece> candidates, char ch);
		vector<Piece> find_matches(vector<Piece> candidates, int i);
		vector<Piece> filterSlope(vector<Piece> candidates, cell destination, char type);
		Piece filterByObstacle(vector<Piece> candidates, cell destination);
		double returnDist(Piece cfinal, cell cinitial);
		void insert_promoted_piece(char promote_type, cell destination, char promote_color);
};

void board::tester() {
		cell dest;
		dest.row = 1;
		dest.column = 'a';
		Piece R1(dest, "R1", 'w', true);
		cell dest1;
		dest1.row = 1;
		dest1.column = 'h';
		Piece R2(dest1, "R2", 'w', true);
		vector<Piece> vector_piece;
		vector_piece.push_back(R1);
		vector_piece.push_back(R2);
		cell destination;
		destination.row = 1;
		destination.column = 'h';
        //capture(destination);
        string mv = "Rxe6+";
        data_from_input ip = convert_str2data(mv,'b');

        //vector <Piece> fm = get_valid_all_pieces ('w','P');
        //for ( vector<Piece>::iterator it = fm.begin(); it != fm.end() ; it++ )
		cout << "row = " << ip.row << " column = " << ip.column << " " << ip.color << " " << ip.type << endl;
		//for (map<string, Piece>::iterator it = configuration.begin(); it != configuration.end(); it++)
		//	if (it->first.compare("Pw5") == 0)
		//		it -> second.toggleIsAlive();
}

void board::display() {
		for (int i = 8; i > 0; i--) {
				for (char a = 'a'; a <= 'h'; a++) {
						map<string, Piece>::iterator it = configuration.begin();
						while(it != configuration.end()) {
								if ((*it).second.getRow() == i && (*it).second.getColumn() == a && (*it).second.getIsAlive()) {
										cout << (*it).second.getType() << (*it).second.getColor() << ":";
										break;
								}
								it++;
						}
						if (it == configuration.end())
								cout << "___:";
				}
				cout << endl;
		}
}

board::board() {
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
				cell pos;
				pos.column = alpha[i];
                if ( pieces[i].compare("R1") == 0 ){
				    pos.row = 1;                //-------------------made changes here-------------------------//
				    Piece p (pos, pieces[i], 'w', true);
				    configuration.insert(pair<string, Piece> (wmap_pieces[i], p));
                }
                else if (pieces[i].compare("B1") == 0){
				    pos.row = 1;                //-------------------made changes here-------------------------//
				    Piece p (pos, pieces[i], 'w', true);
				    configuration.insert(pair<string, Piece> (wmap_pieces[i], p));
                }
                else if ( pieces[i].compare("R2") == 0 ){
				    pos.row = 1;                //-------------------made changes here-------------------------//
				    Piece p (pos, pieces[i], 'w', true);
				    configuration.insert(pair<string, Piece> (wmap_pieces[i], p));
                }
                else{
				    pos.row = 2;                //-------------------made changes here-------------------------//
				    Piece p (pos, pieces[i], 'w', true);
				    configuration.insert(pair<string, Piece> (wmap_pieces[i], p));
                }
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
				cell pos;
				pos.row = 3;
				pos.column = alpha[i];
				char n = nums.at(i);
				string t = "P";
				t += n;
				Piece p(pos,t,'w', true);
				configuration.insert( pair<string, Piece>(white_pawns[i], p) );
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
				cell pos;
				pos.row = 8;
				pos.column = alpha[i];
				Piece p(pos, pieces[i], 'b', true);
				configuration.insert( pair<string, Piece>(bmap_pieces[i], p) );
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
				cell pos;
				pos.row = 7;
				pos.column = alpha[i];
				char n = nums.at(i);
				string t = "P";
				t += n;
				Piece p(pos, t, 'b', true);
				configuration.insert(pair<string, Piece>(black_pawns[i], p) );
		}
}

/*
void board::update_map (string input, int turn) {
	map <string, Piece> piece_map = configuration;
	(*en_passant).row = -1;
	cell destination;
	data_from_input final_piece = convert_str2data (input, turn);
	destination.row = final_piece.row;
	destination.column = final_piece.column;
	char turn_char;
	if (turn % 2 == 1)
		turn_char = 'w';
	if (turn % 2 == 0)
		turn_char = 'b';

	cell new_dest = destination;
	if (final_piece.type[0] == 'P') {
		if (final_piece.is_capture)
			if (is_enpassant(piece_map, destination)) {
				if (turn_char  == 'b')
					new_dest.row = new_dest.row + 1;
				else 
					new_dest.row = new_dest.row - 1;
			}
	}
		
	// Handle Capture
	if (final_piece.is_capture)
		capture(new_dest);

	// Handle Castling
	if (final_piece.is_castling) {
		do_castling(final_piece.type, final_piece.color);
	}

	vector <Piece> candidates;

	// Getting candidate pieces
	if (final_piece.type[0] == 'P')
		candidates = get_valid_pawn_pieces (destination, turn_char);
	else
		candidates = get_valid_all_pieces(turn_char, final_piece.type[0]);
//cout << "piece type print= " << final_piece.type[0] << endl;
	
    //cout <<"piece type= " << final_piece.type[0] << "size=" << candidates.size() << endl;
	
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

	// Filtering candidates on the basis of obstacle
	if (candidates.size() > 1)
		candidates = filterByObstacle(candidates, destination);

	if (candidates.size() < 1) 
		return piece_map;
	Piece moved_piece = *(candidates.begin());

	//cout << "Promotion bachua   haan " << final_piece.promotion << endl;

	
	// Detecting en Passant in next step
	if (final_piece.type[0] == 'P')
		if (abs(moved_piece.row - destination.row) == 2) {
			(*en_passant).row = (moved_piece.row + destination.row) / 2; 
			(*en_passant).column = moved_piece.column;
		}
	string color = "";
	color += final_piece.color;

	moved_piece.type.insert(1, color);
	piece_map[moved_piece.type].row = destination.row;
	piece_map[moved_piece.type].
    column = destination.column;

	// handle Promotion
	cout << "Promotion occurs?? " << final_piece.promotion << " and size is: " << candidates.size() << endl;
	if (final_piece.promotion != 'n') {
		new_dest = destination;
		piece_map = capture(piece_map, new_dest);
		piece_map = insert_promoted_piece(piece_map, final_piece.promotion, destination, final_piece.color);
	}
	configuration = piece_map;
}   
*/

data_from_input board::convert_str2data(string move, int turn) {
		data_from_input chpiece;
		chpiece.is_castling = 0;
		chpiece.is_chkmate = 0;
		size_t found;

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
		found = move.find("=");
		if(found != string::npos) {
				if (move[length - 1] == '+' || move[length - 1] == '#')
						chpiece.promotion = move[length - 2];
				else
						chpiece.promotion = move[length - 1];
		}
		else
				chpiece.promotion = 'n';

		if (chpiece.promotion == 'n') {

				if(move[length - 1] == '+' || move[length - 1] == '#') {
						chpiece.is_chkmate = 1;
						chpiece.column = move[length - 3];
						chpiece.row = move[length - 2] - '1' + 1;
				}
				else if(move[0] != 'O') {
						chpiece.column = move[length - 2];
						chpiece.row = move[length - 1] - '1' + 1;
				}
		}
		else
		{
				if(move[length - 1] == '+' || move[length - 1] == '#') {
						chpiece.is_chkmate = 1;
						chpiece.column = move[length - 5];
						chpiece.row = move[length - 4] - '1' + 1;
				}
				else if(move[0] != 'O') {
						chpiece.column = move[length - 4];
						chpiece.row = move[length - 3] - '1' + 1;
				}
		}
		chpiece.isAlive = 1;
		if (chpiece.is_castling == 0)
				chpiece.conflict = fetch_conflict(move);

		found = move.find("x");
		if(found != string::npos)
				chpiece.is_capture = 1;
		else
				chpiece.is_capture = 0;
		if(chpiece.promotion != 'n' && chpiece.is_capture == 1)
				chpiece.conflict = move[0];
		return chpiece;
}


char board::fetch_conflict(string move) {
		int length = move.length();
		if(move[0]>='A' && move[0] <='Z')
				move = move.substr(1,move.length()-1);
		if(move[move.length() - 1] == '+' || move[move.length() - 1] == '#')
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


void board::capture(cell destination){
    map<string,Piece>::iterator it;
    for ( it = configuration.begin() ; it != configuration.end() ; it++){
        if ( it->second.getColumn() == destination.column && it->second.getRow() == destination.row)
            it->second.toggleIsAlive();
        }
}

/*
void board::do_castling(string type, char color) {
		map<string, Piece> init_map = configuration;
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
		configuration = init_map;
}

*/

vector<Piece> board::get_valid_pawn_pieces (cell destination, char turn) {
		map <string, Piece>::iterator it;
		vector<Piece> pawn_vector;
		for (it = configuration.begin(); it != configuration.end(); it++) {
                string temp = it->second.getType();
				if (temp[0] == 'P' && (it)->second.getIsAlive() && (it)->second.getColor() == turn) {
						if(turn == 'w') {
								if ((it)->second.getRow() - destination.row == -1 || (it)->second.getRow() - destination.row == -2)
										pawn_vector.push_back((it) -> second);
						}
						if(turn == 'b') {
								if ((it)->second.getRow() - destination.row == 1 || (it)->second.getRow() - destination.row == 2)
										pawn_vector.push_back((it) -> second);
						}
				}
		}
		return pawn_vector;
}



vector<Piece> board::get_valid_all_pieces (char turn, char piecetype) {
		vector<Piece> all_pieces;
		map <string, Piece>::iterator it;
		for ( it = configuration.begin() ; it != configuration.end() ; it++ ){
				string temp = (*it).first;
				if ( temp[0] == piecetype && temp[1] == turn && (*it).second.getIsAlive() )
						all_pieces.push_back((*it).second);
		}
		return all_pieces;
}



vector<Piece> board::find_matches(vector<Piece> candidates, char ch) {

		vector<Piece> matches;
		for (vector<Piece>::iterator it = candidates.begin(); it != candidates.end(); it++)
				if ((*it).getColumn() == ch)
						matches.push_back(*it);
		return matches;
}

vector<Piece> board::find_matches(vector<Piece> candidates, int i) {

		vector<Piece> matches;
		for (vector<Piece>::iterator it = candidates.begin(); it != candidates.end(); it++)
				if ((*it).getRow() == i)
						matches.push_back(*it);
		return matches;

}

/*

vector<Piece> board::filterSlope(vector<Piece> candidates, cell destination, char type){
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
*/

Piece board::filterByObstacle(vector<Piece> candidates, cell destination){

        //check colinearity
        vector<Piece>::iterator pit;
        map <string, Piece>::iterator mit;
        cell d2;
        bool isFound;
        for ( pit = candidates.begin() ; pit != candidates.end() ; pit ++ ){
            isFound = false;
            for ( mit = configuration.begin() ; mit != configuration.end() && !isFound ; mit ++ ){
                d2.row = (*pit).getRow();
                d2.column = (*pit).getColumn();
                    if ( (*pit).getType().compare((mit->second).getType()) != 0 || (*pit).getColor() != (mit->second).getColor() ){
                        if( returnDist(*pit,destination) == returnDist(mit->second,destination) + returnDist(mit->second,d2) ){   // obstacle present
                                isFound = true;
                        }
                    }
            }
                if ( mit == configuration.end() && !isFound )
                    return *pit;
        }
}

double board::returnDist(Piece cfinal, cell cinitial){
		double dist =pow(pow((cfinal.getRow() - cinitial.row),2)+pow((cfinal.getColumn() - cinitial.column),2),0.5);
		return dist;
}

void board::insert_promoted_piece(char promote_type, cell destination, char promote_color) {
		map <string, Piece> piece_map = configuration;
		map <string, Piece>::iterator it;
		cell tempDest;
		tempDest.row = destination.row;
		tempDest.column = destination.column;
		
		
		char max_index = '0';
		for (it = piece_map.begin(); it != piece_map.end(); it++) {
				if (it->first[0] == promote_type && it->first[1] == promote_color) {
						if (!(it->second.getIsAlive())){
								break;
						}
						if (it->first[2] > max_index)
								max_index = it->first[2];
				}
		}
		string promote_type_string = "";
		if (it == piece_map.end()) {
				max_index++;
				string reference_string = "";
				reference_string += promote_type;
				reference_string += promote_color;
				reference_string += max_index;
				promote_type_string.push_back(promote_type);
				promote_type_string.push_back(max_index);
				Piece promoted_piece (tempDest, promote_type_string, promote_color, true);
				piece_map.insert(pair <string, Piece> (reference_string, promoted_piece));
		}
		else{
				promote_type_string.push_back(max_index);
				Piece promoted_piece ( tempDest, it->second.getType(), promote_color, true);
				it->second = promoted_piece;
		}
		configuration = piece_map;
}

int main() {
    cout << "bhar main ja" << endl;
    board some_board;
	some_board.display();
	some_board.tester();
	some_board.display();
    //pos.column = 'a';
    //pos.row = 6;
    //chessPiece piece (pos,"P3",'w');
    //cout << "OOPS chal gaya"  << piece.getRow() << endl;
    //cout << "OOPS chal gaya"  << piece.getColor() << endl;
}
