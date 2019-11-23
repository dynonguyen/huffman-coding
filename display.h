#pragma once
#include "Huffman.h"
#include "compress.h"
#include "decompress.h"

#define TEXT_COLOR_PROGRAM_NAME 9
#define TEXT_COLOR_TITLE 10
#define TEXT_COLOR_EXIT 4
#define TEXT_COLOR_CURSOR 6
#define TEXT_COLOR_INFO 13
#define TEXT_COLOR_ERROR 12
#define TEXT_COLOR_NO_ERROR 11
#define TEXT_COLOR 7
#define MAX_OPTION 4

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
void errorsMessage(int, int, int, int);


//rut gon cac ham su dung lai nhieu
void optimizeSetup_1();
void optimizeSetup_2();

//ham main chinh chuong trinh
void displayMain();
