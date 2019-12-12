#include "display.h"

void gotoxy(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}

void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

void SetColor(int ForgC)
{
	WORD wColor;

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	//We use csbi for the wAttributes word.
	if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
	{
		//Mask out all but the background attribute, and add in the forgournd color
		wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
	return;
}

void displayCursor(int pre_move, int move) {
	gotoxy(25, 4 + 2 * pre_move);
	cout << "   ";

	SetColor(TEXT_COLOR_CURSOR);
	gotoxy(25, 4 + 2 * move);
	cout << ">> ";
}

void menu() {

	system("cls");
	SetColor(TEXT_COLOR_PROGRAM_NAME);
	gotoxy(5, 2);
	cout << "=================== WINZIP PRO v.2.0 (LIMITED EDITION) ===================";

	SetColor(TEXT_COLOR_TITLE);
	gotoxy(30, 4);
	cout << "1 -> COMPRESS FILE";

	SetColor(TEXT_COLOR_TITLE);
	gotoxy(30, 6);
	cout << "2 -> COMPRESS FOLDER";

	SetColor(TEXT_COLOR_TITLE);
	gotoxy(30, 8);
	cout << "3 -> EXTRACT HERE";

	SetColor(TEXT_COLOR_TITLE);
	gotoxy(30, 10);
	cout << "4 -> EXTRACT TO...";

	SetColor(TEXT_COLOR_EXIT);
	gotoxy(23, 12);
	cout << "<===== Press ESC key to exit =====>";
}

void errorsMessage(int check, int x, int y) {
	if (!check) {
		gotoxy(x, y);
		SetColor(TEXT_COLOR_ERROR);
		cout << "!============== ERRORS ==============!";
		gotoxy(x, y + 2);
		SetColor(TEXT_COLOR_EXIT);
		cout << "<===== Press any key to exit =====>";

	}
	else {
		gotoxy(x, y + 2);
		SetColor(TEXT_COLOR_NO_ERROR);
		cout << "!============== NO ERROR ==============!";
		gotoxy(x, y + 4);
		SetColor(TEXT_COLOR_EXIT);
		cout << "<===== Press any key to exit =====>";
	}

	cout << endl << endl;
	_getch();
	exit(0);
}

void optimizeSetup_1() {
	system("cls");
	SetColor(6);
	gotoxy(2, 2);
}

void optimizeSetup_2() {
	gotoxy(2, 4);
	SetColor(TEXT_COLOR);
	ShowConsoleCursor(true);
}

void displayMain() {
	int move = 0;
	int pre_move = move;
	int option = 0;

	ShowConsoleCursor(false);
	menu();
	displayCursor(pre_move, move);

	while (1) {
		if (_kbhit()) {
			char key = _getch();
			switch (key) {
			case 'w': case 'W': case 72: {
				if (move > 0) {
					pre_move = move;
					--move;
				}
				break;
			}
			case 's': case 'S': case 80: {
				if (move < MAX_OPTION) {
					pre_move = move;
					++move;
				}
				break;
			}
			case 27: {
				system("cls");
				gotoxy(30, 5);
				cout << "<========== GOOD BYE :\"> ==========>";
				exit(0);
				break;
			}
			case 13: {
				option = move;
				switch (option) {
				case 0: {
					optimizeSetup_1();
					cout << "=================== COMPRESSION ===================";
					optimizeSetup_2();
					compressFile();
					exit(0);
				}
				case 1: {
					optimizeSetup_1();
					cout << "=================== COMPRESS FOLDER ===================";
					optimizeSetup_2();
					compressFolder();
					exit(0);
				}
				case 2: {
					optimizeSetup_1();
					cout << "=================== EXTRACT HERE ===================";
					optimizeSetup_2();
					extractHere();
					exit(0);
				}
				case 3: {
					optimizeSetup_1();
					cout << "=================== EXTRACT TO ... ===================";
					optimizeSetup_2();
					extractTo();
					exit(0);
				}
				}
			}
			}
			displayCursor(pre_move, move);
		}
	}
}
