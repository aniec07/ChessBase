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
	string conflict;
	int row;
	char column;
	string type;
	char color;
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
		//bool isAlive;

    public:
        Piece(cell pos, string type, char color);
        void move(cell destination);
        int getRow();
        char getColumn();
        string getType();
        char getColor();
};

Piece::Piece(cell pos, string type_temp, char col) {
        position.row = pos.row;
        position.column = pos.column;
        type = type_temp;
        color = col;
		//isAlive = Alive;
}

void Piece::move(cell destination) {
		position.row = destination.row;
		position.column = destination.column;
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

/*bool Piece::getIsAlive() {
        return isAlive;
}

void Piece::toggleIsAlive() {
        if(isAlive)
			isAlive = false;
		else isAlive = true;
}*/

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
		bool is_enpassant(cell destination);
	    string fetch_conflict(string move);
		void capture(cell destination);
		void do_castling(string type, char color);
		vector<Piece> get_valid_pawn_pieces (cell destination, char turn);
		vector<Piece> get_valid_all_pieces (char turn, char piecetype);
		vector<Piece> find_matches(vector<Piece> candidates, char ch);
		vector<Piece> find_matches(vector<Piece> candidates, int i);
		vector<Piece> filterSlope(vector<Piece> candidates, cell destination, char type);
		vector<Piece> filterByObstacle(vector<Piece> candidates, cell destination);
		double returnDist(Piece cfinal, cell cinitial);
		void insert_promoted_piece(char promote_type, cell destination, char promote_color);
};

void board::tester() {
		//display();
		/*string inp = "";
		for ( int i = 0 ; i < 100 ; i ++ ){
			cout << "enter the string: ";
			cin >> inp;
            update(inp,i+1);
            display();
            cout << endl << endl;
            cout << string(10, '\n');
		}*/
		update("e4", 1);
		update("e5", 2);
		update("f4", 3);
		update("exf4", 4);
		update("e5", 5);
		update("f3", 6);
		update("h4", 7);
		update("fxg2", 8);
		update("h5", 9);
		update("gxh1=Q", 10);
		update("h6", 11);
		update("g5", 12);
		update("Ke2", 13);
		update("g4", 14);
		update("Kd3", 15);
		update("g3", 16);
		update("Kc3", 17);
		update("g2", 18);
		update("Kb3", 19);
		update("gxf1=Q", 20);
		update("a3", 21);
		update("Qg5", 22);
		update("a4", 23);
		update("Qh2", 24);
		update("Ra3", 25);
		update("Qe1", 26);
		update("Nc3", 27);
		update("Qee2", 28);
		update("d3", 29);
		update("Qgh5", 30);
		update("Nf3", 31);
		update("f5", 32);
		update("Nd4", 33);
		update("f4", 34);
		update("Ne6", 35);
		update("f3", 36);
		update("Nd8", 37);
		update("f2", 38);
		update("Nc6", 39);
		update("f1=Q", 40);
		update("Na5", 41);
		update("Q2h3", 42);
		update("Nb5", 43);
		update("Qfh1", 44);
		update("Ra1", 45);
		update("Qef1", 46);
		update("Nc3", 47);
		update("Q5f3", 48);
		update("Nc6", 49);
		update("Ne7", 50);
		update("Nd4", 51);
		update("Rg8", 52);
		update("Nf5", 53);
		update("Rg7", 54);
		update("Ne3", 55);
		update("d6", 56);
		update("Ng2", 57);
		update("Qf1xg2", 58);
		display();
		//update("Qb3f5", 30);
		//cout << endl << endl;
		//display();
		/*vector_piece = filterSlope(vector_piece, destination, 'P');
		cout << "Returned : " << (vector_piece.begin())->getType() << endl;
		cout << "Row : " << (vector_piece.begin())->getRow() << endl;
		cout << "Column : " << (vector_piece.begin())->getColumn() << endl;
		cout << "size= : " << vector_piece.size() << endl;
		cout << "Returned : " << (++(vector_piece.begin()))->getType() << endl;
		cout << "Row : " << (++(vector_piece.begin()))->getRow() << endl;
		cout << "Column : " << (++(vector_piece.begin()))->getColumn() << endl;*/
		//for (map<string, Piece>::iterator it = configuration.begin(); it != configuration.end(); it++)
		//	if (it->first.compare("Pw5") == 0)
		//		it -> second.toggleIsAlive();
		//Piece p(dest1, "P1", 'w', true);
		//cout << "dist = " << returnDist(p, dest) << endl;
}

