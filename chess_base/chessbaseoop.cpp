#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>

using namespace std;

typedef struct {
        int row;
        char column;
} cell;

typedef struct {
        string whitePlayerName;
        string blackPlayerName;
        int result;
}game;

typedef struct {
        int gameId;
        string nextFEN;
}game_entry;

class chessPiece{
    private :
        cell position;
        string type;
        char color;

    public:
        chessPiece(cell pos, string type, char color);
        void movePiece(cell destination);
        int getRow();
        char getColumn();
        string getType();
        char getColor();
};

chessPiece::chessPiece(cell pos, string type_temp, char col) {
        position.row = pos.row;
        position.column = pos.column;
        type = type_temp;
        color = col;
}


char chessPiece::getColor() {
        return color;
}

string chessPiece::getType() {
        return type;
}

int chessPiece::getRow() {
        return (position.row);
}

char chessPiece::getColumn() {
        return (position.column);
}


class database {
        map<string, list<game_entry> > gamesDatabase;
        map<int, game> gamesTable;
        public :
        void updateDatabase();
};

class board{
        map<string, chessPiece> configuration;
        string FEN_generator();
        public :
        board();
        void updateBoard(string pgn_move, int turn);
        void displayBoard();
        void removeChessPiece(string key);
        void insertChessPiece(string type, cell position, char color);
        friend void database::updateDatabase();
};

     
int main() {
    cout << "bhar main ja" << endl;
    cell pos;
    pos.column = 'a';
    pos.row = 6;
    chessPiece piece (pos,"P3",'w');
    cout << "OOPS chal gaya"  << piece.getRow() << endl;
    cout << "OOPS chal gaya"  << piece.getColor() << endl;
}
