#include <iostream>
#include <vector>
#include <string>

using namespace std;

typedef struct{
	bool is_chkmate;
	bool is_castling;
	char conflict;
        int row;
        char column;
        string type;
        char color;
        bool isAlive;
	bool is_capture;
	
}data_from_input;

data_from_input convert_str2data(string move, int turn);
char fetch_conflict (string move);

int main() {

	data_from_input chpiece = convert_str2data("O-O-O",4);
	cout << "chpiece.is_chkmate: " << chpiece.is_chkmate << "\n";
        cout << "chpiece.is_castling: " << chpiece.is_castling << "\n";
        cout << "chpiece.conflict: " << chpiece.conflict << "\n";
        cout << "chpiece.row: " << chpiece.row << "\n";
        cout << "chpiece.column: " << chpiece.column << "\n";
        cout << "chpiece.type: " << chpiece.type << "\n";
        cout << "chpiece.color: " << chpiece.color << "\n";
        cout << "chpiece.isAlive: " << chpiece.isAlive << "\n";
        cout << "chpiece.is_capture: " << chpiece.is_capture << "\n";

	return 0;

}

char fetch_conflict(string move) {
	int length = move.length();
	if(move[0]>='A' && move[0] <='Z')
		move = move.substr(1,move.length()-1);
	if(move[move.length() - 1] == '+')
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
	if (move[0] >= 'a' && move[0] <= 'h') {
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

	if(move[length - 1] == '+') {
		chpiece.is_chkmate = 1;
		chpiece.column = move[length - 3];
		chpiece.row = move[length - 2] - '1' + 1;
	}
	else if(move[0] != 'O') {
		chpiece.column = move[length - 2];
		chpiece.row = move[length - 1] - '1' + 1;
	}
	chpiece.isAlive = 1;
	if (chpiece.is_castling == 0)
		chpiece.conflict = fetch_conflict(move);
        size_t found;
        found = move.find("x");
        if(found != string::npos)
		chpiece.is_capture = 1;
	return chpiece;
}
