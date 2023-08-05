#include "clipboard.h"
#include <Windows.h>
#include <shlobj.h>
#include <memory>

// inspiration from kenan2002 (https://github.com/kenan2002/electron-clipboard-ex/blob/master/src/clipboard_win.cc)

std::string Read()
{
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
  auto result = std::vector<std::string>();

  if (!OpenClipboard(nullptr))
  {
    return result;
  }

  HDROP drop_files_handle = (HDROP)GetClipboardData(CF_HDROP);

  if (!drop_files_handle)
  {
    CloseClipboard();
    return result;
  }

  UINT file_count = DragQueryFileW(drop_files_handle, 0xFFFFFFFF, NULL, 0);
  result.reserve(file_count);

  for (UINT i = 0; i < file_count; ++i)
  {
    UINT path_len = DragQueryFileW(drop_files_handle, i, NULL, 0);
    UINT buffer_len = path_len + 1;
    std::unique_ptr<WCHAR[]> buffer(new WCHAR[buffer_len]);
    path_len = DragQueryFileW(drop_files_handle, i, buffer.get(), buffer_len);

    auto input = buffer.get();
    int target_len = WideCharToMultiByte(CP_UTF8, 0, input, path_len, NULL, 0, NULL, NULL);
    std::string utf_result(target_len, '\0');
    WideCharToMultiByte(CP_UTF8, 0, input, path_len, utf_result.data(), target_len, NULL, NULL);

    result.emplace_back(utf_result);
  }

  CloseClipboard();
  return result;
}

void WriteFiles(const std::vector<std::string> &file_paths)
{
  std::vector<std::wstring> file_paths_unicode;
  file_paths_unicode.reserve(file_paths.size());

  for (auto p = file_paths.cbegin(); p != file_paths.cend(); ++p)
  {
    int target_len = MultiByteToWideChar(CP_UTF8, 0, p->c_str(), p->size(), NULL, 0);
    std::wstring path_unicode(target_len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, p->c_str(), p->size(), path_unicode.data(), target_len);

    if (path_unicode.size() > MAX_PATH)
    {
      const WCHAR *prefix = L"\\\\?\\";
      std::wstring prefixed_long_path = prefix + path_unicode;
      DWORD buffer_size = GetShortPathNameW(prefixed_long_path.c_str(), NULL, 0);
      auto buffer_pointer = std::unique_ptr<WCHAR[]>(new WCHAR[buffer_size]);
      GetShortPathNameW(prefixed_long_path.c_str(), buffer_pointer.get(), buffer_size);
      size_t offset = wcslen(prefix);
      path_unicode = buffer_pointer.get() + offset;
    }
    file_paths_unicode.emplace_back(path_unicode);
  }

  SIZE_T structure_size_in_bytes = sizeof(DROPFILES);

  for (auto p = file_paths_unicode.cbegin(); p != file_paths_unicode.cend(); ++p)
  {
    structure_size_in_bytes += (p->size() + 1) * sizeof(WCHAR);
  }

  structure_size_in_bytes += sizeof(WCHAR);
  HANDLE data_handle = GlobalAlloc(GMEM_MOVEABLE, structure_size_in_bytes);

  if (!data_handle)
  {
    return;
  }

  BYTE *data_pointer = static_cast<BYTE *>(GlobalLock(data_handle));

  if (!data_pointer)
  {
    return;
  }

  DROPFILES *drop_files_pointer = reinterpret_cast<DROPFILES *>(data_pointer);
  drop_files_pointer->pFiles = sizeof(DROPFILES);
  drop_files_pointer->fWide = TRUE;

  SIZE_T current_offset_in_bytes = sizeof(DROPFILES);

  for (auto p = file_paths_unicode.cbegin(); p != file_paths_unicode.cend(); ++p)
  {
    WCHAR *target_path = reinterpret_cast<WCHAR *>(data_pointer + current_offset_in_bytes);
    SIZE_T offset_in_chars = p->size() + 1;
    SIZE_T offset_in_bytes = offset_in_chars * sizeof(WCHAR);
    memcpy(target_path, p->c_str(), offset_in_bytes);
    current_offset_in_bytes += offset_in_bytes;
  }

  WCHAR *tail = reinterpret_cast<WCHAR *>(data_pointer + current_offset_in_bytes);
  *tail = L'\0';

  GlobalUnlock(data_handle);

  if (!OpenClipboard(nullptr))
  {
    GlobalFree(data_handle);
    return;
  }

  EmptyClipboard();

  if (!SetClipboardData(CF_HDROP, data_handle))
  {
    GlobalFree(data_handle);
    return;
  }

  CloseClipboard();
}

void Clear()
{
  OpenClipboard(nullptr);
  EmptyClipboard();
  CloseClipboard();
}
