#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <conio.h>
#include "Graphics.h"
#include "GameField.h"
#include "Bot.h"

using namespace std;

typedef unsigned u;

const int sz = 64;
GameField arr;
char minimap[sz / 4][sz / 4];
int scoreme = 0, scorehim = 0, targets = 0;
pair <int, int> me, him;
ConsoleGUI p;

inline bool ok(int i, int j) {
	return i >= 0 && i < sz && j >= 0 && j < sz && (arr.get(i, j) == ' ' || arr.get(i, j) == '*');
}

void drawcell(int i, int j) {
	int wall = 0, space = 0;
	bool hero = 0, enemy = 0, star = 0;
	for (int k = 0; k < 4; k++) {
		for (int l = 0; l < 4; l++) {
			if (arr.get(i * 4 + k, j * 4 + l) == 'I') {
				hero = 1;
			}
			else if (arr.get(i * 4 + k, j * 4 + l) == 'H') {
				enemy = 1;
			}
			else if (arr.get(i * 4 + k, j * 4 + l) == '*') {
				star = 1;
			}
			else if (arr.get(i * 4 + k, j * 4 + l) == '@') {
				wall++;
			}
			else if (arr.get(i * 4 + k, j * 4 + l) == ' ') {
				space++;
			}
		}
	}
	if (hero) {
		minimap[i][j] = 'I';
	}
	else if (enemy) {
		minimap[i][j] = 'H';
	}
	else if (star) {
		minimap[i][j] = '*';
	}
	else if (wall > space) {
		minimap[i][j] = '@';
	}
	else {
		minimap[i][j] = ' ';
	}
}

void drawmap() {
	for (int i = 0; i < sz / 4; i++) {
		for (int j = 0; j < sz / 4; j++) {
			drawcell(i, j);
		}
	}
}

void printchar(char c) {
	if (c == '|' || c == '=' || c == '#') {
		p.print(c, RED, BLACK);
	}
	else if (c == 'I' || c == 'H') {
		p.print(c, WHITE, BLACK);
	}
	else if (c == '@') {
		p.print(c, BROWN, BLACK);
	}
	else if (c == ' ') {
		p.print(c, BLACK, BLACK);
	}
	else {
		p.print(c, WHITE, BLACK);
	}
}

vector <string> old;

void print(bool isfirst) {
	vector <string> now;
	string str = "SCORE  ME " + to_string(scoreme) + " : " + to_string(scorehim) + " HIM";
	now.push_back(str);
	now.push_back("##====================##================##");
	pair <int, int> cell = { me.first / 4 - 2, me.second / 4 - 2 };
	for (int i = 0; i < 20; i++) {
		str = "||";
		for (int j = 0; j < 20; j++) {
			str += arr.get(cell.first * 4 + i, cell.second * 4 + j);
		}
		if (i < 16) {
			str += "||";
			for (int j = 0; j < sz / 4; j++) {
				str += minimap[i][j];
			}
			str += "||";
		}
		else if (i == 16) {
			str += "##================##";
		}
		else {
			str += "||";
		}
		now.push_back(str);
	}
	now.push_back("##====================##");
	if (isfirst) {
		for (auto &i : now) {
			for (auto &j : i) {
				printchar(j);
			}
			printchar('\n');
		}
	}
	else {
		for (u i = 0; i < now.size(); i++) {
			for (u j = 0; j < now[i].size(); j++) {
				if (now[i][j] != old[i][j]) {
					p.set_pos({ j,i });
					printchar(now[i][j]);
				}
			}
		}
	}
	old = now;
	p.set_pos({ 24,19 });
}


int main() {
	mt19937 gen(chrono::high_resolution_clock::now().time_since_epoch().count());

	arr = GameField(sz, sz);
	arr.make_border(8, '@');
	arr.generate(uniform_int_distribution<int>(10, 20)(gen), '@', ' ');
	while (1) {
		int i = uniform_int_distribution<int>(0, sz - 1)(gen), j = uniform_int_distribution<int>(0, sz - 1)(gen);
		if (arr.get(i, j) == ' ') {
			arr.set(i, j, 'I');
			me = { i,j };
			break;
		}
	}
	while (1) {
		int i = uniform_int_distribution<int>(0, sz - 1)(gen), j = uniform_int_distribution<int>(0, sz - 1)(gen);
		if (arr.get(i, j) == ' ') {
			arr.set(i, j, 'H');
			him = { i,j };
			break;
		}
	}
	for (int i = 0; i < sz; i++) {
		for (int j = 0; j < sz; j++) {
			if (arr.get(i, j) == ' ' && uniform_int_distribution<int>(0, 8)(gen) <= 2) {
				arr.set(i, j, '*');
				targets++;
			}
		}
	}
	drawmap();
	print(1);
	char c;
	while (1) {
		c = _getch();
		pair <int, int> from = me, to = me;
		if (c == 77) {
			//right
			if (ok(me.first, me.second + 1)) {
				to = { me.first, me.second + 1 };
			}
		}
		else if (c == 80) {
			//down
			if (ok(me.first + 1, me.second)) {
				to = { me.first + 1, me.second };
			}

		}
		else if (c == 72) {
			//up
			if (ok(me.first - 1, me.second)) {
				to = { me.first - 1, me.second };
			}
		}
		else if (c == 75) {
			//left
			if (ok(me.first, me.second - 1)) {
				to = { me.first, me.second - 1 };
			}
		}
		else {
			continue;
		}
		if (from != to) {
			if (arr.get(to.first, to.second) == '*') {
				scoreme++;
				arr.set(to.first, to.second, ' ');
			}
			char meold = arr.get(from.first, from.second);
			char menew = arr.get(to.first, to.second);
			arr.set(from.first, from.second, menew);
			arr.set(to.first, to.second, meold);
			drawcell(from.first / 4, from.second / 4);
			drawcell(to.first / 4, to.second / 4);
			me = to;

			print(0);
		}
		if (scoreme + scorehim == targets) {
			p.clear({ 0,0 }, { 41,25 }, BLACK);
			p.set_pos({ 0,0 });
			if (scoreme == scorehim) {
				p.print("DRAW\n", WHITE, BLACK);
			}
			if (scoreme > scorehim) {
				p.print("WIN!\n", WHITE, BLACK);
			}
			else {
				p.print("LOSE(\n", WHITE, BLACK);
			}
			exit(0);
		}
	}
	return 0;
}