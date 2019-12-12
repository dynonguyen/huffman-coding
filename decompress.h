#pragma once
#ifndef _DECOMPRESS_
#define _DECOMPRESS_
#include "compress.h"
#define MAX_CHAR 300
// ================== GIAI NEN FILE ================== //

//ham giai nen file tai path goc
void extractHereFile(const string&);

//ham giai nen file den 1 duong path khac
void extractFile(const string&);

// ================= GIAI NEN FOLDER ================ //
void decompressFolder(const string&);
void decompressFolderTo(const string& );

// =========== GIAI NEN CHUNG CHO 2 LOAI ========== //
void extractHere();
void extractTo();
#endif 
