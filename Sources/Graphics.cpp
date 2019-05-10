#include "Graphics.h"

ConsoleGUI::ConsoleGUI()
{

}

HANDLE ConsoleGUI::get_console_handle()
{
	return GetStdHandle(STD_OUTPUT_HANDLE);
}

void ConsoleGUI::set_pos(Point p)
{
	COORD c;
	c.X = p.x;
	c.Y = p.y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void ConsoleGUI::set_text_color(Color cl)
{
	WORD attr = static_cast<WORD>(current_bg_color);
	attr <<= 4;
	attr |= static_cast<WORD>(cl);

	if (SetConsoleTextAttribute(get_console_handle(), attr))
		current_text_color = cl;
}

void ConsoleGUI::set_bg_color(Color cl)
{
	WORD attr = static_cast<WORD>(cl);
	attr <<= 4;
	attr |= static_cast<WORD>(current_text_color);

	if (SetConsoleTextAttribute(get_console_handle(), attr))
		current_bg_color = cl;
}

void ConsoleGUI::clear(Point p, Color bg_color)
{
	set_pos(p);
	set_bg_color(bg_color);
	cout << " ";
}

//Çŕëčâŕĺň ďđ˙ěîóăîëüíčę [start.x; end.x] [start.y; end.y] öâĺňîě bg_color
void ConsoleGUI::clear(Point start, Point end, Color bg_color)
{
	set_bg_color(bg_color);

	for (unsigned y = start.y; y <= end.y; y++)
	{
		set_pos(Point(start.x, y));

		for (unsigned x = start.x; x <= end.x; x++)
			cout << " ";
	}
}

void ConsoleGUI::clear_all()
{
	system("cls");
}

int ConsoleGUI::print(char c)
{
	cout << c;
	return 1;
}

int ConsoleGUI::print(int n)
{
	cout << n;
	return to_string(n).size();
}

int ConsoleGUI::print(string text)
{
	cout << text;
	return text.size();
}

int ConsoleGUI::print(char c, Color font, Color back)
{
	set_text_color(font);
	set_bg_color(back);
	print(c);
	return 1;
}
	
int ConsoleGUI::print(int n, Color font, Color back)
{
	set_text_color(font);
	set_bg_color(back);
	print(n);
	return to_string(n).size();
}

int ConsoleGUI::print(string text, Color font, Color back)
{
	set_text_color(font);
	set_bg_color(back);
	print(text);
	return text.size();
}
