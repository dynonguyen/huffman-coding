#include "decompress.h"

// ================== GIAI NEN FILE ================== //
void extractHereFile() {
	//file dau vao
	cout << ">> Enter the compression file path (EX: C:\\filename.winzip): ";
	string in;
	getline(cin, in);
	formatPath(in);
	char* in_path = stringToCharArray(in);
	//nhap phan mo rong file
	string ex;
	do{
		gotoxy(2, 6);
		cout << ">> Enter the file name extension (EX: .exe or .mp3 or .txt): ";
		cin >> ex;
		if (ex.find(".") != string::npos)
			break;
	} while (1);
	
	//file dau ra
	string out = changeFileExtension(in, FILE_NAME_EXTENSION_DECODE + ex);
	char* out_path = stringToCharArray(out);

	//Giai file
	Huffman huff;
	huff.setTime(clock());
	gotoxy(2, 8);
	SetColor(10);
	cout << "Decompressing ... Please wait a few seconds :\">";
	int check = huff.decoding(in_path, out_path);
	//ket qua
	gotoxy(2, 10);
	SetColor(TEXT_COLOR_INFO);
	printf("-----> Program execution time:: %.3f (s)", (double)1.0 * (clock() - huff.getTime()) / CLOCKS_PER_SEC);
	errorsMessage(check, 2, 10);
}

void extractFile() {
	//file dau vao				
	cout << ">> Enter the compression file path (EX: C:\\filename.winzip): ";
	string in;
	getline(cin, in);
	formatPath(in);
	char* in_path = stringToCharArray(in);
	//file dau ra
	gotoxy(2, 6);
	cout << ">> Enter the path to the extracted folder (EX: C:\\FoderName): ";
	string out;
	getline(cin, out);
	formatPath(out);
	string ex;

	do {
		gotoxy(2, 8);
		cout << ">> Enter the file name extension (EX: .exe, .h, .txt, .mp4): ";
		cin >> ex;
		if (ex.find(".") != string::npos)
			break;
	} while (1);

	setOutputPath(in, out, ex);
	char* out_path = stringToCharArray(out);
	//Giai file
	Huffman huff;
	huff.setTime(clock());
	gotoxy(2, 10);
	SetColor(10);
	cout << "Decompressing ... Please wait a few seconds :\">";
	int check = huff.decoding(in_path, out_path);
	//ket qua
	gotoxy(2, 12);
	SetColor(TEXT_COLOR_INFO);
	printf("-----> Program execution time:: %.3f (s)", (double)1.0 * (clock() - huff.getTime()) / CLOCKS_PER_SEC);
	errorsMessage(check, 2, 12);
}

// ================= GIAI NEN FOLDER ================ //
void decompressFolder() {
	//duong dan den file can giai nen
	cout << ">> Enter the compression file path [C:\\filename.winzip]: ";
	string in;
	getline(cin, in);
	formatPath(in);
	if (isFolder(in))
		throw "Not Compression File";
	string pathToFolder = getPathToFolder(in);
	//mo file ghi lai cac duong dan
	FILE* inFile;
	errno_t err = fopen_s(&inFile, stringToCharArray(in), "rb");
	if (inFile == NULL)
		throw "Fail to read file";
	int n;
	fscanf_s(inFile, "%d", &n, 1);

	//lay cac duong dan
	char* path;
	vector<string> filePath;
	for (int i = 0; i < n; i++) {
		path = new char[MAX_CHAR];
		fgets(path, MAX_CHAR, inFile);
		filePath.push_back(charArrayToString(path));
		delete[] path;
	}
	
	//tao cac folder
	for (int i = 0; i < n; ++i) {
		string temp;
		if (isFile(filePath[i])) {
			temp = getPathToFolder(filePath[i]);
		}
		else {
			temp = filePath[i];
			temp[temp.length() - 1] = '\\';
		}	
		for (int j = 0; j < temp.size(); ++j) {
			if (temp[j] == '\\' || temp[j] == '/') {
				//cout << pathToFolder + temp.substr(0, j) << endl;
				newFolder(pathToFolder + temp.substr(0, j));
			}
			
		}
	}

	Huffman huff;
	gotoxy(2, 6);
	SetColor(10);
	cout << "Decompressing ... Please wait a few seconds :\">";
	huff.setTime(clock());
	//tien hanh giai nen
	for (int i = 0; i < n; ++i){
		if (!isFolder(filePath[i])) {
			char* t_path = stringToCharArray(pathToFolder + filePath[i]);
			int check = huff.decodingFolder(inFile, t_path);
			if (check == 0)
				throw "decompression error";
		}
	}
	gotoxy(2, 8);
	SetColor(TEXT_COLOR_INFO);
	printf("-----> Program execution time:: %.3f (s)", (double)1.0 * (clock() - huff.getTime()) / CLOCKS_PER_SEC);
	fclose(inFile);
	errorsMessage(1, 2, 8);
}
