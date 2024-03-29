#include "image_analyzer.hpp"

using namespace std;


int main(int argc, char** args) {

	fstream inputFile;
	ofstream outFile("histogram.txt");
	char PGM_hdr1[2];
	int width = 0;
	int height = 0;

	if (argc == 1) {
		cout << "Please specify input file(s)" << endl;
		return 1;
	}

	for (int i = 1; i < argc; i++) {
		inputFile.open(args[i], ios::binary | ios::in);

		if (!inputFile.is_open()) {
			cout << "File: " << args[i] << " is not accessible" << endl;
			continue;
		}
		cout << "File: " << args[i] << " is opened" << endl;

		inputFile.read(PGM_hdr1, 2);
		if (PGM_hdr1[0] != 0x050 || PGM_hdr1[1] != 0x35) continue;
		cout << "PGM headers detected" << endl;
		inputFile.seekg(static_cast<int>(inputFile.tellg()) + 1);
		
		unsigned int digit_b = static_cast<unsigned int>(inputFile.tellg());
		char sym;
		do {
			inputFile.read(&sym, 1);

		} while (sym != 0x20);

		unsigned int digit_e = static_cast<unsigned int>(inputFile.tellg()) - 2;
		int j = 0;
		for (int d = '0'; d <= '9'; d++, j++) alphabet[d] = j;

		int c = 1;
		for (j = digit_e; j >= digit_b; j--, c *= 10) {
			inputFile.seekg(j);
			inputFile.read(&sym, 1);
			width += alphabet[sym] * c;
		}
		cout << "Width: " << width << endl;

		digit_b = digit_e + 2;
		c = 1;
		do {
			inputFile.read(&sym, 1);
		} while (sym != 0x0A);
		digit_e = static_cast<unsigned int>(inputFile.tellg()) - 2;
		
		for (j = digit_e; j >= digit_b; j--, c *= 10) {
			inputFile.seekg(j);
			inputFile.read(&sym, 1);
			height += alphabet[sym] * c;
		}

		cout << "Height: " << height << endl;


		for (int d = 0; d < 256; ++d) hystogram[d] = 0;
		inputFile.seekg(digit_e + 6);
		for (int d = 0; d < (width * height); ++d) {
			if (inputFile.eof()) break;
			inputFile.read(&sym, 1);
			hystogram[static_cast<unsigned char>(sym)] += 1;
		}
		cout << endl;	

		/*for (j = 0; j < 256; j++) {
			cout << "[" << j << "]";
			for (c = 0; c < hystogram[j]; c++)
				cout << "|";
			cout << endl;	
		}*/
		inputFile.close();

		outFile << "File: " << args[i] << endl;
		outFile << "Width: " << width << " Height:" << height << endl;
		for (j = 0; j < 256; j++) {
			outFile << "[" << j << "]";
			for (c = 0; c < hystogram[j]; c++)
				outFile << "|";
			outFile << endl;
		}

		outFile << "End of file " << args[i] << endl << endl;
		outFile.close();
	}


	return 0;
}