#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <math.h>
#include <sstream>
#include <fstream>

using namespace std;

#include "structures"
#include "signatures"

int main(int argc, char *argv[]) {

        string FEN = engine(argv[1]);
	cout << "\n" << FEN << "\n";
        return 0;
}

string engine(char* filename) {

        string move;
        ifstream infile;
        infile.open(filename);

        //fen args
        string castling = "KQkq";
        char active_color;
        map<char,int> half_move;
        half_move['b'] = 0;
        half_move['w'] = 0;


        //variables
        cell destination;
	Piece current_Piece;

        #include "initial_map.cpp"
        int count = 1;
        while(infile.good()) {
                getline(infile, move);
                if (move.length() > 0) {
                        data_from_input raw = convert_str2data(move,count);

			if (raw.color == 'w') {
				active_color = 'b';
				half_move['w']++;
			}
			else {
				active_color = 'w';
				half_move['b']++;
			}

                        destination.row = raw.row;
                        destination.column = raw.column;

                        if (raw.is_castling) {
                                init_map = do_castling(init_map, raw.type, raw.color);
                                castling = fen_castling(castling, raw.type, raw.color);
                        }
                        else if (raw.is_capture) {
				if (is_capture_pawn(init_map, destination))
					half_move[raw.color] = 0;
                                current_Piece = get_Piece(init_map, raw.type, raw.color, destination, raw.conflict);
                                init_map = capture(init_map, destination);
                                init_map = update_map(init_map, current_Piece, destination);
                        }
                        else {
                                current_Piece = get_Piece(init_map, raw.type, raw.color, destination, raw.conflict);
				if(current_Piece.type[0] == 'P')
					half_move[raw.color] = 0;
                                init_map = update_map(init_map, current_Piece, destination);
                        }

			if(raw.promotion != 'n') {
				if(raw.is_capture == 0)
					init_map = capture(init_map, destination);
				insert_promoted_piece(init_map, raw.promotion , destination, raw.color);
			}

                        count++;
                }
        }
        infile.close();
        for (map<string, Piece>::iterator it = init_map.begin(); it != init_map.end(); it++) {
                if( ((*it).second).isAlive)
                        cout << "(" << ((*it).second).row << "," << ((*it).second).column << ") :" << ((*it).second).type << ((*it).second).color << endl;
        }                       
        string FEN = FEN_generator(init_map, active_color,castling,destination,half_move[current_Piece.color],count/2);
        return FEN;
}

#include "definitions.cpp"
