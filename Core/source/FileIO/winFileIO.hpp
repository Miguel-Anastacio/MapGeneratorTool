#include <iostream>
#include <windows.h>
#include <shobjidl.h>
#include <string>
#include <vector>

namespace FileIO
{
namespace winFileIO	
{
    /**
 * @brief Open a dialog to save an item.
 * @param path Specifies the reference to the string that will receive the target save path. [IN]
 * @param defaultFileName Specifies the default save file name. (optional)
 * @param pFilterInfo Specifies the pointer to the pair that contains filter information. (optional)
 * @note If no path was selected, the function still returns true, and the given string is unmodified.
 * @note `<windows.h>`, `<string>`, `<vector>`, `<shobjidl.h>`
 * @return Returns true if all the operations are successfully performed, false otherwise.
 */
    bool SaveFileDialog(std::wstring& path, std::wstring defaultFileName = L"", std::pair<COMDLG_FILTERSPEC*, int>* pFilterInfo = nullptr)
    {
        IFileSaveDialog* p_file_save = nullptr;
        bool are_all_operation_success = false;
        while (!are_all_operation_success)
        {
            HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
                IID_IFileSaveDialog, reinterpret_cast<void**>(&p_file_save));
            if (FAILED(hr))
                break;

            if (!pFilterInfo)
            {
                COMDLG_FILTERSPEC save_filter[1];
                save_filter[0].pszName = L"All files";
                save_filter[0].pszSpec = L"*.*";
                hr = p_file_save->SetFileTypes(1, save_filter);
                if (FAILED(hr))
                    break;
                hr = p_file_save->SetFileTypeIndex(1);
                if (FAILED(hr))
                    break;
            }
            else
            {
                hr = p_file_save->SetFileTypes(pFilterInfo->second, pFilterInfo->first);
                if (FAILED(hr))
                    break;
                hr = p_file_save->SetFileTypeIndex(1);
                if (FAILED(hr))
                    break;
            }

            if (!defaultFileName.empty())
            {
                hr = p_file_save->SetFileName(defaultFileName.c_str());
                if (FAILED(hr))
                    break;
            }

            hr = p_file_save->Show(NULL);
            if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED)) // No item was selected.
            {
                are_all_operation_success = true;
                break;
            }
            else if (FAILED(hr))
                break;

            IShellItem* p_item;
            hr = p_file_save->GetResult(&p_item);
            if (FAILED(hr))
                break;

            PWSTR item_path;
            hr = p_item->GetDisplayName(SIGDN_FILESYSPATH, &item_path);
            if (FAILED(hr))
                break;
            path = item_path;
            CoTaskMemFree(item_path);
            p_item->Release();

            are_all_operation_success = true;
        }

        if (p_file_save)
            p_file_save->Release();
        return are_all_operation_success;
    }



    /**
 * @brief Open a dialog to select item(s) or folder(s).
 * @param paths Specifies the reference to the string vector that will receive the file or folder path(s). [IN]
 * @param selectFolder Specifies whether to select folder(s) rather than file(s). (optional)
 * @param multiSelect Specifies whether to allow the user to select multiple items. (optional)
 * @note If no item(s) were selected, the function still returns true, and the given vector is unmodified.
 * @note `<windows.h>`, `<string>`, `<vector>`, `<shobjidl.h>`
 * @return Returns true if all the operations are successfully performed, false otherwise.
 */
    bool OpenFileDialog(std::vector<std::wstring>& paths, bool selectFolder = false, bool multiSelect = false)
    {
        IFileOpenDialog* p_file_open = nullptr;
        bool are_all_operation_success = false;
        while (!are_all_operation_success)
        {
            HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                IID_IFileOpenDialog, reinterpret_cast<void**>(&p_file_open));
            if (FAILED(hr))
                break;

            if (selectFolder || multiSelect)
            {
                FILEOPENDIALOGOPTIONS options = 0;
                hr = p_file_open->GetOptions(&options);
                if (FAILED(hr))
                    break;

                if (selectFolder)
                    options |= FOS_PICKFOLDERS;
                if (multiSelect)
                    options |= FOS_ALLOWMULTISELECT;

                hr = p_file_open->SetOptions(options);
                if (FAILED(hr))
                    break;
            }

            hr = p_file_open->Show(NULL);
            if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED)) // No items were selected.
            {
                are_all_operation_success = true;
                break;
            }
            else if (FAILED(hr))
                break;

            IShellItemArray* p_items;
            hr = p_file_open->GetResults(&p_items);
            if (FAILED(hr))
                break;
            DWORD total_items = 0;
            hr = p_items->GetCount(&total_items);
            if (FAILED(hr))
                break;

            for (int i = 0; i < static_cast<int>(total_items); ++i)
            {
                IShellItem* p_item;
                p_items->GetItemAt(i, &p_item);
                if (SUCCEEDED(hr))
                {
                    PWSTR path;
                    hr = p_item->GetDisplayName(SIGDN_FILESYSPATH, &path);
                    if (SUCCEEDED(hr))
                    {
                        paths.emplace_back(path);
                        CoTaskMemFree(path);
                    }
                    p_item->Release();
                }
            }

            p_items->Release();
            are_all_operation_success = true;
        }

        if (p_file_open)
            p_file_open->Release();
        return are_all_operation_success;
    }


    bool SelectFolderDialog(std::wstring& folderPath) {
        IFileOpenDialog* pFileOpen = nullptr;
        HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileOpen));
        if (FAILED(hr)) {
            std::wcout << L"Failed to create FileOpenDialog instance." << std::endl;
            return false;
        }

        // Set options to allow folder picking
        DWORD dwOptions;
        hr = pFileOpen->GetOptions(&dwOptions);
        if (SUCCEEDED(hr)) {
            hr = pFileOpen->SetOptions(dwOptions | FOS_PICKFOLDERS);
        }

        // Show the folder selection dialog
        hr = pFileOpen->Show(NULL);
        if (FAILED(hr)) {
            pFileOpen->Release();
            return false;  // User canceled or an error occurred
        }

        // Get the folder the user selected
        IShellItem* pItem = nullptr;
        hr = pFileOpen->GetResult(&pItem);
        if (SUCCEEDED(hr)) {
            PWSTR pszFolderPath = nullptr;
            hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath);
            if (SUCCEEDED(hr)) {
                folderPath = pszFolderPath;  // Copy the folder path to the output parameter
                CoTaskMemFree(pszFolderPath);
            }
            pItem->Release();
        }

        pFileOpen->Release();
        return SUCCEEDED(hr);
    }
}

}