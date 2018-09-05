#pragma once
#include <Windows.h>
#include <io.h>
#include <string>
#include <vector>

using namespace std;

struct byteString 
{
	byteString(const byteString* bStr)
	{
		data = bStr->data;
		length = bStr->length;
	}
	byteString(unsigned int _size)
	{
		data = new byte[_size];
		length = _size;
	}
	~byteString()
	{
		delete[] data;
		data = nullptr;
	}
	byte* data;
	unsigned int length;
};

class FileReader
{
public:
	FileReader();
	~FileReader();

	static vector<byte> ReadData(LPCWSTR fileName);
	static void GetFiles(wstring filePath, int limitDepth, int depth, vector<wstring> &outFiles);
};

