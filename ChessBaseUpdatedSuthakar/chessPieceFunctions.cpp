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

chessPiece::chessPiece(cell pos, string name, char col) {
	position.row = pos.row;
	position.column = pos.column;
	type = name;
	color = col;
}

void chessPiece::move(cell destination) {
	position.row = destination.row;
	position.column = destination.column;
}
