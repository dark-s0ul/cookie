#include <iostream>
#include <string>

using namespace std;

char **table;
unsigned moves;
unsigned table_size = 0;

void reset() {
	moves = 0;

	table = new char*[table_size];
	for (int i = 0; i < table_size; i++) {
		table[i] = new char[table_size];
		memset(table[i], 0, table_size);
	}
}

char check() {
	for (int i = 0; i < table_size; i++) {
		for (int j = 0; j < table_size; j++) {
			if (table[i][j] == 0) { return 0; }
		}
	}
	return 1;
}

#define range(value, min, max) (min <= value && value <= max)
void move(int x, int y) {
	if (!range(x, 0, table_size - 1) || !range(y, 0, table_size - 1)) { return; }
	table[x][y] = table[x][y] == 0 ? 1 : 0;
}

void read() {
	string _move;
	printf("Moves: %u\nPlease enter your move: ", moves);
	getline(cin, _move);

	if (_move.length() == 0) return;

	if (_move[0] == '~') {
		int size = atoi(_move.c_str() + 1);
		if (range(size, 1, 9)) { table_size = size; }

		reset();
		return;
	}

	if (_move == "!") {
		for (int i = 0; i < table_size; i++) {
			for (int j = 0; j < table_size; j++) {
				table[i][j] = table[i][j] == 0 ? 1 : 0;
			}
		}
		return;
	}

	if (_move.length() != 2) { return; }

	int x = _move[0] - '1';
	int y = _move[1] - '1';

	if (!range(x, 0, table_size - 1) || !range(y, 0, table_size - 1)) { return; }

	moves++;

	move(x, y);
	move(x - 1, y);
	move(x + 1, y);
	move(x, y - 1);
	move(x, y + 1);
}

void draw() {
	system("cls");
	printf("   ");
	for (int i = 1; i <= table_size; i++) {
		printf("  %i ", i);
	}
	printf("\n");
	for (int i = 0; i < table_size; i++) {
		printf("   |");
		for (int i = 0; i < table_size; i++) {
			printf("---|");
		}
		printf("\n");
		printf(" %i ", i + 1);
		for (int j = 0; j < table_size; j++) {
			printf("| %c ", table[i][j] == 1 ? 'X' : ' ');
		}
		printf("|\n");
	}
	printf("   |");
	for (int i = 0; i < table_size; i++) {
		printf("---|", i);
	}
	printf("\n");
}

int main() {
	table_size = 5;

	while (true) {
		reset();

		while (check() == 0) {
			draw();
			read();
		}
		draw();
		printf("\nCongratulations!\nMoves: %u!\n", moves);
		system("pause");
	}
}
