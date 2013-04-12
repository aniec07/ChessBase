void database::saveDatabase() {
	ofstream tempout;
	tempout.open("GamesDatabase.txt");

	for (map<int, gameDetails>::iterator it = gamesTable.begin(); it != gamesTable.end(); it++)
		tempout << ":" << (*it).first << ":" << (*it).second.whitePlayer << ":" << (*it).second.blackPlayer << ":" << (*it).second.result << ":" << endl;
	tempout.close();

	ofstream outfile;
	outfile.open("FenDatabase.txt");
	for (map<string, list<gameEntry> >::iterator it = fenTable.begin(); it != fenTable.end(); it++) {
		outfile << "@" << (*it).first << "@";
		for (list<gameEntry>::iterator it1 = (*it).second.begin(); it1 != (*it).second.end(); it1++)
			outfile << ":" << (*it1).gameId << ":" << (*it1).nextFEN << ":@";
		outfile << endl;
	}
	outfile.close();

}


void database::loadDatabase(char gameFile[], char fenFile[]) {
	ifstream gameRead, fenRead;
	gameRead.open(gameFile);
	fenRead.open(fenFile);
	while (gameRead.good()) {
		string move;
		getline(gameRead, move);
		if (move.size() > 0) {
			istringstream ss( move );
			vector<string> record;
			while (ss) {
				string s;
				if (!getline( ss, s, ':' ))
					break;
				record.push_back( s );
			}
			gameDetails tmp;
			int result, gameId;
			istringstream convert(record[1]);
			convert >> gameId;
			istringstream convert2(record[4]);
			convert2 >> result;
			tmp.whitePlayer = record[2];
			tmp.blackPlayer = record[3];
			tmp.result = result;
			gamesTable.insert( pair<int, gameDetails>(gameId, tmp) );
		}
	}
	gameRead.close();
	while (fenRead.good()) {
		string move;
		getline(fenRead, move);
		if (move.size() > 0) {
			istringstream ss( move );
			vector<string> fenRecord;
			string fen;
			while (ss) {
				string s;
				if (!getline( ss, s, '@' ))
					break;
				fenRecord.push_back( s );
			}
			fen = fenRecord[1];
			for (int i = 2; i < fenRecord.size(); i++) {
				istringstream ss1( fenRecord[i] );
				vector<string> gameRecord;
				while (ss1) {
					string s1;
					if (!getline( ss1, s1, ':' ))
						break;
					gameRecord.push_back( s1 );
				}
				string nextFEN;
				int gameId;
				istringstream convert(gameRecord[1]);
				convert >> gameId;
				gameEntry tmp;
				tmp.nextFEN = gameRecord[2];
				tmp.gameId = gameId;
				fenTable[fen].push_back(tmp);
			}
		}
	}
	fenRead.close();
}

void database::printdb() {
	cout << endl << "Testing : Size of fenTable = " << fenTable.size() << endl;
	for (map<string, list<gameEntry> >::iterator it = fenTable.begin(); it != fenTable.end(); it++) {
		cout << "For :" << (*it).first << ":" << endl;
		for (list<gameEntry>::iterator it1 = (*it).second.begin(); it1 != (*it).second.end(); it1++) {
			cout << "\t\t" << (*it1).gameId << " " << (*it1).nextFEN << endl;
		}
	}
}

bool database::isEndGame(gameEntry entry) {
	return (entry.nextFEN == "NULL");
}

// FOR GETIING nextConfiguration USING gameId AND fen
gameEntry database::nextConfiguration(gameEntry entry) {
	list<gameEntry> tempList;
	if (entry.nextFEN == "NULL") {
		cout << endl << "Error. Current game " << entry.gameId << " had ended";
		exit(1);
	}
	if (fenTable.find(entry.nextFEN) == fenTable.end()) {
		cout << endl << "Searching for invalid FEN" << endl;
		exit(1);
	}
	else
		tempList = fenTable[entry.nextFEN];
	list<gameEntry>::iterator it;
	for (it = tempList.begin(); it != tempList.end(); it++) {
		if ((*it).gameId == entry.gameId) {
			return (*it);
		}
	}
	if (it == tempList.end()) {
		cout << endl << "Error! Game Id Invalid" << endl;
		exit(1);
	}
}

// FOR GETIING nextConfiguration USING gameId AND fen
gameEntry database::nextConfiguration(string fen, int gameId) {
	list<gameEntry> tempList;
	if (fen == "NULL") {
		cout << endl << "Error. Current game " << gameId << " had ended" << endl;
		exit(1);
	}
	if (fenTable.find(fen) == fenTable.end()) {
		cout << endl << "Searching for invalid FEN" << endl;
		exit(1);
	}
	else
		tempList = fenTable[fen];
	list<gameEntry>::iterator it;
	for ( it = tempList.begin(); it != tempList.end(); it++) {
		if ((*it).gameId == gameId) {
			return (*it);
		}
	}
	if (it == tempList.end()) {
		cout << endl << "Error! Game Id Invalid" << endl;
		exit(1);
	}
}

// FOR GETTING nextConfiguration USING ONLY gameId
gameEntry database::nextConfiguration(int gameId) {
	list<gameEntry> tempList = fenTable["rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR/w"];
	list<gameEntry>::iterator it;
	for (it = tempList.begin(); it != tempList.end(); it++) {
		if ((*it).gameId == gameId) {
			return (*it);
		}
	}
	if (it == tempList.end()) {
		cout << endl << "Error! Game Id Invalid" << endl;
		exit(1);
	}
}

