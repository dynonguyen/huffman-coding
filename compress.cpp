#include "compress.h"

// ======================= CAC HAM HO TRO NEN-GIAI NEN ======================= //
float getSize(const string& path) {
	float res = 0;
	ifstream in_size(path);
	if (!in_size.fail()) {
		in_size.seekg(0, ios::end);
		res = in_size.tellg();
		in_size.close();
	}
	return res;
}

char* stringToCharArray(const string& str) {
	char* res = new char[str.size() + 1];
	for (int i = 0; i < str.size(); i++) {
		res[i] = str[i];
	}
	res[str.size()] = NULL;
	return res;
}

string changeFileExtension(const string& str, const string& fileName_Ex) {
	string res = "";
	for (int i = 0; i < str.length(); ++i) {
		if (str[i] == '.')
			break;
		res += str[i];
	}
	res += fileName_Ex;
	return res;
}

void setOutputPath(const string& in_path, string& out, const string& extension) {
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
	out += FILE_NAME_EXTENSION_DECODE + extension;
}

string charArrayToString(char* s) {
	if (s == NULL)
		return "";
	string res = "";
	for (int i = 0; i < strlen(s); i++){
		res += s[i];
	}
	res[strlen(s) - 1] = '\0';
	return res;
}

bool isCompressionFile(const string& path) {
	if (path.find(FILE_NAME_EXTENSION_ENCODE) != string::npos)
		return true;
	if(path.find(".zip") != string::npos)
		return true;
	if (path.find(".rar") != string::npos)
		return true;
	return false;
}

void formatPath(string& path) {
	if (path[0] == '"' && path[path.length() - 1] == '"') {
		path.erase(path.begin());
		path.erase(path.begin() + path.length() - 1);
	}
}
// ============================== NEN FILE ================================== //
void compressFile() {
	cout << ">> Enter the original file path (EX: C:\\filename.txt): ";
	string in;
	getline(cin, in);
	if (!isFile(in) || isCompressionFile(in))
		throw "Not File (The file has been compressed)";
	formatPath(in);
	char* in_path = stringToCharArray(in);
	static int in_size = getSize(in_path);

	//file dau ra
	string out = changeFileExtension(in, FILE_NAME_EXTENSION_ENCODE);
	char* out_path = stringToCharArray(out);
	//nen file
	Huffman huff;
	huff.setTime(clock());
	gotoxy(2, 6);
	SetColor(10);
	cout << "Compressing ... Please wait a few seconds :\">";
	int check = huff.encoding(in_path, out_path, 0);
	
	//ket qua
	gotoxy(2, 8);
	SetColor(TEXT_COLOR_INFO);
	printf("-----> Program execution time:: %.3f (s)", (double)1.0 * (clock() - huff.getTime()) / CLOCKS_PER_SEC);
	gotoxy(2, 9);
	static int out_size = getSize(out_path);
	printf("-----> Original file size: %.5f (MB)", (float)(1.0 * in_size) / (1024 * 1024));
	gotoxy(2, 10);
	printf("-----> Compression file size: %.5f (MB)", (float)(1.0 * out_size) / (1024 * 1024));
	gotoxy(2, 11);
	printf("-----> Compression ratio: %.5f %%", (float)(1 - (float)(1.0 * out_size / in_size)) * 100);
	errorsMessage(check, 2, 11);
}

// ==================== CAC HAM HO TRO NEN-GIAI NEN FOLDER =================== //
bool isFolder(const string& path) {
	if (path.find(".") == string::npos)
		return true;

	return false;
}

bool isFile(const string& path) {
	if (path.find(".") != string::npos)
		return true;
	return false;
}

int newFolder(const string& path) {
	if (!isFolder(path))
		return 1;
	char* path_t = stringToCharArray(path);
	int result = _mkdir(path_t);
	return result;
}

vector<string> saveDirectoryStructure(const string& path) {
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
		if (isFile(s)) {
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

	return result;
}

string getPathToFolder(const string& path) {
	for (int i = path.length() - 1; i >= 0; --i)
		if (path[i] == '\\' || path[i] == '/')
			return path.substr(0, i + 1);
}

// ======================= NEN 1 FOLDER CHUA FILE ======================== //
void compressFolder() {
	cout << ">> Enter the directory path [C:\\foldername]: ";
	string in;
	getline(cin, in);
	if (!isFolder(in))
		throw "Fail to read file";
	formatPath(in);

	//duong dan den folder ma bo qua ten folder
	string pathToFolder = getPathToFolder(in);

	//tao duong dan file nen
	string out = in + FILE_NAME_EXTENSION_ENCODE;
	char* out_path = stringToCharArray(out);

	//luu lai cay thu muc
	vector<string> path = saveDirectoryStructure(in);
	ofstream output(out_path, ios::binary | ios::app);
	output << path.size();
	for (int i = 0; i < path.size(); i++){
		//ghi lai duong dan de giai nen
		string temp(path[i], pathToFolder.length(), path[i].length());
		output << temp << endl;
	}
	output.close();

	Huffman huff;
	gotoxy(2, 4);
	SetColor(10);
	cout << "Compressing ... Please wait a few seconds :\">";
	huff.setTime(clock());
	for (int i = 0; i < path.size(); ++i){
		if (isFile(path[i])) {
			char* com_path = stringToCharArray(path[i]);
			if (isCompressionFile(com_path))
				throw "There is an already compressed file";
			//tien hanh nen
			int check = huff.encoding(com_path, out_path, 1);
			if (check == 0) {
				throw "It is possible that the name of a directory of yours exists a dot...\n\tResulting in indistinguishable files or directories.";
				errorsMessage(0, 2, 6);
			}
		}
	}
	gotoxy(2, 6);
	SetColor(TEXT_COLOR_INFO);
	printf("-----> Program execution time:: %.3f (s)", (double)1.0 * (clock() - huff.getTime()) / CLOCKS_PER_SEC);
	errorsMessage(1, 2, 6);
}
