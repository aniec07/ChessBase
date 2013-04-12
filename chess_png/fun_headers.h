vector<Piece> find_matches(vector<Piece> init_list, char ch);
vector<Piece> find_matches(vector<Piece> init_list, int i);
vector<Piece> get_valid_pawn_pieces (map <string, Piece> piece_struct, cell destination, char turn);

string FEN_generator(map<string, Piece>,char active_color,string castling_availability,cell enpassant_target, int halfMoves,int fullMoves);
