#include <node.h>
#include <vector>
#include "clipboard.h"

void Read(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  v8::Isolate *isolate = args.GetIsolate();
  std::string data = Read();
  args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, data.c_str()).ToLocalChecked());
}

void Write(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  v8::Isolate *isolate = args.GetIsolate();

  if (args.Length() != 1 || !args[0]->IsString())
  {
    v8::Local<v8::String> message = v8::String::NewFromUtf8(isolate, "Argument must be a string").ToLocalChecked();
    isolate->ThrowException(v8::Exception::TypeError(message));
    return;
  }

  v8::String::Utf8Value utf8_data(isolate, args[0].As<v8::String>());
  Write(*utf8_data);
}

void ReadFiles(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  v8::Isolate *isolate = args.GetIsolate();
  std::vector<std::string> file_paths = ReadFiles();
  v8::Local<v8::Array> result = v8::Array::New(isolate, static_cast<int>(file_paths.size()));

  for (size_t i = 0; i < file_paths.size(); ++i)
  {
    v8::Local<v8::String> path = v8::String::NewFromUtf8(isolate, file_paths[i].c_str()).ToLocalChecked();
    (void)result->Set(isolate->GetCurrentContext(), static_cast<uint32_t>(i), path);
  }

  args.GetReturnValue().Set(result);
}

void WriteFiles(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  v8::Isolate *isolate = args.GetIsolate();

  if (args.Length() != 1 || !args[0]->IsArray())
  {
    v8::Local<v8::String> message = v8::String::NewFromUtf8(isolate, "Argument must be an array of strings").ToLocalChecked();
    isolate->ThrowException(v8::Exception::TypeError(message));
    return;
  }

  v8::Local<v8::Array> paths = v8::Local<v8::Array>::Cast(args[0]);
  std::vector<std::string> file_paths;

  for (uint32_t i = 0; i < paths->Length(); ++i)
  {
    v8::Local<v8::Value> pathValue = paths->Get(isolate->GetCurrentContext(), i).ToLocalChecked();
    v8::String::Utf8Value utf8_path(isolate, v8::Local<v8::String>::Cast(pathValue));
    file_paths.push_back(*utf8_path);
  }

  WriteFiles(file_paths);
}

void Clear(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  Clear();
}

void Initialize(v8::Local<v8::Object> exports)
{
  NODE_SET_METHOD(exports, "read", Read);
  NODE_SET_METHOD(exports, "write", Write);
  NODE_SET_METHOD(exports, "readFiles", ReadFiles);
  NODE_SET_METHOD(exports, "writeFiles", WriteFiles);
  NODE_SET_METHOD(exports, "clear", Clear);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