// FOR DISPLAYING queryFen RESULT
void database::queryFEN(string fen) {
	list<gameEntry>::iterator it;
	cout << endl << "The entered FEN can be found in " << fenTable[fen].size() << " games" << endl;
	for (it = fenTable[fen].begin(); it != fenTable[fen].end(); it++) {
		cout << "Game Id : " << (*it).gameId <<endl;
		cout << "White Player : " << gamesTable[(*it).gameId].whitePlayer << endl;
		cout << "Black Player : " << gamesTable[(*it).gameId].blackPlayer << endl;
		cout << "Result : ";
		if (gamesTable[(*it).gameId].result == 0)
			cout << "Draw" << endl;
		else if (gamesTable[(*it).gameId].result == 1)
			cout << gamesTable[(*it).gameId].whitePlayer << " won" << endl;
		else
			cout << gamesTable[(*it).gameId].blackPlayer << " won" << endl;
		cout << "\n";
	}
}

void database::fen2board(string fen){
	int i = 0;
	int h = NO_OF_ROWS;
	while(h > 0){
		cout << endl;
		cout << "---------------------------------" << endl;
		cout << "| " ;
		while(fen[i] != '/' && i != fen.length()){
			if (fen[i] >= ASCII_ZERO && fen[i] <= ASCII_NINE) {
				for ( int j = 0 ; j < charToInt(fen[i]) ; j++)
				cout << "  | " ;
			}
			else {
				cout << fen[i] << " | ";
			}
			i++;
		}
		h--;
		i++;
	}
	cout << endl;
	cout << "---------------------------------" << endl;
	cout << endl << fen << endl;
}

int database::charToInt(char s){
	int result;
	result = s - ASCII_ZERO;
	return result;
}

bool database::isFenPresent(string fen) {
	return (fenTable.find(fen) != fenTable.end());
}

bool database::isGameIdPresent(string fen, int gameId) {
	if (isFenPresent(fen)) {
		for (list<gameEntry>::iterator it = fenTable[fen].begin(); it != fenTable[fen].end(); it++)
			if ((*it).gameId == gameId)
				return 1;
		return 0;
	}
	else {
		cout << endl << "FEN not present or invalid FEN" << endl;
		return 0;
	}
}





void database::dispDetails(int GID) {
        if ( gamesTable.find(GID) != gamesTable.end() ){
                cout << "Game Id : " << GID <<endl;
                cout << "White Player :" << gamesTable[GID].whitePlayer << endl;
                cout << "Black Player :" << gamesTable[GID].blackPlayer << endl;
                cout << "Result :" << gamesTable[GID].result << endl;
                cout << "\n";
        }
        else
                cout << "Invalid GameID" << endl;
}



int database::fastestWin(string tempFEN){
        if ( fenTable.find(tempFEN) != fenTable.end() ){
                list<gameEntry> tempList = fenTable[tempFEN];
                char currentPlay = tempFEN[tempFEN.length()-1];
                int GID = -1;
                int GIDreturn = -1;
                int stopSearch;
                int traceCount;
                int lowestCount = 10000000;
                for ( list<gameEntry>::iterator it = tempList.begin() ; it != tempList.end() ; it++ ){
                        if ( (currentPlay == 'w' && (gamesTable.find((*it).gameId))->second.result == 1) || (currentPlay == 'b' && (gamesTable.find((*it).gameId))->second.result == 0 ) ){
                                stopSearch = 0;
                                traceCount = 0;
                                GID = (*it).gameId;
                                tempFEN = (*it).nextFEN;
                                while ( !stopSearch ){
                                        gameEntry gEnt = nextConfiguration(tempFEN,GID);
                                        traceCount++;
                                        if ( gEnt.nextFEN == "NULL" )// if nextconfig returns null then make stopsearch = 1; implement this!
                                                stopSearch = 1;
                                        else 
                                                tempFEN = gEnt.nextFEN;
                                }
                                if (traceCount < lowestCount){
                                        lowestCount = traceCount;
                                        GIDreturn = GID;
                                }
                        }
                }

                return GIDreturn;
        }
        return -1;
}   


int database::charFrequencyInString( string input, char c ){
        int frequency = 0;
        for ( int i = 0; i < input.length() ; i++ )
                if ( input[i] == c )
                        frequency++;
        return frequency;
}

int database::fastestCapture(string tempFEN, char typeOfPiece){
        if ( fenTable.find(tempFEN) != fenTable.end() ){
                list<gameEntry> tempList = fenTable[tempFEN];
                char currentPlay = tempFEN[tempFEN.length()-1];
                int GID = -1;
                int GIDreturn = -1;
                int stopSearch;
                int traceCount;
                int lowestCount = 10000000;
                int captureSearchTrue = 0;
                for ( list<gameEntry>::iterator it = tempList.begin() ; it != tempList.end() ; it++ ){
                        stopSearch = 0;
                        traceCount = 0;
                        captureSearchTrue = 0;
                        GID = (*it).gameId;
                        tempFEN = (*it).nextFEN;
                        while ( !stopSearch ){
                                gameEntry gEnt = nextConfiguration(tempFEN,GID);
                                traceCount++;
                                if ( gEnt.nextFEN == "NULL" )

                                        stopSearch = 1;

                                else if ( charFrequencyInString(tempFEN,typeOfPiece) - charFrequencyInString(gEnt.nextFEN,typeOfPiece) == 1 ){
                                        stopSearch = 1;
                                        captureSearchTrue = 1;
                                }

                                else 
                                        tempFEN = gEnt.nextFEN;
                        }
                        if ( traceCount < lowestCount && captureSearchTrue ){
                                lowestCount = traceCount;
                                GIDreturn = GID;
                        }
                }
                cout << endl;

                return GIDreturn;
        }
        return -1;
}
