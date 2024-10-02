#pragma once
#include "winFileIO.hpp"
namespace FileIO
{
	void OpenFile(std::vector<std::wstring>& paths, bool selectFolder = false, bool multiSelect = false)
	{
#ifdef _WIN32
		winFileIO::OpenFileDialog(paths, selectFolder, multiSelect);
#endif

	};

	void SaveFile(std::wstring& path, std::wstring defaultFileName = L"", std::pair<COMDLG_FILTERSPEC*, int>* pFilterInfo = nullptr)
	{
#ifdef _WIN32
		winFileIO::SaveFileDialog(path, defaultFileName);
#endif
	}

	void SelectDirectory(std::wstring& path)
	{
#ifdef _WIN32
		winFileIO::SelectFolderDialog(path);
#endif
	}
}