#ifndef CLIPEASY
#define CLIPEASY

#include <vector>
#include <string>

std::string Read();

void Write(const std::string &data);

std::vector<std::string> ReadFiles();

void WriteFiles(const std::vector<std::string> &file_paths);

void Clear();

#endif // CLIPEASY
