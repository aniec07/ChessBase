/*void database::printdb() {
	cout << endl << "Testing : Size of fenTable = " << fenTable.size() << endl;
	for(map<string, list<gameEntry> >::iterator it = fenTable.begin(); it != fenTable.end(); it++) {
		cout << "For :" << (*it).first << ":" << endl;
		for (list<gameEntry>::iterator it1 = (*it).second.begin(); it1 != (*it).second.end(); it1++) {
			cout << "\t\t" << (*it1).gameId << " " << (*it1).nextFEN << endl;
		}
	}
}*/

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
		cout << "White Player :" << gamesTable[(*it).gameId].whitePlayer << endl;
		cout << "Black Player :" << gamesTable[(*it).gameId].blackPlayer << endl;
		cout << "Result :" << gamesTable[(*it).gameId].result << endl;
		cout << "\n";
	}
}


// DISPLAY ONE GAMEDETAIL:     NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW 

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

// TRACE TILL END WINNER: NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW 


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











void database::fen2board(string fen){
	int i = 0;
	int h = 8;
	while(h > 0){
		cout << endl;
		cout << "---------------------------------" << endl;
		cout << "| " ;
		while(fen[i] != '/' && i != fen.length()){
			if (fen[i] > 47 && fen[i] < 58) {
				for ( int j = 0 ; j < stringToInt(fen[i]) ; j++)
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

int database::stringToInt(char s){
	int result;
	result = s - 48;
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