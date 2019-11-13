#pragma once
#include "Huffman.h"

#define TEXT_COLOR 1
#define MAX_OPTION 4
const string FILE_NAME_EXTENSION_ENCODE = ".winzip";
const string FILE_NAME_EXTENSION_DECODE = "_decode.txt";

//ham dua con tro den vi tri x, y
void gotoxy(int , int );

//ham an con tro
void ShowConsoleCursor(bool);

//to mau chu
void SetColor(int);

//ham hien thi con tro moi va xoa con tro cu
void displayCursor(int, int);

//ham hien thi menu lua chon
void menu();

//ham thong bao loi hay khong
void errorsMessage(int, int, int);

//ham chuyen chuoi string sang char*
char* stringToCharArray(string);

//ham thay doi phan mo rong cua file
string changeFileExtension(string , string);

//tim duong dan cu the [path(file out) + namefile(file in)]
void findOutputPath(string , string&);

//ham nen (type = true) hoac giai nen(type = flase) 1 thu muc
void optimizeComp_Decomp_Folder(bool type);

//rut gon cac ham su dung lai nhieu
void optimizeSetup_1();
void optimizeSetup_2();

//ham main chinh chuong trinh
void displayMain();
