#include "clipboard.h"
#include <Windows.h>
#include <vector>
#include <string>

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

  EmptyClipboard();
  HDROP hDrop = GlobalAlloc(GHND | GMEM_ZEROINIT, sizeof(DROPFILES) + (file_paths.size() + 1) * MAX_PATH);

  if (hDrop == nullptr)
  {
    CloseClipboard();
    return;
  }

  DROPFILES *drop_files = static_cast<DROPFILES *>(GlobalLock(hDrop));
  drop_files->pFiles = sizeof(DROPFILES);
  drop_files->fWide = FALSE;

  char *buffer = reinterpret_cast<char *>(drop_files + 1);

  for (const std::string &file_path : file_paths)
  {
    strcpy_s(buffer, MAX_PATH, file_path.c_str());
    buffer += MAX_PATH;
  }

  GlobalUnlock(hDrop);
  SetClipboardData(CF_HDROP, hDrop);

  CloseClipboard();
}

void Clear()
{
  OpenClipboard(nullptr);
  EmptyClipboard();
  CloseClipboard();
}
