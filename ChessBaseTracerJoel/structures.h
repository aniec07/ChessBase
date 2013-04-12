typedef struct {
	int row;
	char column;
} cell;

typedef struct {
	string whitePlayer;
	string blackPlayer;
	int result;
}gameDetails;

typedef struct {
	int gameId;
	string nextFEN;
}gameEntry;

typedef struct{
	bool is_chkmate;
	bool is_castling;
	char promotion;
	string conflict;
	int row;
	char column;
	string type;
	char color;
	bool is_capture;
}data_from_input;
