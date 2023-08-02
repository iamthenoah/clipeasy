#include "clipboard.h"
#include <Windows.h>
#include <vector>
#include <string>

struct File
{
  DWORD pFiles;
  POINT pt;
  BOOL fNC;
  BOOL fWide;
};

std::string Read()
{
  if (!IsClipboardFormatAvailable(CF_UNICODETEXT))
  {
    return "";
  }

  if (!OpenClipboard(nullptr))
  {
    return "";
  }

  HANDLE clipboard_data = GetClipboardData(CF_UNICODETEXT);

  if (clipboard_data == nullptr)
  {
    CloseClipboard();
    return "";
  }

  LPCWSTR data = static_cast<LPCWSTR>(GlobalLock(clipboard_data));

  if (data == nullptr)
  {
    CloseClipboard();
    return "";
  }

  std::vector<std::string> file_paths = ReadFiles();

  if (!file_paths.empty())
  {
    return "";
  }

  std::wstring contents(data);
  GlobalUnlock(clipboard_data);
  CloseClipboard();

  return std::string(contents.begin(), contents.end());
}

void Write(const std::string &data)
{
  if (!OpenClipboard(nullptr))
  {
    return;
  }

  HGLOBAL clipboard_data = GlobalAlloc(GMEM_MOVEABLE, (data.size() + 1) * sizeof(wchar_t));

  if (clipboard_data == nullptr)
  {
    CloseClipboard();
    return;
  }

  LPWSTR data_buffer = static_cast<LPWSTR>(GlobalLock(clipboard_data));

  if (data_buffer == nullptr)
  {
    GlobalFree(clipboard_data);
    CloseClipboard();
    return;
  }

  std::wstring wide_data = std::wstring(data.begin(), data.end());
  wcscpy_s(data_buffer, wide_data.size() + 1, wide_data.c_str());

  GlobalUnlock(clipboard_data);
  EmptyClipboard();
  SetClipboardData(CF_UNICODETEXT, clipboard_data);

  CloseClipboard();
}

std::vector<std::string> ReadFiles()
{
  std::vector<std::string> file_paths;

  if (!IsClipboardFormatAvailable(CF_HDROP))
  {
    return file_paths;
  }

  if (!OpenClipboard(nullptr))
  {
    return file_paths;
  }

  HDROP hDrop = static_cast<HDROP>(GetClipboardData(CF_HDROP));

  if (hDrop == nullptr)
  {
    CloseClipboard();
    return file_paths;
  }

  UINT num_files = DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);
  char buffer[MAX_PATH];

  for (UINT i = 0; i < num_files; ++i)
  {
    DragQueryFileA(hDrop, i, buffer, sizeof(buffer));
    file_paths.push_back(buffer);
  }

  CloseClipboard();

  return file_paths;
}

void WriteFiles(const std::vector<std::string> &file_paths)
{
  if (file_paths.empty())
  {
    return;
  }

  if (!OpenClipboard(nullptr))
  {
    return;
  }

  size_t totalSize = sizeof(File);

  for (const std::string &file_path : file_paths)
  {
    totalSize += (file_path.size() + 1) * sizeof(wchar_t);
  }

  HGLOBAL hDrop = GlobalAlloc(GHND, totalSize);

  if (hDrop == nullptr)
  {
    CloseClipboard();
    return;
  }

  File *drop_files = static_cast<File *>(GlobalLock(hDrop));
  drop_files->pFiles = sizeof(File);
  drop_files->fWide = TRUE;

  wchar_t *buffer = reinterpret_cast<wchar_t *>(drop_files + 1);

  for (const std::string &file_path : file_paths)
  {
    size_t converted = mbstowcs(buffer, file_path.c_str(), file_path.size() + 1);

    if (converted == static_cast<size_t>(-1))
    {
      GlobalUnlock(hDrop);
      CloseClipboard();
      return;
    }
    buffer += (converted + 1);
  }

  GlobalUnlock(hDrop);
  EmptyClipboard();
  SetClipboardData(CF_HDROP, hDrop);

  CloseClipboard();
}

void Clear()
{
  OpenClipboard(nullptr);
  EmptyClipboard();
  CloseClipboard();
}
