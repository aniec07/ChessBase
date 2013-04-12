#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <utility>

using namespace std;
/*
typedef struct{
        int row;
        char column;
        string type;
        char color;
        bool isAlive;
}Piece;
*/
map<string, Piece> do_castling(map<string, Piece> init_map, string type, char color);
/*
int main() {

	return 0;
}
*/
map<string, Piece> do_castling(map<string, Piece> init_map, string type, char color) {
	if (color == 'w') {
		if (type[0] == 'K') {
			for(map<string, Piece>::iterator it = init_map.begin(); it != init_map.end(); it++) {
				if( ((*it).second).row == 1 && ((*it).second).column == 'e' ) {
					((*it).second).column = 'g';
				}
				if( ((*it).second).row == 1 && ((*it).second).column == 'h' ) {
                                        ((*it).second).column = 'f';
                                }
			}
		}
		else if(type[0] == 'Q') {
                        for(map<string, Piece>::iterator it = init_map.begin(); it != init_map.end(); it++) {
                                if( ((*it).second).row == 1 && ((*it).second).column == 'e' ) {
                                        ((*it).second).column = 'c';
                                }
                                if( ((*it).second).row == 1 && ((*it).second).column == 'a' ) {
                                        ((*it).second).column = 'd';
                                }
                        }

		}
	}
	else {
                if (type[0] == 'K') {
			for(map<string, Piece>::iterator it = init_map.begin(); it != init_map.end(); it++) {
                                if( ((*it).second).row == 8 && ((*it).second).column == 'e' ) {
                                        ((*it).second).column = 'g';
                                }
                                if( ((*it).second).row == 8 && ((*it).second).column == 'h' ) {
                                        ((*it).second).column = 'f';
                                }
                        }

                }
                else if(type[0] == 'Q') {
			for(map<string, Piece>::iterator it = init_map.begin(); it != init_map.end(); it++) {
                                if( ((*it).second).row == 8 && ((*it).second).column == 'a' ) {
                                        ((*it).second).column = 'c';
                                }
                                if( ((*it).second).row == 8 && ((*it).second).column == 'h' ) {
                                        ((*it).second).column = 'd';
                                }
                        }
                }
	}

	return init_map;
}
