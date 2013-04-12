class chessPiece;
class chessBoard;
class database;

class chessPiece{
	cell position;
	string type;
	char color;
public:
	chessPiece(cell pos, string name, char col);
	void move(cell destination);
	int getRow();
	char getColumn();
	string getType();
	char getColor();
};

class chessBoard{
	map<string, chessPiece> configuration;
	gameDetails gameInfo;
	int gameId;
// File handling
	void initialize();
	void parseFile(string filename);
	int getNextGameId(database &current);
// Database Handling
	void updateGamesTable(database &current);
	void updateFenTable(database &current, string fen, string nextFEN);
// Fen Generator
	string fenGenerator();
	string intToString(int x);
	int get_index(char c);

// For Updating Board
	void update(string input, int turn);
	data_from_input convert_str2data(string move, int turn);
	bool is_enpassant(cell destination);
	string fetch_conflict(string move);
	void capture(cell destination);
	void do_castling(string type, char color);
	vector<chessPiece> get_valid_pawn_pieces (cell destination, char turn);
	vector<chessPiece> get_valid_all_pieces (char turn, char piecetype);
	vector<chessPiece> find_matches(vector<chessPiece> candidates, char ch);
	vector<chessPiece> find_matches(vector<chessPiece> candidates, int i);
	vector<chessPiece> filterSlope(vector<chessPiece> candidates, cell destination, char type);
	vector<chessPiece> filterByObstacle(vector<chessPiece> candidates, cell destination);
	double returnDist(chessPiece cfinal, cell cinitial);
	void insert_promoted_piece(char promote_type, cell destination, char promote_color);

public :
	chessBoard();
	void processPGN(string filename, database &current);
};

class database {
	map<string, list<gameEntry> > fenTable;
	map<int, gameDetails> gamesTable;

	int charToInt(char s);

	friend class chessBoard;
public :
	
	bool isEndGame(gameEntry entry);
	bool isGameIdPresent(string fen, int gameId);
	bool isFenPresent(string fen);
	void queryFEN(string fen);
	gameEntry nextConfiguration(string fen, int gameId);
	gameEntry nextConfiguration(int gameId);
	gameEntry nextConfiguration(gameEntry entry);
	void fen2board(string fen);
	void printdb();
	void loadDatabase(char gameFile[], char fenFile[]);
	void saveDatabase();
    void dispDetails(int GID);
    int fastestWin(string tempFEN);
    int fastestCapture(string tempFEN, char typeOfPiece);
    int charFrequencyInString(string input, char c);
};
