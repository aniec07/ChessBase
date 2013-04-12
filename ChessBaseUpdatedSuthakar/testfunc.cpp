#include <iostream>

using namespace std;

int main(){

        int turn = 99;
        char turn_char = 'b' + ( (turn % 2) * ('w' - 'b') );
        int oldrow = 5;
        int PREVIOUS_ROW = 1;
        int row = oldrow + 2 * ( ( ('w' - turn_char)/('w' - 'b') ) - 0.5 ) * PREVIOUS_ROW ; 
        cout << "The turn is: " << turn << " And the color is : " << turn_char << " The changed row is " << row << endl;
        return 0;
}
