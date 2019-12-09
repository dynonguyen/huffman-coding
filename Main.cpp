#include "display.h"

int main() {
	try {
		displayMain();
		SetColor(7);
		system("pause");
		return 0;
	}
	catch (const char* error) {
		
		system("cls");
		SetColor(4);
		cerr << "\n!===== ERROR: " << error << " =====!" << endl;
		SetColor(7);
		system("pause");
		return -1;
	}	
}