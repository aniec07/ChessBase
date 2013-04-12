char fetch_conflict(string move) {
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

data_from_input convert_str2data(string move, int turn) {
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
    if(chpiece.promotion != 'n' && chpiece.is_capture == 1)
	chpiece.conflict = move[0];
    return chpiece;
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

string fen_castling(string castling, string type, char color) {
        if (type[0] == 'K') {
                if (color == 'w')
                        castling[0] = '-';
                else
                        castling[2] = '-';
        }
        else {
                if (color == 'w')
                        castling[1] = '-';
                else
                        castling[3] = '-';
        }

        return castling;
}

Piece get_Piece(map<string, Piece> init_map, string type, char color, cell destination, char conflict) {

        Piece current_Piece;
        vector<Piece> candidates;
        if (type[0] == 'P') {
                candidates = get_valid_pawn_pieces(init_map, destination, color);
                if (conflict == 'n') {
                        candidates =  filterSlope(candidates, destination, type[0]);
                        if (candidates.size() > 1) {
                                current_Piece = filterDistance(candidates, destination, type[0]);
                        }
                        else if (candidates.size() == 1)
                                current_Piece = candidates[0];
                        return current_Piece;
                }
                else {
                        if (conflict >= '1' && conflict <= '8') {
                                int tmp = conflict - '1' + 1;
                                current_Piece = find_matches(candidates, tmp);
                        }
                        else
                                current_Piece = find_matches(candidates, conflict);
                        return current_Piece;
                }
        }
        else {
                candidates = get_valid_all_pieces(init_map, color, type[0]);
                if (conflict == 'n') {
                        candidates =  filterSlope(candidates, destination, type[0]);
                        if (candidates.size() > 1) {
                                current_Piece = filterDistance(candidates, destination, type[0]);
                        }
                        else if (candidates.size() == 1)
                                current_Piece = candidates[0];
                        return current_Piece;
                }
                else {
                        if (conflict >= '1' && conflict <= '8') {
                                int tmp = conflict - '1' + 1;
                                current_Piece = find_matches(candidates, tmp);
                        }
                        else
                                current_Piece = find_matches(candidates, conflict);
                        return current_Piece;
                }

        }

}

vector<Piece> get_valid_pawn_pieces (map <string, Piece> init_map, cell destination, char color) {
    map <string, Piece>::iterator it;
    vector<Piece> pawn_vector;
    for (it = init_map.begin(); it != init_map.end(); it++) {
        if ((it)->second.type[0] == 'P' && (it)->second.isAlive && (it)->second.color == color) {
            if(color == 'w') {
                if ((it)->second.row - destination.row == -1 || (it)->second.row - destination.row == -2)
                    pawn_vector.push_back((it) -> second);
            }
            if(color == 'b') {
                if ((it)->second.row - destination.row == 1 || (it)->second.row - destination.row == 2)
                    pawn_vector.push_back((it) -> second);
            }
        }
    }
    return pawn_vector;
}

vector<Piece> get_valid_all_pieces (map <string, Piece> init_map, char color, char type) {
    vector<Piece> all_pieces;
    map <string, Piece>::iterator it;
    for ( it = init_map.begin() ; it != init_map.end() ; it++ ){
        string temp = (*it).first;
        if ( temp[0] == type && temp[1] == color && (*it).second.isAlive )
            all_pieces.push_back((*it).second);
    }
    return all_pieces;
}


Piece find_matches(vector<Piece> init_list, char ch) {

    for (vector<Piece>::iterator it = init_list.begin(); it != init_list.end(); it++)
        if ((*it).column == ch)
            return (*it);
}

Piece find_matches(vector<Piece> init_list, int i) {

        for (vector<Piece>::iterator it = init_list.begin(); it != init_list.end(); it++)
                if ((*it).row == i)
                        return(*it);

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

Piece filterDistance(vector<Piece> candidates, cell destination, char type){
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
    return mindist_piece;
}

double returnDist(Piece cfinal, cell cinitial){
    double dist =pow(pow((cfinal.row - cinitial.row),2)+pow((cfinal.column - cinitial.column),2),0.5);
    return dist;
}

int abs(int i) {
        if (i < 0)
                return -i;
        else
                return i;
}

map<string, Piece> capture(map<string,Piece> init_map, cell destination) {
    map<string,Piece>::iterator it;
    Piece temp;
    for ( it = init_map.begin() ; it != init_map.end() ; it++){
        temp = (*it).second;
        if ( temp.column == destination.column && temp.row == destination.row)
            (*it).second.isAlive = false;
        }
    return init_map;
}

map<string, Piece> update_map(map<string, Piece> init_map, Piece current_Piece, cell destination) {

        string color = "";
        color += current_Piece.color;

        current_Piece.type.insert(1, color);
        init_map[current_Piece.type].row = destination.row;
        init_map[current_Piece.type].column = destination.column;

        return init_map;
}

bool is_capture_pawn(map<string, Piece> init_map, cell destination) {

	for(map<string, Piece>::iterator it = init_map.begin(); it != init_map.end(); it++)
		if (destination.row == (*it).second.row && destination.column == (*it).second.column)
			return ((*it).second.type[0] == 'P');

}

map <string, Piece> insert_promoted_piece(map <string, Piece> init_map, char promote_type, cell destination, char promote_color) {

	map <string, Piece>::iterator it;

	char max_index = '0';

	Piece promoted_piece;

	promoted_piece.row = destination.row;

	promoted_piece.column = destination.column;

	promoted_piece.color = promote_color;

	promoted_piece.type = promote_type;

	promoted_piece.isAlive = true;

	for (it = init_map.begin(); it != init_map.end(); it++) {

		if (it->first[0] == promote_type && it->first[1] == promote_color) {

			if (!(it->second.isAlive))

				break;

			if (it->first[2] > max_index)

				max_index = it->first[2];

		}

	}

	if (it == init_map.end()) {

		max_index ++;

		string reference_string = "";

		reference_string += promote_type;

		reference_string += promote_color;

		reference_string += max_index;

		init_map[reference_string] = promoted_piece;

	}

	else it->second = promoted_piece;

}

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
