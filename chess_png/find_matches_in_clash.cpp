vector<Piece> find_matches(vector<Piece> init_list, char ch);
vector<Piece> find_matches(vector<Piece> init_list, int i);

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
