#include <napi.h>
#include <vector>
#include "clipboard.h"

Napi::String ReadJs(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  std::string data = Read();
  return Napi::String::New(env, data);
}

void WriteJs(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1 || !info[0].IsString())
  {
    Napi::TypeError::New(env, "Argument must be a string.").ThrowAsJavaScriptException();
    return;
  }

  std::string utf8_data = info[0].As<Napi::String>();
  Write(utf8_data);
}

Napi::Array ReadFilesJs(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  std::vector<std::string> file_paths = ReadFiles();
  Napi::Array result = Napi::Array::New(env, file_paths.size());

  for (size_t i = 0; i < file_paths.size(); ++i)
  {
    result.Set(i, Napi::String::New(env, file_paths[i]));
  }

  return result;
}

void WriteFilesJs(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() != 1 || !info[0].IsArray())
  {
    Napi::TypeError::New(env, "Argument must be an array of strings.").ThrowAsJavaScriptException();
    return;
  }

  Napi::Array paths = info[0].As<Napi::Array>();
  std::vector<std::string> file_paths;

  for (uint32_t i = 0; i < paths.Length(); ++i)
  {
    file_paths.push_back(paths.Get(i).As<Napi::String>());
  }

  WriteFiles(file_paths);
}

void ClearJs(const Napi::CallbackInfo &info)
{
  Clear();
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set("read", Napi::Function::New(env, ReadJs));
  exports.Set("write", Napi::Function::New(env, WriteJs));
  exports.Set("readFiles", Napi::Function::New(env, ReadFilesJs));
  exports.Set("writeFiles", Napi::Function::New(env, WriteFilesJs));
  exports.Set("clear", Napi::Function::New(env, ClearJs));
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
