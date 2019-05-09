#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <conio.h>
#include <random>
#include <time.h>
#include "Graphics.h"
#include "Bot.h"
#include "GameField.h"

using namespace std;

typedef unsigned u;

#define inf 1'000'000'007

const int sz = 64;
char arr[sz][sz];
char minimap[sz / 4][sz / 4];
pair <int, int> me, him;
int scoreme = 0, scorehim = 0;



int dx[] = { -1,0,1,0 };
int dy[] = { 0,-1,0,1 };

inline bool okgen(int x, int y) {
	return x > 8 && x < sz - 8 && y > 8 && y < sz - 8 && arr[x][y] == '@';
}

inline bool okturn(int x, int y) {
	return x > 8 && x < sz - 8 && y > 8 && y < sz - 8 && (arr[x][y] == ' ' || arr[x][y] == '*');
}

void generate0() {
	for (int i = 0; i < sz; i++) {
		for (int j = 0; j < sz; j++) {
			arr[i][j] = '@';
		}
	}
	int num = min((sz - 16) * (sz - 16), rand() % 1000 + 1500);
	queue <pair <int, int>> q;
	q.push({ rand() % (sz - 16) + 8, rand() % (sz - 16) + 8 });
	while (q.size() && num--) {
		int x = q.front().first, y = q.front().second;
		q.pop();
		arr[x][y] = ' ';
		for (int i = 0; i < 4; i++) {
			int X = x + dx[i], Y = y + dy[i];
			if (okgen(X, Y) && rand() % 2) {
				q.push({ X,Y });
			}
		}
		while (q.empty()) {
			int X = rand() % (sz - 16) + 8, Y = rand() % (sz - 16) + 8;
			if (arr[X][Y] == ' ') {
				for (int i = 0; i < 4; i++) {
					if (okgen(X + dx[i], Y + dy[i])) {
						q.push({ X + dx[i], Y + dy[i] });
					}
				}
			}
		}
	}

	char add[] = { 'I', 'H', '*', '*', '*', '*', '*' };
	for (int k = 0; k < 7; k++) {
		while (1) {
			int x = rand() % (sz - 16) + 8, y = rand() % (sz - 16) + 8;
			if (arr[x][y] == ' ') {
				arr[x][y] = add[k];
				if (k == 0) {
					me = { x,y };
				}
				else if (k == 1) {
					him = { x,y };
				}
				break;
			}
		}
	}
}


void drawcell(int i, int j) {
	int wall = 0, space = 0;
	bool hero = 0, enemy = 0, star = 0;
	for (int k = 0; k < 4; k++) {
		for (int l = 0; l < 4; l++) {
			if (arr[i * 4 + k][j * 4 + l] == 'I') {
				hero = 1;
			}
			else if (arr[i * 4 + k][j * 4 + l] == 'H') {
				enemy = 1;
			}
			else if (arr[i * 4 + k][j * 4 + l] == '*') {
				star = 1;
			}
			else if (arr[i * 4 + k][j * 4 + l] == '@') {
				wall++;
			}
			else if (arr[i * 4 + k][j * 4 + l] == ' ') {
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


ConsoleGUI p;
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
			str += arr[cell.first * 4 + i][cell.second * 4 + j];
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

void histurn() {
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
		to = { him.first + dx[i], him.second + dy[i] };
		if (okturn(to.first, to.second)) {
			break;
		}
	}
	if (to.first == -1) {
		return;
	}
	for (int i = 0; i < 4; i++) {
		int x = him.first + dx[i], y = him.second + dy[i];
		if (okturn(x, y) && dp[to.first][to.second] > dp[x][y] && rand() % 125) {
			to = { x,y };
		}
	}
	if (arr[to.first][to.second] == '*') {
		scorehim++;
		arr[to.first][to.second] = ' ';
	}
	swap(arr[him.first][him.second], arr[to.first][to.second]);
	drawcell(him.first / 4, him.second / 4);
	drawcell(to.first / 4, to.second / 4);
	him = to;
}


int main() {
	srand(time(0));
	generate0();
	drawmap();
	print(1);
	char c;
	while (1) {
		c = _getch();
		pair <int, int> from = me, to = me;
		if (c == 77) {
			//right
			if (okturn(me.first, me.second + 1)) {
				to.second++;
			}
		}
		else if (c == 80) {
			//down
			if (okturn(me.first + 1, me.second)) {
				to.first++;
			}
		}
		else if (c == 72) {
			//up
			if (okturn(me.first - 1, me.second)) {
				to.first--;
			}
		}
		else if (c == 75) {
			//left
			if (okturn(me.first, me.second - 1)) {
				to.second--;
			}
		}
		else {
			continue;
		}
		if (from != to) {
			if (arr[to.first][to.second] == '*') {
				scoreme++;
				arr[to.first][to.second] = ' ';
			}
			swap(arr[from.first][from.second], arr[to.first][to.second]);
			drawcell(from.first / 4, from.second / 4);
			drawcell(to.first / 4, to.second / 4);
			me = to;
			histurn();
			print(0);
		}
		if (scoreme + scorehim == 5) {
			p.clear({ 0,0 }, { 41,25 }, BLACK);
			p.set_pos({ 0,0 });
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