#include "clipboard.h"
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

std::string Read()
{
  NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
  NSArray *classes = @[ [NSString class] ];
  NSDictionary *options = @{};

  if (![pasteboard canReadObjectForClasses:classes options:options])
  {
    return "";
  }

  NSString *contents = [pasteboard stringForType:NSPasteboardTypeString];

  if (!contents)
  {
    return "";
  }

  std::vector<std::string> files = ReadFiles();

  if (!files.empty())
  {
    return "";
  }

  return [contents UTF8String];
}

void Write(const std::string &data)
{
  NSString *contents = [NSString stringWithUTF8String:data.c_str()];
  NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
  [pasteboard clearContents];
  [pasteboard setString:contents forType:NSPasteboardTypeString];
}

std::vector<std::string> ReadFiles()
{
  NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
  NSArray *classes = @[ [NSURL class] ];
  NSDictionary *options = @{NSPasteboardURLReadingFileURLsOnlyKey : @YES};

  if (![pasteboard canReadObjectForClasses:classes options:options])
  {
    return std::vector<std::string>();
  }

  NSArray *items = [pasteboard readObjectsForClasses:classes options:options];
  std::vector<std::string> file_paths;

  for (NSURL *url in items)
  {
    NSString *path = [url path];
    file_paths.push_back([path UTF8String]);
  }

  return file_paths;
}

void WriteFiles(const std::vector<std::string> &file_paths)
{
  NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
  [pasteboard clearContents];

  NSMutableArray *urls = [NSMutableArray array];

  for (const std::string &file_path : file_paths)
  {
    NSString *path = [NSString stringWithUTF8String:file_path.c_str()];
    NSURL *url = [NSURL fileURLWithPath:path];
    [urls addObject:url];
  }

  [pasteboard writeObjects:urls];
}

void Clear()
{
  NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
  [pasteboard clearContents];
}
