/*
	Su dung thuat toan nen file khong mat du lieu (lossless compression) Static Huffman

	Nguyen nhan lua chon:
	-Thuat toan de cai dat
	-He so nen tuong doi cao
	-Can it bo nho

	Tuy nhien:
	-Can mat 2 lan duyet file khi nen
	-Luu thong tin khi de giai nen
	-Tao lai cay huffman khi giai nen
	=> Doi voi file dung luong rat thap thi se co xay ra truong hop dung luong nen > dung luong thuc

	P/s: Trong truong hop file chi co 1 loai ky tu duy nhat thi bai toan se duoc giai quyet bang
		 thuat toan RLE (Run Length Encoding)
*/

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <Windows.h>
#include <conio.h>
#include <ctime>
#include <sys/stat.h>
#include <direct.h>
#include <filesystem>

using namespace std;

const string FILE_NAME_EXTENSION_ENCODE = ".winzip";
const string FILE_NAME_EXTENSION_DECODE = "_decode.txt";

#define MAX_NODE 511		//cay nhi phan nen co 2n - 1 node (ma ASCII co 256 = n)
#define MAX_BIT_CODE 2048   //so bit toi da cua mang bitCode[]

/* ==================== cau truc 1 node cua cay huffman ==================== */
struct Node {
	char c;				  //ky tu
	int freq;			 //tan so xuat hien ky tu
	int flag;			 //danh dau con ton tai trong bang thong ke khong
	int left;			//chi so cua node con ben trai
	int right;			//chi so cua node con ben phai
};

/* ======================== cau truc cua 1 day bit ========================= */
struct bitCode {
	char* bits;			//chua ma code
	int n_bit;			//chua so luong bit
};

/* ===================== Lop thuat toan cua nen hufman ===================== */
class Huffman {
public:
	Node huffTree[MAX_NODE];						//cay huffman duoi dang array
	int n_Node = 0;									//so node co tan so > 0 (dung de giai nen sau nay)
	char typeEncode = 'h';							//loai nen neu co 1 ky tu thi nen theo RLE se tot hon

	bitCode bitCodeTable[256];						//chua day bit cua cac ky tu ASCII

	void initHuffTree();							//khoi tao cay huffman

	void freqStatistics(char*);						//thong ke tan so xuat hien trong bang huffman

	void countNode();								//dem so node co trong bang thong ke

	bool findTwoMin(int&, int&, int);				//tim vi tri 2 node co tan so thap nhat

	int buildHuffmanTree();							//xay dung cay huffman (Tra ve vi tri node root)

	void browseHuffmanTree(int , char [], int );	//ham duyet qua cay de tao 1 ma bit cho cac ky tu tu huffman tree

	void createBitCodetable(int);					//thiet lap bang bit code

	bool saveFreqStatisTable(FILE*&);				//Ghi ban thong ke tan so vao file nen

	int encoding(char*, char*);						//ma hoa du lieu va nen file

	bool recreateHuffTree(FILE*&);					//Ham tao lai cay huffman

	bool decoding(char*, char*);					//Ham giai nen

};

/* ========================== Thuat toan nen RLE =========================== */
bool runLengthEncoding(char*, char*);				//Nen file
bool runLengthDecoding(char*);						//giai nen

