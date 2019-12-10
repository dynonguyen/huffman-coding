#include "Huffman.h"
// ============================ KHOI TAO ====================================//
void Huffman::initHuffTree() {
	for (int i = 0; i < 256; i++) {
		this->huffTree[i].c = i;
		this->huffTree[i].freq = 0;
		this->huffTree[i].flag = true;
		this->huffTree[i].left = -1;
		this->huffTree[i].right = -1;
		this->n_Node = 0;
	}
}

void Huffman::freqStatistics(char* path) {

	//mo file doc
	FILE* in = NULL;
	errno_t err = fopen_s(&in, path, "rb");
	if (in == NULL) {
		return;
	}

	char c[MAX_BUFF];
	while (1) {
		int n = fread(&c, sizeof(char), MAX_BUFF, in);
		for (int i = 0; i < n; ++i) {
			if (c[i] < 0)
				this->huffTree[(int)(c[i] + 256)].freq++;
			else
				this->huffTree[c[i]].freq++;
		}	
		if (feof(in))
			break;
	}

	fclose(in);
}

void Huffman::countNode() {
	for (int i = 0; i < 256; i++) {
		if (huffTree[i].freq > 0)
			++this->n_Node;
	}
}

void swap(int& a, int& b) {
	int t = a;
	a = b;
	b = t;
}

// ============================ ENCODING ====================================//
bool Huffman::findTwoMin(int& pos_min_1, int& pos_min_2, int n_node) {

	//Khoi tao gia tri mac dinh
	pos_min_1 = -1;
	pos_min_2 = -1;

	for (int i = 0; i < n_node; i++) {
		//tan so phai > 0 va con trong bang thong ke
		if (huffTree[i].freq > 0 && huffTree[i].flag == true) {

			if (pos_min_1 == -1)
				pos_min_1 = i;
			else if (pos_min_2 == -1)
				pos_min_2 = i;
			else {
				//freq[i] > freq[pos_min_1] && freq[pos_min_2] < freq[pos_min_1]
				if (huffTree[pos_min_1].freq > huffTree[pos_min_2].freq) {
					if (huffTree[i].freq < huffTree[pos_min_1].freq)
						pos_min_1 = i;
				}
				else {
					if (huffTree[i].freq < huffTree[pos_min_2].freq)
						pos_min_2 = i;
				}
			}

		}
	}

	//da tim thay 2 vi tri min (pos_min nao co freq lon hon thi nam ben phai, neu bang
	//thi ky tu nao ma ASCII lon hon nam ben phai
	if (pos_min_1 != -1 && pos_min_2 != -1) {
		if (huffTree[pos_min_1].freq > huffTree[pos_min_2].freq || (huffTree[pos_min_1].freq == huffTree[pos_min_2].freq && huffTree[pos_min_1].c > huffTree[pos_min_2].c))
			swap(pos_min_1, pos_min_2);
		return true;
	}

	return false;
}

int Huffman::buildHuffmanTree() {
	//vi tri node cuoi
	int nNode = 256;
	int pos_min_1, pos_min_2;
	bool found = true;

	while (1) {

		found = this->findTwoMin(pos_min_1, pos_min_2, nNode);

		//con 1 node root
		if (!found)
			break;

		//gan freq cho node cha va 2 node con cua no
		huffTree[nNode].freq = huffTree[pos_min_1].freq + huffTree[pos_min_2].freq;
		huffTree[nNode].left = pos_min_1;
		huffTree[nNode].right = pos_min_2;

		//gan ky tu cho node cha (lay ki tu con ben trai)
		if (huffTree[pos_min_1].c > huffTree[pos_min_2].c)
			huffTree[nNode].c = huffTree[pos_min_2].c;
		else
			huffTree[nNode].c = huffTree[pos_min_1].c;

		//thay doi flag ton tai trong bang thong ke
		huffTree[pos_min_1].flag = false;
		huffTree[pos_min_2].flag = false;
		huffTree[nNode].flag = true;

		//tang so node cuoi len
		++nNode;
	}

	return nNode - 1;	//vi tri node root
}

