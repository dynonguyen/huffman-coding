#pragma once
#include "display.h"
const string FILE_NAME_EXTENSION_ENCODE = ".winzip";
const string FILE_NAME_EXTENSION_DECODE_TEXT = "_decode.txt";

//==================== CAC HAM HO TRO NEN-GIAI NEN FILE TXT ====================//

//ham chuyen chuoi string sang char*
char* stringToCharArray(string);

//ham thay doi phan mo rong cua file
string changeFileExtension(string, string);

//tim duong dan cu the [path(file out) + namefile(file in)]
void setOutputPath(string, string&);

// ======================= CAC HAM HO TRO NEN-GIAI NEN FOLDER TXT ====================== //

//ham kiem tra 1 duong path co dan den 1 folder
bool isFolder(string);

//ham kiem tra 1 path la file hay folder
bool isFileTxt(string );

//ham tao 1 folder moi
int newFolder(string);

// chay qua tung folder luu lai path (folder chua no)
vector<string> saveDirectoryStructure(string);

//ham lay duong dan folder chua file txt (vd: C:\folder\in.txt => C:\folder)
string getFolderPath(string);

// ================================== NEN FILE TEXT =================================== //
int compressFileText();

// =========================== NEN 1 FOLDER CHUA FILE TEXT =========================== //
int compressFolderTxt();