#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <math.h>
#include <utility>

using namespace std;

#include "structures.h"
#include "classes.h"


void UI(database &db);

int main(int argc, char *argv[]){

	if (argc != 2) {
		cout << "PGN file not entered.\nError. Exiting...\n";
		exit(1);
	}
	database current;
//
	ifstream infile;
	infile.open(argv[1]);
	ofstream outfile;
	bool isOneGame = false;
    int fileCount = 1;
	while(infile.good()) {
		string move;
		getline(infile, move);
        cout << "testing move=start" << move << "end" << endl;
		if (isOneGame == true && move.size() != 0) {
			outfile.open("onePGNfile.txt");
			isOneGame = false;
		}
		if (move.find(" 0-1") == string::npos && move.find(" 1-0") == string::npos && move.find(" 1/2-1/2") == string::npos)
			outfile << move << endl;
		else {
			isOneGame = true;
			outfile << move << endl;
			outfile.close();
			chessBoard temp;
     cout << "Before process" << endl;
			temp.processPGN("onePGNfile.txt", current);
     cout << "After process" << endl;
		}
        cout << "Making Data Base " << endl;
        cout << "File Number: " << fileCount << endl;
        fileCount++;
	}

    cout << "Finished making database: " << endl;
	outfile.close();
	infile.close();
	remove("onePGNfile.txt");
//	cout << "Database created\n";
//	cout << "Printing Db...\n";
//	current.printdb();
	UI(current);
}

void UI(database &db) {
	cout << string(50, '\n');
	cout << endl << "\t\t\t\tMAIN MENU" << endl;
	cout << endl << "Enter a FEN string to query for previous occurences" << endl;
	cout << "Format  : 1st row/2nd row/3rd row/4th row/5th row/6th row/ 7th row/8th row/active colour" << endl;
	cout << "Example : rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR/w" << endl;

	string inputFEN;
	cin >> inputFEN;
	int choice = 1;
	while ( choice != 0){
		if( choice == 2){
			cout << string(25, '\n');
			cout << endl << "MAIN MENU" << endl;
			cout << endl<< "Enter a FEN string to query for previous occurences" << endl;
			cout << "Format  : 1st row/2nd row/3rd row/4th row/5th row/6th row/ 7th row/8th row/active colour" << endl;
			cout << "Example : rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR/w" << endl;
			cin >> inputFEN;
		}
		if (db.isFenPresent(inputFEN))
			db.queryFEN(inputFEN);
		else {
			cout << endl << "FEN not present in database or Invalid FEN" << endl;
			choice = 2;
			continue;
		}
		cout << endl << "Enter a game-id to proceed" << endl;

		cin >> choice;
		if (!(db.isGameIdPresent(inputFEN, choice))) {
			cout << "The GameId is invalid for searched fen" << endl;
			continue;
		}

		string tempFEN = inputFEN;
		int gameId = choice;
		while ( choice != 0){
			cout << gameId << " HELLO " << endl;
			db.fen2board(tempFEN);
			cout << endl << "Press 1 for next configuration" << endl;
			cout << "Press 2 to view the game from beginning" << endl;
			cout << "Press 3 to trace game that makes the fastest win" <<endl;
			cout << "Press 0 to exit this game" << endl;
			int t;
			cin >> t;
			gameEntry game;
            int GID = db.fastestWin(tempFEN);
			switch(t){
				case 1:
						game = db.nextConfiguration(tempFEN,gameId);
						break;
				case 2:
						game = db.nextConfiguration(gameId);
						break;
				case 3:
						if ( GID != -1 ){
							game = db.nextConfiguration(GID);
							cout << "Game traced is: " << GID << "Won by: " ;
						    db.dispDetails(GID);
						}
						else
							cout << " No possible suggestion found " << endl;	
						break;
				case 0:
						choice = 0;
						break;
				default:
						break;
			}
			cout << endl;
			if (db.isEndGame(game)){
				cout << endl << "The game you are currently viewing has ended " << endl;
				break;
			}
			else if (t == 2) {
				tempFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR/w";
			}
			else if (choice != 0){
				cout << "ENTER" <<endl;
				cout << game.nextFEN << endl;
				db.fen2board(game.nextFEN);
				tempFEN = game.nextFEN;
			}
			else
				tempFEN = inputFEN;
		}
		cout << string(25, '\n');
		cout << endl << "Press 1 to view the previous search result." << endl;
		cout << "Press 2 to MAIN MENU" << endl;
		cout << "Press 0 to exit" << endl;
		cin >> choice;
	}
}

#include "chessBoardFunctions.cpp"
#include "chessPieceFunctions.cpp"
#include "databaseFunctions.cpp"
