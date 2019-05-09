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

#define inf 1'000'000'007

const int sz = 64;
GameField arr;
char minimap[sz / 4][sz / 4];
int target = 0, simosk = 0, stproger = 0;
pair <int, int> imosk, tproger;
ConsoleGUI p;
Bot b;
int dx[] = { -1,0,1,0 };
int dy[] = { 0,-1,0,1 };

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
	string str = "SCORE  Imosk " + to_string(simosk) + " : " + to_string(stproger) + " Tproger";
	now.push_back(str);
	now.push_back("##====================##====================##");
	pair <int, int> cell1 = { imosk.first / 4 - 2, imosk.second / 4 - 2 };
	pair <int, int> cell2 = { tproger.first / 4 - 2, tproger.second / 4 - 2 };
	for (int i = 0; i < 20; i++) {
		str = "||";
		for (int j = 0; j < 20; j++) {
			str += arr.get(cell1.first * 4 + i, cell1.second * 4 + j);
		}
		str += "||";
		for (int j = 0; j < 20; j++) {
			str += arr.get(cell2.first * 4 + i, cell2.second * 4 + j);
		}
		str += "||";
		now.push_back(str);
	}
	now.push_back("##====================##====================##");
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

void turntrpoger() {
	b.move();
	arr.set(tproger.first, tproger.second, ' ');
	drawcell(tproger.first / 4, tproger.second / 4);
	tproger = { b.pos.vi, b.pos.vj };
	if (arr.get(tproger.first, tproger.second) == '*') {
		stproger++;
		arr.set(tproger.first, tproger.second, ' ');
	}
	arr.set(tproger.first, tproger.second, 'H');
	drawcell(tproger.first / 4, tproger.second / 4);
}

inline bool okturn(int x, int y) {
	return x > 0 && x < sz && y > 0 && y < sz && (arr[x][y] == ' ' || arr[x][y] == '*');
}

void turnimosk() {
	int dp[sz][sz];
	queue <pair <int, int>> q;

	for (int i = 0; i < sz; i++) {
		for (int j = 0; j < sz; j++) {
			if (arr[i][j] == '*') {
				q.push({ i,j });
				dp[i][j] = 0;
			}
			else {
				dp[i][j] = inf;
			}
		}
	}
	while (q.size()) {
		int x = q.front().first, y = q.front().second;
		q.pop();
		for (int i = 0; i < 4; i++) {
			int X = x + dx[i], Y = y + dy[i];
			if (arr[X][Y] != '@' && dp[X][Y] > dp[x][y] + 1) {
				q.push({ X,Y });
				dp[X][Y] = dp[x][y] + 1;
			}
		}
	}

	pair <int, int> to = { -1,-1 };
	for (int i = 0; i < 4; i++) {
		to = { imosk.first + dx[i], imosk.second + dy[i] };
		if (okturn(to.first, to.second)) {
			break;
		}
	}
	if (to.first == -1) {
		return;
	}
	for (int i = 0; i < 4; i++) {
		int x = imosk.first + dx[i], y = imosk.second + dy[i];
		if (okturn(x, y) && dp[to.first][to.second] > dp[x][y] && rand() % 125) {
			to = { x,y };
		}
	}
	if (arr[to.first][to.second] == '*') {
		simosk++;
		arr[to.first][to.second] = ' ';
	}
	arr[to.first][to.second] = arr[imosk.first][imosk.second];
	arr[imosk.first][imosk.second] = ' ';
	drawcell(imosk.first / 4, imosk.second / 4);
	drawcell(to.first / 4, to.second / 4);
	imosk = to;
}

int main() {
	mt19937 gen(chrono::high_resolution_clock::now().time_since_epoch().count());
	srand(time(0));

	arr = GameField(sz, sz);
	arr.make_border(8, '@');
	arr.generate(uniform_int_distribution<int>(10, 20)(gen), '@', ' ');
	while (1) {
		int i = uniform_int_distribution<int>(0, sz - 1)(gen), j = uniform_int_distribution<int>(0, sz - 1)(gen);
		if (arr.get(i, j) == ' ') {
			arr.set(i, j, 'I');
			imosk = { i,j };
			break;
		}
	}
	while (1) {
		int i = uniform_int_distribution<int>(0, sz - 1)(gen), j = uniform_int_distribution<int>(0, sz - 1)(gen);
		if (arr.get(i, j) == ' ') {
			arr.set(i, j, 'H');
			tproger = { i,j };
			break;
		}
	}
	for (int i = 0; i < sz; i++) {
		for (int j = 0; j < sz; j++) {
			if (arr.get(i, j) == ' ' && uniform_int_distribution<int>(0, 8)(gen) <= 2) {
				arr.set(i, j, '*');
				target++;
			}
		}
	}
	b = Bot(arr, { tproger.first, tproger.second });
	drawmap();
	print(1);
	
	if (rand() % 2) {
		while (simosk + stproger != target) {
			turnimosk();
			print(0);
			Sleep(500);
			turntrpoger();
			print(0);
			Sleep(500);
		}
	}
	else {
		while (simosk + stproger != target) {
			turntrpoger();
			print(0);
			Sleep(500);
			turnimosk();
			print(0);
			Sleep(500);
		}
	}

	p.clear({ 0,0 }, { 41,25 }, BLACK);
	p.set_pos({ 0,0 });
	if (simosk == stproger) {
		p.print("DRAW\n", WHITE, BLACK);
	}
	if (simosk > stproger) {
		p.print("IMOSK WIN!\n", WHITE, BLACK);
	}
	else {
		p.print("TPROGER WIN!\n", WHITE, BLACK);
	}

	return 0;
}