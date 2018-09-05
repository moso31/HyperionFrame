#include "FileReader.h"
#include "DirectXHelper.h"

FileReader::FileReader()
{
}

FileReader::~FileReader()
{
}

vector<byte> FileReader::ReadData(LPCWSTR fileName)
{
	HANDLE hFile = CreateFileW(fileName, GENERIC_READ, FILE_SHARE_READ,
		nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);			// �����ļ�
	ThrowIfFailed(hFile == INVALID_HANDLE_VALUE);							// �ж��ļ�����Ƿ���Ч

	FILE_STANDARD_INFO fileInfo = { 0 };
	ThrowIfFailed(!GetFileInformationByHandleEx(hFile,
		FileStandardInfo, &fileInfo, sizeof(fileInfo)));					// ͨ���ļ����hFile����ļ���Ϣ
	ThrowIfFailed(fileInfo.EndOfFile.HighPart != 0);

	vector<byte> fileData(fileInfo.EndOfFile.LowPart);
	ThrowIfFailed(!ReadFile(hFile, fileData.data(), static_cast<DWORD>(fileData.size()),
		nullptr, nullptr));													// ��ȡ�ļ����ݵ�fileData
	return fileData;
}

void FileReader::GetFiles(wstring filePath, int limitDepth, int depth, vector<wstring> &outFiles)
{
	if (depth == limitDepth)
		return;

	//�ļ����
	intptr_t hFile = 0;
	//�ļ���Ϣ
	struct _wfinddata_t fileinfo;
	wstring p;
	hFile = _wfindfirst(p.assign(filePath).append(L"\\*").c_str(), &fileinfo);
	if (hFile == -1)
		return;

	do
	{
		//�����Ŀ¼,����֮
		//�������,�����б�
		if ((fileinfo.attrib &  _A_SUBDIR))
		{
			if (wcscmp(fileinfo.name, L".") != 0 && wcscmp(fileinfo.name, L"..") != 0)
				GetFiles(p.assign(filePath).append(L"\\").append(fileinfo.name), limitDepth, depth + 1, outFiles);
		}
		else
		{
			outFiles.push_back(p.assign(filePath).append(L"\\").append(fileinfo.name));
		}
	} while (_wfindnext(hFile, &fileinfo) == 0);
	_findclose(hFile);
}
