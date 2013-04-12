chessBoard::chessBoard() {
	initialize();
}

void chessBoard::processPGN(string filename, database &current) {


cout << "ghus gaya process main" << endl;
	initialize();
	gameId = getGameId(current);
	parseFile(filename);
	updateGamesTable(current);
	ifstream infile;
	infile.open("temporaryPreProcessedFile.txt");
	string color = "w";
	string fen = fenGenerator();
	fen += "/";
	fen += color;
	//cout << endl << gameInfo.whitePlayer << ":" << gameInfo.blackPlayer << ":" << gameInfo.result << endl;
	while(!infile.eof()) {
		string move;
 cout << "before getline" << endl;
		getline(infile, move);
 cout << "after getline" << endl;
 cout << "color=" << color << endl;
		//cout << move << endl;
		if (move.size() > 1) {
			if (color == "w") {
				color = "b";
 cout << "before update" << endl;
				update(move, 1);
 cout << "after update" << endl;
			}
			else {
				color = "w";
//        cout << "before update move=starting" << move << "end" << endl;
				update(move, 0);
        cout << "after update" << endl;
			}
 cout << "after if else" << endl;

			string nextFEN = fenGenerator();
			nextFEN += "/";
			nextFEN += color;
            cout << "before updateFENTable" << endl;
			updateFenTable(current, fen, nextFEN);
            cout << "after updateFENTable" << endl;
			fen = nextFEN;
		}
	}
 cout << "nikla while main" << endl;
	updateFenTable(current, fen, "NULL");
	remove("temporaryPreProcessedFile.txt");
}















// Private Member functions
void chessBoard::updateFenTable(database &current, string fen, string nextFEN) {
	int isGamePresent = 0;
	for (list<gameEntry>::iterator it = current.fenTable[fen].begin(); it != current.fenTable[fen].end(); it++) {
		if ((*it).gameId == gameId) {
			(*it).nextFEN = nextFEN;
			isGamePresent = 1;
		}
	}
	if (isGamePresent == 0) {
		gameEntry entry;
		entry.gameId = gameId;
		entry.nextFEN = nextFEN;
		current.fenTable[fen].push_back(entry);
	}
}

void chessBoard::updateGamesTable(database &current) {
	current.gamesTable[gameId] = gameInfo;
}

int chessBoard::getGameId(database &current) {
	return (current.gamesTable.size() + 1);
}

void chessBoard::parseFile(string filename) {
	ifstream infile;
	char *a = (char*)filename.c_str();
	infile.open(a);
	ofstream outfile;
	outfile.open("temporaryPreProcessedFile.txt");
	string move;
	int count = 0;
	while(infile.good()) {
		getline(infile, move);
		if (count == 0 && move.find("[White ") != string::npos)
			count = 1;
		else if (count == 1 && move.find("[Black ") != string::npos)
			count = 2;
		else if (count == 2 && move.find("[Result ") != string::npos)
			count = 3;
		else if (move.find("[") != string::npos || move.size() == 0)
			continue;
		else
			count = 4;
		istringstream ss( move );
		vector<string> record;
		while (ss) {
			string s;
			if (!getline( ss, s, '"' )) break;
			record.push_back( s );
		}
		switch (count){
			case 1 :
				gameInfo.whitePlayer = record[1];
				break;
			case 2 :
				gameInfo.blackPlayer = record[1];
				break;
			case 3 :
				if (record[1] == "1-0")
					gameInfo.result = 1;
				else if (record[1] == "0-1")
					gameInfo.result = -1;
				else
					gameInfo.result = 0;
				break;
			default :
				istringstream turn(move);
				vector<string> fullMove;
				while (turn) {
					string s;
					if (!getline( turn, s, '.' )) break;
					fullMove.push_back( s );
				}
				for (int i = 0; i < fullMove.size(); i++) {
					if (fullMove[i].find(" ") != string::npos) {
						istringstream half_turn(fullMove[i]);
						vector<string> tmp;
						while (half_turn) {
							string s;
							if (!getline( half_turn, s, ' ' )) break;
							tmp.push_back( s );
						}
						if (tmp[0].size() > 0)
							outfile << tmp[0] << endl;
						if (tmp[1].size() > 0)
							outfile << tmp[1] << endl;
					}
				}
		}
	}
	infile.close();
	outfile.close();
}