void board::display() {
		for (int i = 8; i > 0; i--) {
				for (char a = 'a'; a <= 'h'; a++) {
						map<string, Piece>::iterator it = configuration.begin();
						while(it != configuration.end()) {
								if ((*it).second.getRow() == i && (*it).second.getColumn() == a ) {
										cout << (*it).second.getType() << (*it).second.getColor() << ":  ";
										break;
								}
								it++;
						}
						if (it == configuration.end())
								cout << "___:  ";
				}
				cout << endl << endl << endl;
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
				pos.row = 1;
				pos.column = alpha[i];
				Piece p(pos,pieces[i],'w');
				configuration.insert( pair<string, Piece>(wmap_pieces[i],p) );
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
				pos.row = 2;
				pos.column = alpha[i];
				char n = nums.at(i);
				string t = "P";
				t += n;
				Piece p(pos,t,'w');
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
				Piece p(pos, pieces[i], 'b');
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
				Piece p(pos, t, 'b');
				configuration.insert(pair<string, Piece>(black_pawns[i], p) );
		}
}

void board::update(string input, int turn) {
	cell destination;
	data_from_input parsed_move = convert_str2data (input, turn);
	destination.row = parsed_move.row;
	destination.column = parsed_move.column;
	char turn_char;
	if (turn % 2 == 1)
		turn_char = 'w';
	if (turn % 2 == 0)
		turn_char = 'b';

	cell new_dest = destination;
	if (parsed_move.type[0] == 'P') {
		if (parsed_move.is_capture)
			if (is_enpassant(destination)) {
				if (turn_char  == 'b')
					new_dest.row = new_dest.row + 1;
				else 
					new_dest.row = new_dest.row - 1;
			}
	}
		
	// Handle Capture
	if (parsed_move.is_capture)
		capture(new_dest);

	// Handle Castling
	if (parsed_move.is_castling) {
		do_castling(parsed_move.type, parsed_move.color);
		return;
	}

	vector <Piece> candidates;

	// Getting candidate pieces
	if (parsed_move.type[0] == 'P')
		candidates = get_valid_pawn_pieces (destination, turn_char);
	else
		candidates = get_valid_all_pieces(turn_char, parsed_move.type[0]);
	
	// Filtering candidates in case of an conflict
	if (candidates.size() > 1 ){
            if ( parsed_move.conflict.length() == 1 ){
		        if (parsed_move.conflict[0] >= '1' && parsed_move.conflict[0] <= '8')
			        candidates = find_matches( candidates, parsed_move.conflict[0] - '1' + 1);
		        else if (parsed_move.conflict[0] >= 'a' && parsed_move.conflict[0] <= 'h')
			        candidates = find_matches( candidates, parsed_move.conflict[0]);
            }
            else if ( parsed_move.conflict.length() > 1 ){
		        if (parsed_move.conflict[0] >= '1' && parsed_move.conflict[0] <= '8') {
			        candidates = find_matches( candidates, parsed_move.conflict[0] - '1' + 1);
                }
		        else if (parsed_move.conflict[0] >= 'a' && parsed_move.conflict[0] <= 'h'){
			        candidates = find_matches( candidates, parsed_move.conflict[0]);
                }
		        if (parsed_move.conflict[1] >= '1' && parsed_move.conflict[1] <= '8'){
			        candidates = find_matches( candidates, parsed_move.conflict[1] - '1' + 1);
                }
		        else if (parsed_move.conflict[1] >= 'a' && parsed_move.conflict[1] <= 'h'){
			        candidates = find_matches( candidates, parsed_move.conflict[1]);
                }
            }


	}
	

	// Filtering candidates on the basis of slope
	if (candidates.size() > 1)
		candidates = filterSlope(candidates, destination, parsed_move.type[0]);

	// Filtering candidates on the basis of having obstacle between Piece and Destination
	if (candidates.size() > 1)
		candidates = filterByObstacle(candidates, destination);


	if (candidates.size() < 1) 
		return;



	// Move the desired piece
	Piece moved_piece = *(candidates.begin());
	string reference_string = moved_piece.getType();
	string piece_color = "";
	piece_color.push_back(moved_piece.getColor());
	reference_string.insert(1, piece_color);
	map<string, Piece>::iterator it = configuration.find(reference_string);
	(*it).second.move(destination);

	// handle Promotion
	if (parsed_move.promotion != 'n') {
		new_dest = destination;
		capture(new_dest);
		insert_promoted_piece(parsed_move.promotion, destination, parsed_move.color);
	}
} 


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
				chpiece.conflict = "n";
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
		if (chpiece.is_castling == 0)
				chpiece.conflict = fetch_conflict(move);

		found = move.find("x");
		if(found != string::npos)
				chpiece.is_capture = 1;
		else
				chpiece.is_capture = 0;
		if(chpiece.promotion != 'n' && chpiece.is_capture == 1){
                move = move.erase(move.length() - 2, 2);
				chpiece.conflict = fetch_conflict(move);
        }
		return chpiece;
}

