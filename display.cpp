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

	SetColor(TEXT_COLOR);
	gotoxy(25, 4 + 2 * move);
	cout << ">> ";

}

void menu() {

	SetColor(3);
	gotoxy(4, 2);
	cout << "=================== WINZIP PRO v1.0 (LIMITED EDITION) ===================";

	SetColor(2);
	gotoxy(30, 4);
	cout << "1 -> COMPRESS FILE";

	SetColor(2);
	gotoxy(30, 6);
	cout << "2 -> EXTRACT HERE";

	SetColor(2);
	gotoxy(30, 8);
	cout << "3 -> EXTRACT FILE...";

	SetColor(2);
	gotoxy(30, 10);
	cout << "4 -> COMPRESS FOLDER";

	SetColor(2);
	gotoxy(30, 12);
	cout << "5 -> DECOMPRESS FOLDER...";

	SetColor(4);
	gotoxy(22, 14);
	cout << "<===== Press ESC key to exit =====>";
}

void errorsMessage(int check, int x, int y) {
	if (!check) {
		gotoxy(x, y);
		SetColor(12);
		cout << "!============== ERRORS ==============!";
		gotoxy(x, y + 2);
		SetColor(6);
		cout << "<===== Press any key key to exit =====>";

	}
	else {
		gotoxy(x, y);
		SetColor(10);
		cout << "!============== NO ERRORS ==============!";
		SetColor(6);
		gotoxy(x, y+2);
		cout << "<===== Press any key key to exit =====>";
	}
	cout << endl << endl;
	_getch();
	exit(0);
}

char* stringToCharArray(string str) {
	char* res = NULL;
	res = new char[str.length() + 1];
	for (int i = 0; i < str.length(); i++){
		res[i] = str[i];
	}
	res[str.length()] = NULL;
	return res;
}

string changeFileExtension(string str, string fileName_Ex) {
	string res = "";
	for (int i = 0; i < str.length(); ++i) {
		if (str[i] == '.')
			break;
		res += str[i];
	}
	res += fileName_Ex;
	return res;
}

void findOutputPath(string in_path, string& out) {
	string out_path = "";
	out += "\\";
	for (int i = in_path.length() - FILE_NAME_EXTENSION_ENCODE.size() - 1; i >= 0; --i) {
		if (in_path[i] == '\\')
			break;
		out_path += in_path[i];
	}
	for (int i = out_path.length() - 1; i >= 0; --i) {
		out += out_path[i];
	}
	out += FILE_NAME_EXTENSION_DECODE;
}

void optimizeSetup_1() {
	system("cls");
	SetColor(6);
	gotoxy(4, 2);
}

void optimizeSetup_2() {
	gotoxy(2, 4);
	SetColor(7);
	ShowConsoleCursor(true);
}

