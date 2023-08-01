#include "clipboard.h"
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/Atoms.h>
#include <vector>
#include <string>
#include <cstring>

std::string Read()
{
  std::string result = "";
  Display *display = XOpenDisplay(nullptr);

  if (display == nullptr)
  {
    return result;
  }

  Atom clipboard = XInternAtom(display, "CLIPBOARD", False);

  if (clipboard == None)
  {
    XCloseDisplay(display);
    return result;
  }

  Atom utf8 = XInternAtom(display, "UTF8_STRING", False);

  if (utf8 == None)
  {
    XCloseDisplay(display);
    return result;
  }

  Window owner = XGetSelectionOwner(display, clipboard);

  if (owner == None)
  {
    XCloseDisplay(display);
    return result;
  }

  Atom actual_type;
  int actual_format;
  unsigned long nitems, bytes_after;
  unsigned char *data;
  int status = XGetWindowProperty(display, owner, clipboard, 0, LONG_MAX, False, utf8, &actual_type, &actual_format, &nitems, &bytes_after, &data);

  if (status == Success && actual_type == utf8 && actual_format == 8 && nitems > 0)
  {
    result = std::string(reinterpret_cast<const char *>(data), nitems);
  }

  if (data != nullptr)
  {
    XFree(data);
  }

  XCloseDisplay(display);
  return result;
}

void Write(const std::string &data)
{
  Display *display = XOpenDisplay(nullptr);

  if (display == nullptr)
  {
    return;
  }

  Atom clipboard = XInternAtom(display, "CLIPBOARD", False);

  if (clipboard == None)
  {
    XCloseDisplay(display);
    return;
  }

  XSetSelectionOwner(display, clipboard, None, CurrentTime);
  Atom utf8 = XInternAtom(display, "UTF8_STRING", False);

  if (utf8 == None)
  {
    XCloseDisplay(display);
    return;
  }

  Window owner = XGetSelectionOwner(display, clipboard);

  if (owner != None && owner != Xmu::Atm::None)
  {
    XConvertSelection(display, clipboard, utf8, utf8, owner, CurrentTime);
  }

  Atom targets = XInternAtom(display, "TARGETS", False);

  if (targets == None)
  {
    XCloseDisplay(display);
    return;
  }

  XEvent event;
  memset(&event, 0, sizeof(event));
  event.xselection.type = SelectionRequest;
  event.xselection.display = display;
  event.xselection.owner = owner;
  event.xselection.requestor = owner;
  event.xselection.selection = clipboard;
  event.xselection.target = targets;
  event.xselection.time = CurrentTime;

  XSendEvent(display, owner, False, NoEventMask, &event);
  XCloseDisplay(display);
}

std::vector<std::string> ReadFiles()
{
  std::vector<std::string> file_paths;
  // TODO
  return file_paths;
}

void WriteFiles(const std::vector<std::string> &file_paths)
{
  // TODO
}

void Clear()
{
  // TODO
}