bool board::is_enpassant(cell destination) {
	map<string, Piece> piece_map = configuration;
	map <string, Piece>::iterator it;
	for (it = piece_map.begin(); it != piece_map.end(); it++)
		if ((*it).second.getRow() == destination.row && (*it).second.getColumn() == destination.column )
			return false;
	return true;
}

/*char board::fetch_conflict(string move) {
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
}*/

string board::fetch_conflict(string move) {
		int length = move.length();
		if(move[0]>='A' && move[0] <='Z')
				move = move.substr(1,move.length()-1);
		if(move[move.length() - 1] == '+' || move[move.length() - 1] == '#')
				move.erase(move.length()-1, 1);
	    move.erase(move.length()-2, 2);
		size_t found;
		found = move.find("x");
		if(found != string::npos) // means found
            move.erase(found,1);
        if (move.length() > 0 )
                return move;
        else
                return "n";
                            
}


void board::capture(cell destination){
    map<string,Piece>::iterator it;
    for ( it = configuration.begin() ; it != configuration.end() ; it++){
        if ( it->second.getColumn() == destination.column && it->second.getRow() == destination.row )
                configuration.erase(it);
            //it->second.toggleIsAlive();
        }
}

void board::do_castling(string type, char color) {
		map<string, Piece> init_map = configuration;
		cell destination;
		if (color == 'w') {
				if (type[0] == 'K') {
						for(map<string, Piece>::iterator it = init_map.begin(); it != init_map.end(); it++) {
								destination.row = ((*it).second).getRow();
								if( ((*it).second).getRow() == 1 && ((*it).second).getColumn() == 'e' ) {
										destination.column = 'g';
										((*it).second).move(destination);
								}
								if( ((*it).second).getRow() == 1 && ((*it).second).getColumn() == 'h' ) {
										destination.column = 'f';
										((*it).second).move(destination);
								}
						}
				}
				else if(type[0] == 'Q') {
						for(map<string, Piece>::iterator it = init_map.begin(); it != init_map.end(); it++) {
								destination.row = ((*it).second).getRow();
								if( ((*it).second).getRow() == 1 && ((*it).second).getColumn() == 'e' ) {
										destination.column = 'c';
										((*it).second).move(destination);
								}
								if( ((*it).second).getRow() == 1 && ((*it).second).getColumn() == 'a' ) {
										destination.column = 'd';
										((*it).second).move(destination);
								}
						}
				}
		}

		else {
				if (type[0] == 'K') {
						for(map<string, Piece>::iterator it = init_map.begin(); it != init_map.end(); it++) {
								destination.row = ((*it).second).getRow();
								if( ((*it).second).getRow() == 8 && ((*it).second).getColumn() == 'e' ) {
										destination.column = 'g';
										((*it).second).move(destination);
								}
								if( ((*it).second).getRow() == 8 && ((*it).second).getColumn() == 'h' ) {
										destination.column = 'f';
										((*it).second).move(destination);
								}
						}
				}
				else if(type[0] == 'Q') {
						for(map<string, Piece>::iterator it = init_map.begin(); it != init_map.end(); it++) {
								destination.row = ((*it).second).getRow();
								if( ((*it).second).getRow() == 8 && ((*it).second).getColumn() == 'e' ) {
										destination.column = 'c';
										((*it).second).move(destination);
								}
								if( ((*it).second).getRow() == 8 && ((*it).second).getColumn() == 'a' ) {
										destination.column = 'd';
										((*it).second).move(destination);
								}
						}
				}
		}
		configuration = init_map;
}

