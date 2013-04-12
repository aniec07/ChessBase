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
	cout << string(50, '\n');
	cout << endl << "\t\t\t\tMAIN MENU" << endl;
	cout << endl << "Enter a FEN string to query for previous occurences" << endl;
	cout << "Format  : 1st row/2nd row/3rd row/4th row/5th row/6th row/ 7th row/8th row/active colour" << endl;
	cout << "Example : rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR/w" << endl;
	cout << ">";

	string inputFEN;
	cin >> inputFEN;
	int choice = 1;
	while ( choice == 1 || choice == 2){
		if (choice == 2){
			cout << string(25, '\n');
			cout << endl << "MAIN MENU" << endl;
			cout << endl<< "Enter a FEN string to query for previous occurences" << endl;
			cout << "Format  : 1st row/2nd row/3rd row/4th row/5th row/6th row/ 7th row/8th row/active colour" << endl;
			cout << "Example : rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR/w" << endl;
			cout << ">";
			cin >> inputFEN;
		}
		if (db.isFenPresent(inputFEN))
			db.queryFEN(inputFEN);
		else {
			cout << endl << "FEN not present in database or Invalid FEN" << endl;
			choice = 2;
			continue;
		}

        int GID = db.fastestWin(inputFEN);
        if ( GID != -1 ){
                cout << " Game leading to fastest win is: " << endl;
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
		cin >> choice;
		if (!(db.isGameIdPresent(inputFEN, choice))) {
			cout << "The GameId is invalid for searched fen" << endl;
			continue;
		}


		string tempFEN = inputFEN;
		int gameId = choice;
		bool isWrongOption = false;
		while ( choice != 0){
			db.fen2board(tempFEN);
			cout << endl << "Press 1 for next configuration" << endl;
			cout << "Press 2 to view the game from beginning" << endl;
			cout << "Press 0 to exit this game" << endl;
			cout << ">";
			string subChoice;
			cin >> subChoice;
			gameEntry game;
			if (subChoice == "1")
				game = db.nextConfiguration(tempFEN,gameId);
			else if (subChoice == "2")
				game = db.nextConfiguration(gameId);
			else if (subChoice == "0")
						choice = 0;
			else {
						cout << endl << "Wrong option" << endl;
						isWrongOption = true;
			}
			cout << endl;
			if (db.isEndGame(game)){
				cout << endl << "The game you are currently viewing has ended " << endl;
				break;
			}
			else if (subChoice == "2") {
				tempFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR/w";
			}
			else if (choice != 0 && isWrongOption == false){
				cout << game.nextFEN << endl;
				db.fen2board(game.nextFEN);
				tempFEN = game.nextFEN;
			}
			else if (isWrongOption == true) {
				continue;
			}
			else
				tempFEN = inputFEN;
		}
		cout << string(25, '\n');
		cout << endl << "Press 1 to view the previous search result." << endl;
		cout << "Press 2 to MAIN MENU" << endl;
		cout << "Press any key to exit" << endl << ">";
		cin >> choice;
	}
}

#include "chessBoardFunctions.cpp"
#include "chessPieceFunctions.cpp"
#include "databaseFunctions.cpp"
