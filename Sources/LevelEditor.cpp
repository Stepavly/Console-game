#include <iostream>
#include <conio.h>
#include <fstream>
#include "GameField.h"
using namespace std;

enum ActionType
{
	LARROW = 75,
	RARROW = 77,
	UARROW = 72,
	DARROW = 80,
	CLEAR = 83,       //Delete
	FINISH = 27,      //Esc
	CHANGE_BACK = 59, //F1
	CHANGE_FONT = 60  //F2
};

ConsoleGUI drawer;
Point bg_color_pos, font_color_pos;
int offset = 2;

void print_ui(int w, int h)
{
	Point cursor(0, h);

	for (int i = 0; i < w; i++)
	{
		drawer.set_pos(cursor);
		cursor.x += drawer.print("-", WHITE, BLACK);
	}

	cursor = Point(w, 0);

	for (int i = 0; i < h; i++)
	{
		drawer.set_pos(cursor);
		cursor.y += drawer.print("|", WHITE, BLACK);
	}
	
	cursor = Point(0, h + offset + 1);

	drawer.set_pos(cursor);
	drawer.set_text_color(WHITE);

	cursor.x += drawer.print("Цвета: ");

	for (int i = 0; i <= 15; i++)
	{
		drawer.set_pos(cursor);
		cursor.x += drawer.print(' ', WHITE, (Color)i);

		if (i >= 10)
		{
			drawer.set_pos(cursor);
			cursor.x += drawer.print(' ', WHITE, (Color)i);
		}

		cursor.x += drawer.print(' ', WHITE, BLACK);
	}

	cursor = Point(0, h + offset + 2);
	drawer.set_pos(cursor);
	drawer.set_bg_color(BLACK);
	cursor.x += drawer.print("Коды: ");
	cursor.x += 1;

	for (int i = 0; i <= 15; i++)
	{
		drawer.set_pos(cursor);
		cursor.x += drawer.print(i);
		cursor.x += drawer.print(' ', WHITE, BLACK);
	}

	cursor = Point(0, h + offset);
	drawer.set_pos(cursor);

	cursor.x += drawer.print("Текущие цвета(фон, текст): ", WHITE, BLACK);
	drawer.set_pos(cursor);
	bg_color_pos = cursor;
	cursor.x += drawer.print("  ");
	drawer.set_pos(cursor);
	font_color_pos = cursor;
	cursor.x += drawer.print(' ', WHITE, WHITE);
}

string decode(const GameField& field, int w, int h)
{
	string res;
	res += to_string(w) + " " + to_string(h) + "\n";

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			res += "[";
			res += field.get(i, j).text;
			res += ",";

			if (field.get(i, j).back_color < 10)
				res += "0";

			res += to_string(field.get(i, j).back_color);
			res += ",";

			if (field.get(i, j).font_color < 10)
				res += "0";

			res += to_string(field.get(i, j).font_color);
			res += "]";
		}

		res += "\n";
	}

	return res;
}

GameField encode(const string& path)
{
	ifstream in(path);
	int w, h;
	in >> w >> h;

	GameField res(w, h);

	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
		{
			string t;
			char c;

			for (int k = 0; k < 9; k++)
			{
				in.get(c);

				if (c == '\n')
					in.get(c);

				t += c;
			}

			char symbol = t[1];
			int back_color = (t[3] - '0') * 10 + (t[4] - '0');
			int font_color = (t[6] - '0') * 10 + (t[7] - '0');

			res.set(i, j, Cell(symbol, (Color)back_color, (Color)font_color));
		}

	in.close();

	return res;
}

void work(GameField& field, int w, int h)
{
	Point cursor(0, 0);
	drawer.set_pos(cursor);
	Color bg_color = BLACK;
	Color font_color = WHITE;

	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
			if (field.get(i, j).text == 0)
				field.set(i, j, Cell(' ', bg_color, font_color));
	
	while (1)
	{
		int code = _getch();

		if (code == FINISH)
		{
			cursor = Point(0, h + 4);
			drawer.set_bg_color(BLACK);
			drawer.set_text_color(WHITE);
			drawer.set_pos(cursor);

			cursor.x += drawer.print("Введите путь с названием файла для сохранения: ");

			string path;
			cin >> path;

			ofstream out(path);
			out << decode(field, w, h);
			return;
		}
		else if (code == LARROW)
			cursor.x--;
		else if (code == RARROW)
			cursor.x++;
		else if (code == UARROW)
			cursor.y--;
		else if (code == DARROW)
			cursor.y++;
		else if (code == CHANGE_BACK)
		{
			Point prev_pos = cursor;
			cursor = Point(0, h + offset + 3);
			drawer.set_bg_color(BLACK);
			drawer.set_text_color(WHITE);
			drawer.set_pos(cursor);

			cursor.x += drawer.print("Введите код нового цвета: ") + 2;
			int num;
			cin >> num;

			drawer.clear({ 0, (unsigned)h + offset + 3 }, cursor, BLACK);
			bg_color = (Color)num;
			drawer.set_pos(bg_color_pos);
			drawer.print(' ', bg_color, bg_color);
			bg_color = (Color)num;
			drawer.set_bg_color(bg_color);
			drawer.set_text_color(font_color);
			cursor = prev_pos;
		}
		else if (code == CHANGE_FONT)
		{
			Point prev_pos = cursor;
			cursor = Point(0, h + offset + 3);
			drawer.set_bg_color(BLACK);
			drawer.set_text_color(WHITE);
			drawer.set_pos(cursor);

			cursor.x += drawer.print("Введите код нового цвета: ") + 2;
			int num;
			cin >> num;

			drawer.clear({ 0, (unsigned)h + offset + 3 }, cursor, BLACK);
			drawer.set_pos(font_color_pos);
			font_color = (Color)num;
			drawer.print(' ', font_color, font_color);
			drawer.set_bg_color(bg_color);
			drawer.set_text_color(font_color);
			cursor = prev_pos;
		}
		else if (code == CLEAR)
		{
			drawer.clear(cursor, bg_color);
			field.set(cursor.y, cursor.x, Cell(' ', bg_color, font_color));
		}
		else if (char(code) > 0)
		{
			drawer.print((char)code, font_color, bg_color);
			field.field[cursor.y][cursor.x] = Cell((char)code, bg_color, font_color);
		}

		drawer.set_pos(cursor);
	}
}

void print_field(const GameField& field, int w, int h)
{
	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
		{
			drawer.set_pos({ (unsigned)j, (unsigned)i });

			Cell t = field.get(i, j);

			drawer.print(t.text, t.font_color, t.back_color);
		}
}

int main()
{
	setlocale(LC_ALL, "rus");
	
	cout << "Новая карта - 1\n";
	cout << "Редактировать карту - 2\n";
	cout << "Посмотреть карту - 3\n";

	int type;
	cin >> type;

	if (type == 1)
	{
		system("cls");

		cout << "Ширина карты: ";
		int w;
		cin >> w;

		cout << "Высота карты: ";
		int h;
		cin >> h;

		system("cls");

		print_ui(w, h);

		GameField field(h, w);
		
		work(field, w, h);
	}
	else if (type == 2 || type == 3)
	{
		system("cls");
		cout << "Введите путь до карты: ";
		
		string path;
		cin >> path;

		system("cls");
		GameField field = encode(path);
		print_field(field, field.w, field.h);

		if (type == 2)
		{
			print_ui(field.w, field.h);
			work(field, field.w, field.h);
		}
		else
			cin >> path;
	}
	else
		exit(1);

	return 0;
}