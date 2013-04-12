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

const string MAIN_MENU = "2";
const string SEARCH_RESULT = "1";
const string QUIT = "q";
const string NEXT_CONFIG = "1";
const string RESTART_GAME = "2";
const string EXIT_GAME = "q";

void UI(database &db);

int main(int argc, char *argv[]){

	database current;
	char gameFile[] = "GamesDatabase.txt", fenFile[] = "FenDatabase.txt";
	current.loadDatabase(gameFile, fenFile);

	if (argc != 2 && argc != 1) {
		cout << "Wrong number of arguements.\n0 arguements for cached database.\n1 arguement for new PGN file\nError. Exiting...\n";
		exit(1);
	}
	if (argc == 2) {
		ifstream infile;
		infile.open(argv[1]);
		ofstream outfile;
		outfile.open("onePGNfile.txt");
		bool isOneGame = false;
		while(infile.good()) {
			string move;
			getline(infile, move);
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
				temp.processPGN("onePGNfile.txt", current);
			}
		}
		outfile.close();
		infile.close();
		remove("onePGNfile.txt");
		current.saveDatabase();
	}
//	cout << "Database created\n";
//	cout << "Printing Db...\n";
//	current.printdb();
	UI(current);
}

void UI(database &db) {
	string inputFEN;
	string choice = MAIN_MENU;

	while ( choice != QUIT){
		int gameId;
		if (choice == MAIN_MENU){
			cout << string(25, '\n');
			cout << endl << "\t\t\t\tMAIN MENU" << endl;
			cout << endl << "Enter a FEN string to query for previous occurences" << endl;
			cout << "Format  : 1st row/2nd row/3rd row/4th row/5th row/6th row/ 7th row/8th row/active colour" << endl;
			cout << "Example : rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR/w" << endl;
			cout << endl << "OR" << endl;
			cout << endl << "Enter 'q' to quit" << endl;
			cout << ">";
			cin >> inputFEN;
			if (inputFEN == "q")
				choice = QUIT;
			else if (db.isFenPresent(inputFEN)) {
				choice = SEARCH_RESULT;
			}
			else {
				cout << endl << "FEN not present in database or Invalid FEN" << endl;
				continue;
			}
		}
        


		if (choice == SEARCH_RESULT) {
            db.queryFEN(inputFEN);
            int GID = db.fastestWin(inputFEN);
            if ( GID != -1 ){
                cout << endl << "Game leading to fastest win is: " << endl;    
                db.dispDetails(GID);
            }
            
            char captureChoice;
            cout << "Do you want to view a game that leads to fastest capture of a Piece?: (y/n)  ";
            cin >> captureChoice;
            if ( captureChoice == 'y' ) {
                cout << "Enter the Piece Type that you want captured: " ;
                cin >> captureChoice;
                if ( db.charFrequencyInString(inputFEN,captureChoice) > 0 && db.fastestCapture(inputFEN,captureChoice) != -1 ){
                    cout << "Game leading to fastest capture of " << captureChoice << " is " << endl;
                    db.dispDetails(db.fastestCapture(inputFEN,captureChoice));
                }
                else
                    cout << "Unsuccessful search result" << endl;
            }

			cout << endl << "Enter a game-id to proceed" << endl;
			cout << ">";
			cin >> gameId;
			if (!(db.isGameIdPresent(inputFEN, gameId))) {
				cout << "The GameId is invalid for searched fen" << endl;
				continue;
			}
			string tempFEN = inputFEN;
			db.fen2board(tempFEN);
			string subChoice = "1";
			while ( subChoice != EXIT_GAME){
				cout << endl << "Press '1' for next configuration" << endl;
				cout << "Press '2' to view the game from beginning" << endl;
				cout << "Press 'q' to exit this game" << endl;
				cout << ">";
				cin >> subChoice;
				gameEntry game;
				if (subChoice == NEXT_CONFIG) {
					game = db.nextConfiguration(tempFEN,gameId);
					if (db.isEndGame(game)){
						cout << endl << "The game you are currently viewing has ended " << endl;
						break;
					}
					else {
						cout << game.nextFEN << endl;
						db.fen2board(game.nextFEN);
						tempFEN = game.nextFEN;
					}

				}
				else if (subChoice == RESTART_GAME) {
					tempFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR/w";
					db.fen2board(tempFEN);
				}
				else if (subChoice != EXIT_GAME){
					cout << endl << "Wrong option" << endl;
					db.fen2board(tempFEN);
					continue;
				}
				cout << endl;
			}
			do {
				cout << string(25, '\n');
				cout << endl << "Press '1' to view the previous search result." << endl;
				cout << "Press '2' to MAIN MENU" << endl;
				cout << "Press 'q' to exit" << endl << ">";
				cin >> choice;
				if (choice != QUIT && choice != MAIN_MENU && choice != SEARCH_RESULT) {
					cout << endl << "Wrong option" << endl;
				}
			} while (choice != QUIT && choice != MAIN_MENU && choice != SEARCH_RESULT);
		}
		
	}
}

#include "chessBoardFunctions.cpp"
#include "chessPieceFunctions.cpp"
#include "databaseFunctions.cpp"