vector<Piece> board::get_valid_pawn_pieces (cell destination, char turn) {
		map <string, Piece>::iterator it;
		vector<Piece> pawn_vector;
		for (it = configuration.begin(); it != configuration.end(); it++) {
                string temp = it->second.getType();
				if (temp[0] == 'P' && (it)->second.getColor() == turn) {
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
				if ( temp[0] == piecetype && temp[1] == turn )
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

vector<Piece> board::filterSlope(vector<Piece> candidates, cell destination, char type){
		vector<Piece> Slopefilter;
		vector<Piece>::iterator it;

		for ( it = candidates.begin() ; it != candidates.end() ; it++){
				if ( type == 'P' ){
						if ( (abs(destination.row - (*it).getRow()) == 1 || abs(destination.row - (*it).getRow()) == 2) && abs(destination.column-(*it).getColumn()) == 0)
								Slopefilter.push_back(*it);
				}
				if ( type == 'K' ){
						if ( abs(destination.row - (*it).getRow()) == 1 ||  abs(destination.column-(*it).getColumn()) == 1)
								Slopefilter.push_back(*it);
				}
				if ( type == 'N' ){
						if ( (abs(destination.row - (*it).getRow()) == 1 && abs(destination.column - (*it).getColumn()) == 2) || (abs(destination.column-(*it).getColumn()) == 1 && abs(destination.row - (*it).getRow()) == 2))
								Slopefilter.push_back(*it);
				}
				if ( type == 'B' ){
						if ( abs(destination.row - (*it).getRow()) == abs(destination.column - (*it).getColumn()))
								Slopefilter.push_back(*it);
				}
				if ( type == 'R' ){
						if ( destination.column - (*it).getColumn() == 0 ||  destination.row - (*it).getRow() == 0 )
								Slopefilter.push_back(*it);
				}
				if ( type == 'Q' ){
						if ( destination.column - (*it).getColumn() == 0 ||  destination.row - (*it).getRow() == 0 || abs(destination.row - (*it).getRow()) == abs(destination.column - (*it).getColumn()))
								Slopefilter.push_back(*it);
				}
		}
		return Slopefilter;
}

vector<Piece> board::filterByObstacle(vector<Piece> candidates, cell destination){

        //check colinearity
        vector<Piece>::iterator pit;
		vector<Piece> updated_candidates;
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
                if ( mit == configuration.end() && !isFound ) {
                    updated_candidates.push_back(*pit);
					return updated_candidates;
				}
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
						if (it->first[2] > max_index)
								max_index = it->first[2];
				}
		}
		string promote_type_string = "";
        max_index++;
        string reference_string = "";
        reference_string += promote_type;
        reference_string += promote_color;
        reference_string += max_index;
        promote_type_string.push_back(promote_type);
        promote_type_string.push_back(max_index);
        Piece promoted_piece (tempDest, promote_type_string, promote_color);
        piece_map.insert(pair <string, Piece> (reference_string, promoted_piece));
        configuration = piece_map;
}


int main() {
    cout << "bhar main ja" << endl;
    board some_board;
	//some_board.display();
	some_board.tester();
	//some_board.display();
    //pos.column = 'a';
    //pos.row = 6;
    //chessPiece piece (pos,"P3",'w');
    //cout << "OOPS chal gaya"  << piece.getRow() << endl;
    //cout << "OOPS chal gaya"  << piece.getColor() << endl;
}
