//
// Created by merika on 9/17/2024.
//

#include "FileDialogs.hpp"

#include <Windows.h>
#include <string>
#include <shobjidl.h>

namespace forza_saveswapper {

    TFileOpenResult FileDialogs::OpenFile() {
        TFileOpenResult result;
        HRESULT f_SysHr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (FAILED(f_SysHr)) {
            return result;
        }

        IFileOpenDialog* f_FileSystem;
        f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
        if (FAILED(f_SysHr)) {
            CoUninitialize();
            return result;
        }

        f_SysHr = f_FileSystem->Show(nullptr);
        if (FAILED(f_SysHr)) {
            f_FileSystem->Release();
            CoUninitialize();
            return result;
        }

        IShellItem* f_Files;
        f_SysHr = f_FileSystem->GetResult(&f_Files);
        if (FAILED(f_SysHr)) {
            f_FileSystem->Release();
            CoUninitialize();
            return result;
        }

        PWSTR f_Path;
        f_SysHr = f_Files->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
        if (FAILED(f_SysHr)) {
            f_Files->Release();
            f_FileSystem->Release();
            CoUninitialize();
            return result;
        }

        std::wstring wPath(f_Path);
        const std::string path(wPath.begin(), wPath.end());
        result.Path = path;

        const size_t slash = result.Path.find_last_of("/\\");
        result.SelectedFileName = result.Path.substr(slash + 1);

        CoTaskMemFree(f_Path);
        f_Files->Release();
        f_FileSystem->Release();
        CoUninitialize();
        result.Succeeded = true;
        return result;
    }

    TFileSaveResult FileDialogs::SaveFile() {
        TFileSaveResult result{};
        HRESULT f_SysHr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (FAILED(f_SysHr)) {
            return result;
        }

        IFileSaveDialog* f_FileSystem;
        f_SysHr = CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&f_FileSystem));
        if (FAILED(f_SysHr)) {
            CoUninitialize();
            return result;
        }

        f_SysHr = f_FileSystem->Show(nullptr);
        if (FAILED(f_SysHr)) {
            f_FileSystem->Release();
            CoUninitialize();
            return result;
        }

        IShellItem* f_Files;
        f_SysHr = f_FileSystem->GetResult(&f_Files);
        if (FAILED(f_SysHr)) {
            f_FileSystem->Release();
            CoUninitialize();
            return result;
        }

        PWSTR f_Path;
        f_SysHr = f_Files->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
        if (FAILED(f_SysHr)) {
            f_Files->Release();
            f_FileSystem->Release();
            CoUninitialize();
            return result;
        }

        std::wstring wPath(f_Path);
        const std::string path(wPath.begin(), wPath.end());
        result.Path = path;

        CoTaskMemFree(f_Path);
        f_Files->Release();
        f_FileSystem->Release();
        CoUninitialize();
        result.Succeeded = true;
        return result;
    }
} // forza_saveswapper