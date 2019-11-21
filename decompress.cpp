#include "decompress.h"

// ================== GIAI NEN FILE TEXT ================== //
int extractHereFileText() {
	//file dau vao
	cout << ">> Enter the compression file path (EX: C:\\filename.winzip): ";
	string in;
	getline(cin, in);
	char* in_path = stringToCharArray(in);

	//file dau ra
	string out = changeFileExtension(in, FILE_NAME_EXTENSION_DECODE_TEXT);
	char* out_path = stringToCharArray(out);

	//Giai file
	Huffman huff;
	int check = huff.decoding(in_path, out_path);

	Free(in_path);
	Free(out_path);
	return check;
}

int extractFileText() {
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
	setOutputPath(in, out);
	char* out_path = stringToCharArray(out);
	//Giai nen file
	Huffman huff;
	int check = huff.decoding(in_path, out_path);

	Free(in_path);
	Free(out_path);
	return check;
}

// ================= GIAI NEN FOLDER TEXT ================ //
int decompressFolderTxt() {
	//duong dan den file can giai nen
	cout << ">> Enter the compression file path [C:\\filename.winzip]: ";
	string in;
	getline(cin, in);
	if (isFolder(in))
		return 0;

	char* in_path = stringToCharArray(in);
	//lay ten filename de tao duong dan moi
	string outFolder;
	for (int i = 0; i < in.length(); ++i) {
		if (in[i] == '.')
			break;
		outFolder += in[i];
	}
	outFolder += "_decode";

	//tao folder giai nen
	if (newFolder(outFolder) == 1)
		return 0;

	//mo file ghi lai cac duong dan
	FILE* inFile;
	errno_t err = fopen_s(&inFile, in_path, "rb");
	if (inFile == NULL)
		return 0;
	int n;
	fscanf_s(inFile, "%d", &n, 1);

	//lay cac duong dan
	vector<char*> path;
	vector<string> filePath;
	path.resize(n);
	filePath.resize(n);
	for (int i = 0; i < n; i++) {

		path[i] = new char[MAX_CHAR];
		fgets(path[i], MAX_CHAR, inFile);

		filePath[i] = outFolder;
		for (int j = 2; j < strlen(path[i]) - 1; ++j)
			filePath[i] += path[i][j];

		delete[] path[i];
	}

	//tao cac folder
	for (int i = 0; i < n; ++i) {
		if (isFolder(filePath[i])) {
			for (int j = 3; j < filePath[i].size(); ++j) {
				if (filePath[i][j] == '\\' || filePath[i][j] == '/')
					newFolder(filePath[i].substr(0, j));
				if (j == filePath[i].size() - 1)
					newFolder(filePath[i].substr(0, j + 1));
			}
		}
		else {
			string temp = getFolderPath(filePath[i]);
			for (int j = 3; j < temp.size(); ++j) {
				if (temp[j] == '\\' || temp[j] == '/')
					newFolder(temp.substr(0, j));
				if (j == temp.size() - 1)
					newFolder(temp.substr(0, j + 1));
			}
		}
	}

	//tien hanh giai nen
	for (int i = 0; i < n; ++i){
		if (!isFolder(filePath[i])) {
			char* t_path = stringToCharArray(filePath[i]);
			Huffman huff;
			int check = huff.decodingFolder(inFile, t_path);
			Free(t_path);
			if (check == 0)
				return 0;
		}
	}
	fclose(inFile);
	return 1;
}