#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

const char* USING_MSG = "Using: base64 <INPUTFILE>";
const char* LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char* encode(char* input, int size, int* final_size)
{
	int encoded_size = (int)((size + 4 - (size % 4)) *  8.0 / 6.0);
	char* encoded_data = (char*)malloc(encoded_size);
	
	int i = 0;
	int j = 0;
	while (j < size)
	{
		char a = input[j++];
		char b = input[j++];
		char c = input[j++];

		encoded_data[i++] = (a & 252)>>2;
		encoded_data[i++] = (a & 3)<<4		| (b & 240)>>4;
		encoded_data[i++] = (b & 15)<<2	| (c & 192) >> 6;
		encoded_data[i++] = (c & 63);
	}

	*final_size = encoded_size;

	for (int i = 0; i < encoded_size; i++)
	{
		encoded_data[i] = LETTERS[encoded_data[i]];
	}
	return encoded_data;
}

int main(int argc, char** argv)
{
	if (argc == 0)
	{
		printf(USING_MSG);
		return 1;
	}

	char* data;
	streampos size;
	ifstream file(argv[1], ios::in | ios::binary | ios::ate);

	if (file.is_open())
	{
		size = file.tellg();
		data = new char[size];

		file.seekg(0, ios::beg);
		file.read(data, size);
		file.close();
	}
	else {
		printf("Error openning the file.\n");
		return 1;
	}

	int final_size;
	char* final_data = encode(data, size, &final_size);
	printf_s("%.*s\n", final_size, final_data);
	return 0;
}