void optimizeComp_Decomp_Folder(bool type) {

	optimizeSetup_1();
	if(type)
		cout << "=================== COMPRESS FOLDER ===================";
	else
		cout << "=================== DECOMPRESS FOLDER ===================";
	optimizeSetup_2();

	//file dau vao
	string in = "forfiles /p ";
	cout << ">> Enter the directory path [C:\\foldername]: ";
	string path;
	getline(cin, path);
	in = in + path + " > data.txt";
	char* inFile = stringToCharArray(in);

	//ghi tat ca ten file trong thu muc co duong dan path vao file data.txt
	//system(forfiles /p .path >> data.txt);
	system(inFile);

	//doc file data.txt
	vector<string> fileName;
	ifstream input("data.txt");
	if (input.fail()) {
		errorsMessage(0, 2, 6);
	}
	string s;
	getline(input, s);					//Bo qua dong trong dau tien trong file
	while (1) {
		getline(input, s);
		if (input.eof())
			break;
		fileName.push_back(s);
	}
	input.close();

	//vector chua cac duong dan den cac file trong thu muc nen
	vector<string> filePath;
	filePath.resize(fileName.size());
	for (int i = 0; i < fileName.size(); ++i) {
		filePath[i] = filePath[i] + path + "\\";
		for (int j = 1; j < fileName[i].length() - 1; ++j) {
			filePath[i].push_back(fileName[i][j]);
		}
	}

	//Tao ra thu muc chua file nen
	string comp_path;
	if(type)
		comp_path = path + "_Encode";
	else
		comp_path = path + "_Decode";
	
	wchar_t* folderPath = new wchar_t[comp_path.length() + 1];
	for (int i = 0; i < comp_path.length(); i++) {
		folderPath[i] = comp_path[i];
	}
	folderPath[comp_path.length()] = NULL;
	int status = _wmkdir(folderPath);

	//nen cac file vao thu muc vua tao
	//chuyen cac duong dan den file can nen tu string sang char*
	vector<char*> compFilePath;
	compFilePath.resize(filePath.size());
	for (int i = 0; i < filePath.size(); ++i) {
		compFilePath[i] = stringToCharArray(filePath[i]);
	}

	//chuyen duong dan den thu muc nen string sang char*
	//ten day du cua cac duong dan file nen = path + File name
	vector<string> fullFolderPath;
	fullFolderPath.resize(fileName.size());
	for (int i = 0; i < fileName.size(); i++) {
		fullFolderPath[i] = comp_path + "\\";
		for (int j = 1; j < fileName[i].size() - 1; ++j) {
			if (fileName[i][j] == '.')
				break;
			fullFolderPath[i] += fileName[i][j];
		}
		if(type)
			fullFolderPath[i] += FILE_NAME_EXTENSION_ENCODE;
		else
			fullFolderPath[i] += FILE_NAME_EXTENSION_DECODE;
	}

	//chuyen doi sang char*
	vector<char*> folderCompPath;
	folderCompPath.resize(fullFolderPath.size());
	for (int i = 0; i < fullFolderPath.size(); i++) {
		folderCompPath[i] = NULL;
		folderCompPath[i] = stringToCharArray(fullFolderPath[i]);
	}

	//Tien hanh nen
	int check = 1;
	for (int i = 0; i < compFilePath.size(); ++i) {
		Huffman huff;
		if(type)
			check = huff.encoding(compFilePath[i], folderCompPath[i]);
		else
			check = huff.decoding(compFilePath[i], folderCompPath[i]);
		if (!check) {
			errorsMessage(check, 2, 6);
		}
	}
	errorsMessage(1, 2, 6);
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

					//nhap file dau vao
					optimizeSetup_2();
					cout << ">> Enter the original file path (EX: C:\\filename.txt): ";
					string in;
					getline(cin, in);
					char* in_path = stringToCharArray(in);

					//file dau ra
					string out = changeFileExtension(in, FILE_NAME_EXTENSION_ENCODE);
					char* out_path = stringToCharArray(out);

					//nen file
					Huffman huff;
					int check = huff.encoding(in_path, out_path);
					errorsMessage(check, 2, 6);
					break;
				}
				case 1: {
					optimizeSetup_1();
					cout << "=================== EXTRACT HERE ===================";
					optimizeSetup_2();

					//file dau vao
					cout << ">> Enter the compression file path (EX: C:\\filename.winzip): ";
					string in;
					getline(cin, in);
					char* in_path = stringToCharArray(in);

					//file dau ra
					string out = changeFileExtension(in, FILE_NAME_EXTENSION_DECODE);
					char* out_path = stringToCharArray(out);

					//Giai file
					Huffman huff;
					int check = huff.decoding(in_path, out_path);
					errorsMessage(check, 2, 6);
				}
				case 2: {
					optimizeSetup_1();
					cout << "=================== EXTRACT FILE ... ===================";
					optimizeSetup_2();
					//file dau vao				
					cout << ">> Enter the compression file path (EX: C:\\filename.winzip): ";
					string in;
					getline(cin, in);
					char* in_path = stringToCharArray(in);

					//file dau ra
					gotoxy(2, 6);
					cout << ">> Enter the path to the extracted folder (EX: C:\\FoderName): ";
					string out;
					getline(cin, out);
					findOutputPath(in, out);
					char* out_path = stringToCharArray(out);

					//Giai nen file
					Huffman huff;
					int check = huff.decoding(in_path, out_path);
					errorsMessage(check, 2, 8);
				}
				case 3: {
					optimizeComp_Decomp_Folder(true);
				}
				case 4: {
					optimizeComp_Decomp_Folder(false);
				}
				}
			}
			}
			displayCursor(pre_move, move);
		}
	}
}