void Huffman::browseHuffmanTree(int pos_root, char bitCode[], int nBitCode) {

	//tim ra cac node la va tao ma bit cho ky tu node do
	if (huffTree[pos_root].left == -1 && huffTree[pos_root].right == -1) {

		this->bitCodeTable[pos_root].bits = new char[nBitCode];
		this->bitCodeTable[pos_root].n_bit = nBitCode;
		for (int i = 0; i < nBitCode; i++) {
			this->bitCodeTable[pos_root].bits[i] = bitCode[i];
		}

		//gan ky tu ket thuc chuoi
		this->bitCodeTable[pos_root].bits[nBitCode] = NULL;
		return;
	}
	else {
		//Qua trai = 0
		bitCode[nBitCode] = '0';
		browseHuffmanTree(huffTree[pos_root].left, bitCode, nBitCode + 1);

		//Qua phai = 1
		bitCode[nBitCode] = '1';
		browseHuffmanTree(huffTree[pos_root].right, bitCode, nBitCode + 1);
	}
}

void Huffman::createBitCodetable(int pos_root) {
	//khoi tao bang day ma bit
	for (int i = 0; i < 256; i++) {
		this->bitCodeTable[i].n_bit = 0;
		this->bitCodeTable[i].bits = NULL;
	}

	char bitCode[MAX_BIT_CODE];
	int nBitCode = 0;

	browseHuffmanTree(pos_root, bitCode, nBitCode);
}

bool Huffman::saveFreqStatisTable(FILE*& outFile) {

	fwrite(&this->typeEncode, sizeof(char), 1, outFile);				//Ghi Kieu nen
	fwrite(&this->n_Node, sizeof(int), 1, outFile);						//Ghi so node freq > 0

	for (int i = 0; i < 256; i++) {
		if (huffTree[i].freq > 0) {
			char c = i;
			fwrite(&c, sizeof(char), 1, outFile);						//ky tu c
			fwrite(&huffTree[i].freq, sizeof(int), 1, outFile);			//Tan so cua ky tu c
		}
	}

	return true;
}

int Huffman::encoding(char* in_path, char* out_path,int type) {
	//tao cay huffman
	this->initHuffTree();

	//tao bang thong ke
	this->freqStatistics(in_path);

	//dem node freq > 0 trong huffman tree
	this->countNode();

	//Neu co 1 node thi giai thuat nen theo RLE va tra ve -1
	if (type == 0)
		if (this->n_Node == 1) {
			if (runLengthEncoding(in_path, out_path)) {
				this->typeEncode = 'r';		//Nen theo kieu RLE
				return -1;
			}
			//Khong thanh cong
			return 0;
		}

	//xay dung cay huffman
	int pos_root = this->buildHuffmanTree();

	//tao bang bitcode
	this->createBitCodetable(pos_root);

	//mo file de Ghi
	FILE* outFile;
	//nen file
	if(type == 0)
		errno_t err = fopen_s(&outFile, out_path, "wb");
	//nen folder
	else
		errno_t err = fopen_s(&outFile, out_path, "ab");

	if (!outFile) {
		return 0;
	}

	//ghi bang thong ke de giai nen sau nay
	this->saveFreqStatisTable(outFile);

	//Bien dung de luu day bit xuat ra (du 1 byte (1 char out) xuat ra file)
	char out = 0;

	//vi tri bit hien hanh trong day bit dang xet
	int pos_bit = 7;

	//mo file de doc
	FILE* inFile;
	errno_t err2 = fopen_s(&inFile, in_path, "rb");
	if (!inFile) {
		return 0;
	}

	char c[MAX_BUFF];
	char ch[MAX_BUFF];
	while (1) {
		int n = fread(&c, sizeof(char), MAX_BUFF, inFile);
		int count = 0;
		for (int i = 0; i < n; ++i) {
			int temp_c = c[i];
			if (c[i] < 0)
				temp_c = (int)(c[i] + 256);
			for (int j = 0; j < this->bitCodeTable[temp_c].n_bit; ++j) {

				if (this->bitCodeTable[temp_c].bits[j] == '1')
					out = out | (1 << pos_bit);	//shiftLeft bat bit 1 tai vi tri pos_bit
				if (!pos_bit) {
					ch[count++] = out;
					out = 0;
					pos_bit = 7;
				}
				else
					--pos_bit;
			}
		}
		fwrite(ch, sizeof(char), count, outFile);
		//ki tu ket thuc file
		if (feof(inFile))
			break;
	}


	//Xu ly truong hop byte cuoi cua ky tu out chua du 8 bit
	int n_BitFinal = 7 - pos_bit;		//so bit cua ky tu out o byte cuoi

	//out chua du 8 bit
	if (n_BitFinal)
		fwrite(&out, sizeof(char), 1, outFile);

	fclose(inFile);
	fclose(outFile);

	//Thanh cong
	return 1;
}

