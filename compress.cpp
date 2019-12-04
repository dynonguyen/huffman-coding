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
	return res;
}

// ============================== NEN FILE ================================== //
int compressFile(float info[]) {
	cout << ">> Enter the original file path (EX: C:\\filename.txt): ";
	string in;
	getline(cin, in);
	if (!isFile(in))
		return 0;
	
	char* in_path = stringToCharArray(in);
	//file dau ra
	string out = changeFileExtension(in, FILE_NAME_EXTENSION_ENCODE);
	char* out_path = stringToCharArray(out);

	//tinh thoi gian thuc thi chuong trinh
	clock_t start = clock();

	//nen file
	Huffman huff;
	int check = huff.encoding(in_path, out_path, 0);
	
	//giai phong vung nho
	Free(in_path);
	Free(out_path);

	//luu cac thong tin file
	info[0] = (double)1.0 * (clock() - start) / CLOCKS_PER_SEC;
	info[1] = getSize(in);
	info[2] = getSize(out);
	return check;
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
	Free(path_t);
	return result;
}

string getFolderPath(const string& path) {
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
		errorsMessage(0, 2, 6, 0);
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

	Free(in_path);
	return result;
}

// ======================= NEN 1 FOLDER CHUA FILE ======================== //

int compressFolder(float info[]) {
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

	//thoi gian chay chuong trinh
	clock_t start = clock();

	for (int i = 0; i < path.size(); ++i){
		if (isFile(path[i])) {
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

	info[0] = (double)1.0 * (clock() - start) / CLOCKS_PER_SEC;
	return 1;
}
