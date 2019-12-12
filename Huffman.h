/*
	=========== DO AN NEN FILE MON CTDL&GT - HCMUS K18 - 18CTT5/C ===========
	//Nhom thuc hien:
	18120634 - Nguyen Le Anh Tuan
	18120644 - Nguyen Cat Tuong

	// ====== GIOI THIEU VE THUAT TOAN NEN ======= //

	Su dung thuat toan nen file khong mat du lieu (lossless compression) Static Huffman
	Nguyen nhan lua chon:
	-Thuat toan de cai dat
	-He so nen tuong doi cao file text
	-Can it bo nho

	Tuy nhien:
	-Can mat 2 lan duyet file khi nen
	-Luu thong tin khi de giai nen
	-Tao lai cay huffman khi giai nen
	=> Doi voi file dung luong rat thap thi se co xay ra truong hop dung luong nen > dung luong thuc
	=> Khong nen tot voi nhung file da nen nhu mp3, mp4, jpg, ....

	P/s: Trong truong hop file chi co 1 loai ky tu duy nhat thi bai toan se duoc giai quyet bang
		 thuat toan RLE (Run Length Encoding)

	//Project duoc cap nhat, sua chua tai: https://github.com/TuanNguyen2504/compress-static-huffman.git
	//Project dung de tham khao. Neu co su dung trong muc dich lam do an vui long ghi nguon day du. Cam on !


*/

#pragma once
#ifndef _HUFFMAN_
#define _HUFFMAN_

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <Windows.h>
#include <conio.h>
#include <ctime>
#include <sys/stat.h>
#include <direct.h>
#include <filesystem>
#include <string.h>
#include <iomanip>

#pragma comment(lib, "User32.lib")
using namespace std;

#define MAX_NODE 511		//cay nhi phan nen co 2n - 1 node (ma ASCII co 256 = n)
#define MAX_BIT_CODE 2048   //so bit toi da cua mang bitCode[]
#define MAX_BUFF 65536		//so luong luu tru toi da buffer khi doc file
#define FILE_CHAR 'f'
#define FOLDER_CHAR 'F'
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
private:
	//luu thoi gian chay
	clock_t _time;
public:
	void setTime(const clock_t& time) {
		_time = time;
	};
	clock_t& getTime() {
		return _time;
	}

	Node huffTree[MAX_NODE];						//cay huffman duoi dang array
	int n_Node = 0;									//so node co tan so > 0 (dung de giai nen sau nay)
	char typeEncode = 'h';							//loai nen neu co 1 ky tu thi nen theo RLE se tot hon
	char formEncode;								//nen folder hay nen tai tin

	bitCode bitCodeTable[256];						//chua day bit cua cac ky tu ASCII

	void initHuffTree();							//khoi tao cay huffman

	void freqStatistics(char*);						//thong ke tan so xuat hien trong bang huffman

	void countNode();								//dem so node co trong bang thong ke

	bool findTwoMin(int&, int&, int);				//tim vi tri 2 node co tan so thap nhat

	int buildHuffmanTree();							//xay dung cay huffman (Tra ve vi tri node root)

	void browseHuffmanTree(int , char [], int );	//ham duyet qua cay de tao 1 ma bit cho cac ky tu tu huffman tree

	void createBitCodetable(int);					//thiet lap bang bit code

	bool saveFreqStatisTable(FILE*&);				//Ghi ban thong ke tan so vao file nen

	int encoding(char*, char*, int);				//ma hoa du lieu va nen file

	bool recreateHuffTree(FILE*&);					//Ham tao lai cay huffman

	bool decoding(char*, char*);					//Ham giai nen 1 file

	bool decodingFolder(FILE*&, char*);				//Ham giai nen 1 file
};

/* ========================== Thuat toan nen RLE =========================== */
bool runLengthEncoding(char*, char*);				//Nen file
bool runLengthDecoding(char*);						//giai nen

#endif 
