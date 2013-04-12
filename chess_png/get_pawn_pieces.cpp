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