void chessBoard::initialize() {
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
                                chessPiece p(pos,pieces[i],'w');
                                configuration.insert( pair<string, chessPiece>(wmap_pieces[i],p) );
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
                                chessPiece p(pos,t,'w');
                                configuration.insert( pair<string, chessPiece>(white_pawns[i], p) );
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
                                chessPiece p(pos, pieces[i], 'b');
                                configuration.insert( pair<string, chessPiece>(bmap_pieces[i], p) );
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
                                chessPiece p(pos, t, 'b');
                                configuration.insert(pair<string, chessPiece>(black_pawns[i], p) );
                }
}

string chessBoard::fenGenerator(){
	string FEN_state;
	map<string,chessPiece>::iterator it = configuration.begin();
	//initialising the board with empty cells
	vector<vector<char> > boardvector(8,vector<char>(8));
	for ( int i = 0 ; i < 8 ; i++){
		for ( int j = 0 ; j < 8 ;j++){
			boardvector[i][j] = '0';
		}
	}
	//creating the board vector with the data given from updated map
	for ( it = configuration.begin() ; it != configuration.end() ; it++){
		//if(it->second.isAlive){
			int col = get_index(it->second.getColumn());
			int row = it->second.getRow();
			string p_type = it->second.getType();
			if ( it->second.getColor() == 'w')
				boardvector[row-1][col] = p_type.at(0);
			if ( it->second.getColor() == 'b')
				boardvector[row-1][col] = tolower(p_type.at(0));
	//	}
	}

	for ( int i = 7 ; i >= 0 ; i--){
		bool inBlank = false;
		int count = 0;
		for ( int j = 0 ; j < 8 ; j++){
			if ( boardvector[i][j] == '0' && inBlank == false){
				inBlank = true;
				count++;
			}
			else if ( boardvector[i][j] == '0' && inBlank == true){
				count++;
			}
			else if ( boardvector[i][j] != '0' && inBlank == false)
				FEN_state += boardvector[i][j];
			else if ( boardvector[i][j] != '0' && inBlank == true){
				string blank_count = intToString(count);
				FEN_state += blank_count;
				FEN_state += boardvector[i][j];
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
	FEN_state.erase(FEN_state.length() - 1, 1);
	//FEN_state += " ";
	return FEN_state;
		
}

string chessBoard::intToString(int x){
	ostringstream convert;
	convert << x;
	string str = convert.str();
	return str;
}

int chessBoard::get_index(char c){
	char alpha[8] = { 'a','b','c','d','e','f','g','h'};
	for ( int i = 0 ; i < 8 ; i++){
		if ( alpha[i] == c )
			return i ;
	}
}

// For Updating Board Alone
void chessBoard::update(string input, int turn) {
cout << "ghus gaya with" << input << endl;
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
		
cout << "is capture" << parsed_move.is_capture << endl;
cout << "new dest row=" << new_dest.row << endl;
cout << "new dest col=" << new_dest.column << endl;
	// Handle Capture
	if (parsed_move.is_capture)
		capture(new_dest);

cout << "castling se pehl;e tak theek h" << endl;
	// Handle Castling
	if (parsed_move.is_castling) {
		do_castling(parsed_move.type, parsed_move.color);
		return;
	}

	vector <chessPiece> candidates;

	// Getting candidate pieces
	if (parsed_move.type[0] == 'P')
		candidates = get_valid_pawn_pieces (destination, turn_char);
	else
		candidates = get_valid_all_pieces(turn_char, parsed_move.type[0]);
	
	// Filtering candidates in case of an conflict
	if (candidates.size() > 1) {
		if (parsed_move.conflict >= '1' && parsed_move.conflict <= '8')
			candidates = find_matches( candidates, parsed_move.conflict - '1' + 1);
		else if (parsed_move.conflict >= 'a' && parsed_move.conflict <= 'h')
			candidates = find_matches( candidates, parsed_move.conflict);
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
	chessPiece moved_piece = *(candidates.begin());
	string reference_string = moved_piece.getType();
	string piece_color = "";
	piece_color.push_back(moved_piece.getColor());
	reference_string.insert(1, piece_color);
	map<string, chessPiece>::iterator it = configuration.find(reference_string);
	(*it).second.move(destination);

	// handle Promotion
	if (parsed_move.promotion != 'n') {
		new_dest = destination;
		capture(new_dest);
		insert_promoted_piece(parsed_move.promotion, destination, parsed_move.color);
	}
if(input.compare("bxc4") == 0)
cout << "nikal gaya with bxc4" << endl;
} 


data_from_input chessBoard::convert_str2data(string move, int turn) {
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

bool chessBoard::is_enpassant(cell destination) {
	map<string, chessPiece> piece_map = configuration;
	map <string, chessPiece>::iterator it;
	for (it = piece_map.begin(); it != piece_map.end(); it++)
		if ((*it).second.getRow() == destination.row && (*it).second.getColumn() == destination.column )
			return false;
	return true;
}

char chessBoard::fetch_conflict(string move) {
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

void chessBoard::capture(cell destination){
    map<string,chessPiece>::iterator it;
    for ( it = configuration.begin() ; it != configuration.end() ; it++){
        if ( it->second.getColumn() == destination.column && it->second.getRow() == destination.row )
                configuration.erase(it);
        }
}

void chessBoard::do_castling(string type, char color) {
		map<string, chessPiece> init_map = configuration;
		cell destination;
		if (color == 'w') {
				if (type[0] == 'K') {
						for(map<string, chessPiece>::iterator it = init_map.begin(); it != init_map.end(); it++) {
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
						for(map<string, chessPiece>::iterator it = init_map.begin(); it != init_map.end(); it++) {
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
						for(map<string, chessPiece>::iterator it = init_map.begin(); it != init_map.end(); it++) {
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
						for(map<string, chessPiece>::iterator it = init_map.begin(); it != init_map.end(); it++) {
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

vector<chessPiece> chessBoard::get_valid_pawn_pieces (cell destination, char turn) {
		map <string, chessPiece>::iterator it;
		vector<chessPiece> pawn_vector;
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


vector<chessPiece> chessBoard::get_valid_all_pieces (char turn, char piecetype) {
		vector<chessPiece> all_pieces;
		map <string, chessPiece>::iterator it;
		for ( it = configuration.begin() ; it != configuration.end() ; it++ ){
				string temp = (*it).first;
				if ( temp[0] == piecetype && temp[1] == turn )
						all_pieces.push_back((*it).second);
		}
		return all_pieces;
}


vector<chessPiece> chessBoard::find_matches(vector<chessPiece> candidates, char ch) {

		vector<chessPiece> matches;
		for (vector<chessPiece>::iterator it = candidates.begin(); it != candidates.end(); it++)
				if ((*it).getColumn() == ch)
						matches.push_back(*it);
		return matches;
}

vector<chessPiece> chessBoard::find_matches(vector<chessPiece> candidates, int i) {

		vector<chessPiece> matches;
		for (vector<chessPiece>::iterator it = candidates.begin(); it != candidates.end(); it++)
				if ((*it).getRow() == i)
						matches.push_back(*it);
		return matches;

}

vector<chessPiece> chessBoard::filterSlope(vector<chessPiece> candidates, cell destination, char type){
		vector<chessPiece> Slopefilter;
		vector<chessPiece>::iterator it;

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

vector<chessPiece> chessBoard::filterByObstacle(vector<chessPiece> candidates, cell destination){

        //check collinearity
        vector<chessPiece>::iterator pit;
		vector<chessPiece> updated_candidates;
        map <string, chessPiece>::iterator mit;
        cell piece_position;
        bool isFound;
        for ( pit = candidates.begin() ; pit != candidates.end() ; pit ++ ){
            isFound = false;
            for ( mit = configuration.begin() ; mit != configuration.end() && !isFound ; mit ++ ){
                piece_position.row = (*pit).getRow();
                piece_position.column = (*pit).getColumn();
                    if ( (*pit).getType().compare((mit->second).getType()) != 0 || (*pit).getColor() != (mit->second).getColor() ){
                        if( returnDist(*pit,destination) == returnDist(mit->second,destination) + returnDist(mit->second, piece_position) ){   // obstacle present
                                isFound = true;
                        }
                    }
            }
                if ( !isFound ) {
                    updated_candidates.push_back(*pit);
					return updated_candidates;
				}
        }
}


double chessBoard::returnDist(chessPiece cfinal, cell cinitial){
		double dist =pow(pow((cfinal.getRow() - cinitial.row),2)+pow((cfinal.getColumn() - cinitial.column),2),0.5);
		return dist;
}

void chessBoard::insert_promoted_piece(char promote_type, cell destination, char promote_color) {
		map <string, chessPiece> piece_map = configuration;
		map <string, chessPiece>::iterator it;
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
        chessPiece promoted_piece (tempDest, promote_type_string, promote_color);
        piece_map.insert(pair <string, chessPiece> (reference_string, promoted_piece));
        configuration = piece_map;
}