/* ========================== Thuat toan nen RLE =========================== */
bool runLengthEncoding(char* in_path, char* out_path) {
	ifstream in(in_path, ios::binary);
	if (in.fail())
		return false;

	ofstream out(out_path, ios::binary | ios::trunc);
	if (out.fail())
		return false;

	char typeDecode = 'r';
	out.write(&typeDecode, sizeof(char));			//ghi kieu giai nen

	char c1, c2;
	int count = 1;
	in.read(&c1, sizeof(char));
	while (1) {
		in.read(&c2, sizeof(char));
		if (c2 != c1) {
			out << count;
			//truong hop ky tu la 1 so, tranh gay nham lan khi giai nen
			if (c1 <= '9' && c1 >= '0') {
				out << '-';
			}
			out.write(&c1, sizeof(char));
			c1 = c2;
			count = 0;
		}

		if (in.eof()) {
			out << count;
			//truong hop ky tu la 1 so, tranh gay nham lan khi giai nen
			if (c1 <= '9' && c1 >= '0') {
				out << '-';
			}
			out.write(&c1, sizeof(char));
			break;
		}

		++count;
	}

	in.close();
	out.close();
	return true;
}

bool runLengthDecoding(char* in_path) {

	//mo file giai nen
	ifstream in(in_path, ios::binary);
	if (in.fail())
		return false;

	//tao file ghi du lieu da giai nen
	//Tao ten file giai nen
	char* nameFile = new char[18 + strlen(in_path)];
	int count = 0;
	for (int i = 0; i < strlen(in_path); ++i) {
		if (in_path[i] == '.')
			break;
		nameFile[i] = in_path[i];
		++count;
	}
	string s = "_decode.txt";
	for (int i = 0; i < s.length(); ++i) {
		nameFile[count + i] = s[i];
	}
	nameFile[count + s.length()] = NULL;

	ofstream out(nameFile, ios::binary | ios::trunc);
	if (out.fail())
		return false;

	//nhap ky tu
	char typeDecode;
	in.read(&typeDecode, sizeof(char));	//lay kieu giai nen
	int n;
	char c;
	while (1) {
		in >> n;
		in.read(&c, sizeof(char));
		if (in.eof())
			break;
		if (c == '-')
			in.read(&c, sizeof(char));

		for (int i = 0; i < n; ++i) {
			out.write(&c, sizeof(char));
		}
	}

	in.close();
	out.close();
	return true;
}

// ============================ DECODING ====================================//
bool Huffman::recreateHuffTree(FILE*& in) {

	//khoi tao lai cau huffman
	this->initHuffTree();

	//doc kieu nen
	fread(&this->typeEncode, sizeof(char), 1, in);

	//neu kieu giai nen la RLE thi thoat ra khong doc nua
	if (this->typeEncode == 'r') {
		fclose(in);
		return true;
	}

	//doc so node
	fread(&this->n_Node, sizeof(int), 1, in);

	char c;
	int freq;
	for (int i = 0; i < this->n_Node; i++) {
		fread(&c, sizeof(char), 1, in);
		fread(&freq, sizeof(int), 1, in);
		if (c < 0)
			huffTree[(int)(c + 256)].freq = freq;
		else
			huffTree[c].freq = freq;
	}

	return true;
}

