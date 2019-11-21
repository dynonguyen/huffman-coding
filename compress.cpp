#include "compress.h"

//==================== CAC HAM HO TRO NEN-GIAI NEN FILE TXT ====================//
char* stringToCharArray(string str) {
	char* res = new char[str.size() + 1];
	for (int i = 0; i < str.size(); i++) {
		res[i] = str[i];
	}
	res[str.size()] = NULL;
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

void setOutputPath(string in_path, string& out) {
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
	out += FILE_NAME_EXTENSION_DECODE_TEXT;
}

// =============== NEN FILE TEXT =================== //
int compressFileText() {
	cout << ">> Enter the original file path (EX: C:\\filename.txt): ";
	string in;
	getline(cin, in);
	char* in_path = stringToCharArray(in);

	//file dau ra
	string out = changeFileExtension(in, FILE_NAME_EXTENSION_ENCODE);
	char* out_path = stringToCharArray(out);

	//nen file
	Huffman huff;
	int check = huff.encoding(in_path, out_path, 0);

	Free(in_path);
	Free(out_path);
	return check;
}

// ==================== CAC HAM HO TRO NEN-GIAI NEN FOLDER TXT =================== //
bool isFolder(string path) {
	if (path.find(".") == string::npos)
		return true;

	return false;
}

bool isFileTxt(string path) {
	if (path.find(".txt") != string::npos)
		return true;
	return false;
}

int newFolder(string path) {
	if (!isFolder(path))
		return 1;
	char* path_t = stringToCharArray(path);
	int result = _mkdir(path_t);
	Free(path_t);
	return result;
}

string getFolderPath(string path) {
	int pos_i = 0;
	for(int i = path.size() - 1; i >= 0; --i)
		if (path[i] == '/' || path[i] == '\\') {
			pos_i = i;
			break;
		}
	string result = "";
	for (int i = 0; i < pos_i; ++i){
		result += path[i];
	}
	return result;
}

vector<string> saveDirectoryStructure(string path) {
	static vector<string> result;
	vector<string> folder;

	string in = "forfiles /p ";
	in = in + "\"" + path + "\"" + " > data.txt";
	char* in_path = stringToCharArray(in);
	if (system(in_path))							//Ghi cac folder va file trong 1 folder re data.txt
		result.push_back(path);

	//xoa bo thong bao loi cua system
	optimizeSetup_1();
	cout << "=================== COMPRESS FOLDER ===================";

	//doc file data.txt
	ifstream input("data.txt");		
	if (input.fail()) {
		errorsMessage(0, 2, 6);
	}
	string s;
	getline(input, s);								//Bo qua dong trong dau tien trong file
	while (1) {
		getline(input, s);
		if (input.eof())
			break;
		string str = path + "\\";
		if (isFileTxt(s)) {
			for (int i = 1; i < s.length() - 1; ++i)
				str += s[i];
			result.push_back(str);
		}
		else {
			for (int i = 1; i < s.length() - 1; ++i)
				str += s[i];
			folder.push_back(str);
		}	
	}
	input.close();
	for (int i = 0; i < folder.size(); ++i)
		saveDirectoryStructure(folder[i]);

	Free(in_path);
	return result;
}

// =============== NEN 1 FOLDER CHUA FILE TEXT =============== //

int compressFolderTxt() {
	cout << ">> Enter the directory path [C:\\foldername]: ";
	string in;
	getline(cin, in);
	if (!isFolder(in))
		return 0;
	
	//luu lai cay thu muc
	vector<string> path = saveDirectoryStructure(in);

	//tao duong dan file nen
	string out = in + FILE_NAME_EXTENSION_ENCODE;
	char* out_path = stringToCharArray(out);

	ofstream output(out_path, ios::binary | ios::app);
	output << path.size();
	for (int i = 0; i < path.size(); i++){
		//ghi lai duong dan de giai nen
		output << path[i] << endl;
	}
	output.close();

	for (int i = 0; i < path.size(); ++i){
		if (isFileTxt(path[i])) {
			char* com_path = stringToCharArray(path[i]);
			//tien hanh nen
			Huffman huff;
			int check = huff.encoding(com_path, out_path, 1);
			Free(com_path);
			if (check == 0)
				return 0;
		}
	}
	Free(out_path);
	return 1;
}