bool Huffman::decoding(char* in_path, char* out_path) {

	//mo file doc
	FILE* in;
	errno_t err = fopen_s(&in, in_path, "rb");
	if (!in) {
		return 0;
	}

	//tai tao lai cay huffman
	this->recreateHuffTree(in);

	if (this->typeEncode == 'r')
		return runLengthDecoding(in_path);

	int pos_root = this->buildHuffmanTree();

	this->countNode();

	//tao lai bang bitcode
	this->createBitCodetable(pos_root);

	//So chu trong file de tranh bi du bit
	double n_Char = 0;
	for (int i = 0; i < 256; i++) {
		if (huffTree[i].freq > 0)
			n_Char += huffTree[i].freq;
	}

	//vi tri hien hanh tren cay huffman
	int pos_current = pos_root;

	//so ky tu hien tai
	double countChar = 0;

	//mo file de ghi
	FILE* out;
	errno_t err_2 = fopen_s(&out, out_path, "wb");
	if (!out) {
		return 0;
	}

	char c[MAX_BUFF];
	char ch[MAX_BUFF];

	while (1) {
		int n = fread(&c, sizeof(char), MAX_BUFF, in);
		int count = 0;
		for (int k = 0; k < n; ++k) {
			for (int i = 7; i >= 0; --i) {
				int getBit_i = (c[k] >> i) & 1;						//lay bit thu i cua ky tu c

				if (getBit_i == 1)
					pos_current = huffTree[pos_current].right;		//di qua phai
				else
					pos_current = huffTree[pos_current].left;		//di qua trai

				//Tim den node la chua ky tu
				if (huffTree[pos_current].left == -1 && huffTree[pos_current].right == -1) {
					ch[count++] = huffTree[pos_current].c;
					//tang so ky tu hien tai
					++countChar;
					//da du so ky tu
					if (countChar == n_Char) {
						fwrite(&ch, sizeof(char), count, out);
						fclose(in);
						fclose(out);
						return true;
					}

					//tro ve goc cua cay huffman
					pos_current = pos_root;
				}
			}
		}
		fwrite(&ch, sizeof(char), count, out);
		if (feof(in))
			break;
	}

	fclose(in);
	fclose(out);
	return true;
}

bool Huffman::decodingFolder(FILE*& in, char* out_path) {

	//mo file doc
	if (!in) {
		return 0;
	}

	//tai tao lai cay huffman
	this->recreateHuffTree(in);

	int pos_root = this->buildHuffmanTree();

	this->countNode();

	//tao lai bang bitcode
	this->createBitCodetable(pos_root);

	//So chu trong file de tranh bi du bit
	double n_Char = 0;
	for (int i = 0; i < 256; i++) {
		if (huffTree[i].freq > 0)
			n_Char += huffTree[i].freq;
	}

	//vi tri hien hanh tren cay huffman
	int pos_current = pos_root;

	//so ky tu hien tai
	double countChar = 0;

	//ky tu hien tai
	char c;

	//mo file de ghi
	FILE* out;
	errno_t err_2 = fopen_s(&out, out_path, "wb");
	if (!out) {
		return 0;
	}
	while (1) {
		fread(&c, sizeof(char), 1, in);
		if (feof(in))
			break;
		for (int i = 7; i >= 0; --i) {
			int getBit_i = (c >> i) & 1;						//lay bit thu i cua ky tu c

			if (getBit_i == 1)
				pos_current = huffTree[pos_current].right;		//di qua phai
			else
				pos_current = huffTree[pos_current].left;		//di qua trai

			//Tim den node la chua ky tu
			if (huffTree[pos_current].left == -1 && huffTree[pos_current].right == -1) {
				fwrite(&huffTree[pos_current].c, sizeof(char), 1, out);

				//tang so ky tu hien tai
				++countChar;

				//da du so ky tu
				if (countChar == n_Char) {
					fclose(out);
					return true;
				}

				//tro ve goc cua cay huffman
				pos_current = pos_root;
			}
		}

	}

	fclose(out);
	return true;
}
