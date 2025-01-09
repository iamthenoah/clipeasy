#include "cliPeasy.h"
#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <xcb/xcb_atom.h>
#include <sstream>
#include <iostream>

std::string Read()
{
    std::string result;
    xcb_connection_t *connection = xcb_connect(nullptr, nullptr);

    if (xcb_connection_has_error(connection))
    {
        return result;
    }
    xcb_intern_atom_cookie_t clipboard_cookie = xcb_intern_atom(connection, 0, 9, "CLIPBOARD");
    xcb_intern_atom_reply_t *clipboard_reply = xcb_intern_atom_reply(connection, clipboard_cookie, nullptr);

    if (!clipboard_reply)
    {
        xcb_disconnect(connection);
        return result;
    }
    xcb_atom_t clipboard_atom = clipboard_reply->atom;
    free(clipboard_reply);

    xcb_intern_atom_cookie_t utf8_string_cookie = xcb_intern_atom(connection, 0, 11, "UTF8_STRING");
    xcb_intern_atom_reply_t *utf8_string_reply = xcb_intern_atom_reply(connection, utf8_string_cookie, nullptr);

    if (!utf8_string_reply)
    {
        xcb_disconnect(connection);
        return result;
    }
    xcb_atom_t utf8_string_atom = utf8_string_reply->atom;
    free(utf8_string_reply);

    xcb_get_property_cookie_t property_cookie = xcb_get_property(connection, 0, xcb_get_setup(connection)->roots->root, clipboard_atom, utf8_string_atom, 0, -1);
    xcb_get_property_reply_t *property_reply = xcb_get_property_reply(connection, property_cookie, nullptr);

    if (property_reply)
    {
        int length = xcb_get_property_value_length(property_reply);
        char *value = (char *)xcb_get_property_value(property_reply);
        result = std::string(value, length);
        free(property_reply);
    }
    xcb_disconnect(connection);
    return result;
}

void Write(const std::string &data)
{
    xcb_connection_t *connection = xcb_connect(nullptr, nullptr);

    if (xcb_connection_has_error(connection))
    {
        return;
    }
    xcb_intern_atom_cookie_t clipboard_cookie = xcb_intern_atom(connection, 0, 9, "CLIPBOARD");
    xcb_intern_atom_reply_t *clipboard_reply = xcb_intern_atom_reply(connection, clipboard_cookie, nullptr);

    if (!clipboard_reply)
    {
        xcb_disconnect(connection);
        return;
    }
    xcb_atom_t clipboard_atom = clipboard_reply->atom;
    free(clipboard_reply);

    xcb_window_t window = xcb_generate_id(connection);
    xcb_create_window(connection, XCB_COPY_FROM_PARENT, window, xcb_get_setup(connection)->roots->root, 0, 0, 1, 1, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, XCB_COPY_FROM_PARENT, 0, nullptr);

    xcb_set_selection_owner(connection, window, clipboard_atom, XCB_CURRENT_TIME);
    xcb_flush(connection);

    xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window, clipboard_atom, XCB_ATOM_STRING, 8, data.size(), data.c_str());
    xcb_flush(connection);
    xcb_disconnect(connection);
}

std::vector<std::string> ReadFiles()
{
    std::vector<std::string> file_paths;
    xcb_connection_t *connection = xcb_connect(nullptr, nullptr);

    if (xcb_connection_has_error(connection))
    {
        return file_paths;
    }
    xcb_intern_atom_cookie_t clipboard_cookie = xcb_intern_atom(connection, 0, 9, "CLIPBOARD");
    xcb_intern_atom_reply_t *clipboard_reply = xcb_intern_atom_reply(connection, clipboard_cookie, nullptr);

    if (!clipboard_reply)
    {
        xcb_disconnect(connection);
        return file_paths;
    }
    xcb_atom_t clipboard_atom = clipboard_reply->atom;
    free(clipboard_reply);

    xcb_intern_atom_cookie_t targets_cookie = xcb_intern_atom(connection, 0, 7, "TARGETS");
    xcb_intern_atom_reply_t *targets_reply = xcb_intern_atom_reply(connection, targets_cookie, nullptr);

    if (!targets_reply)
    {
        xcb_disconnect(connection);
        return file_paths;
    }
    xcb_atom_t targets_atom = targets_reply->atom;
    free(targets_reply);

    xcb_convert_selection(connection, xcb_get_setup(connection)->roots->root, clipboard_atom, targets_atom, clipboard_atom, XCB_CURRENT_TIME);
    xcb_flush(connection);

    xcb_generic_event_t *event;
    while ((event = xcb_wait_for_event(connection)))
    {
        if ((event->response_type & ~0x80) == XCB_SELECTION_NOTIFY)
        {
            xcb_selection_notify_event_t *selection_notify = (xcb_selection_notify_event_t *)event;

            if (selection_notify->property == clipboard_atom)
            {
                xcb_get_property_cookie_t property_cookie = xcb_get_property(connection, 0, xcb_get_setup(connection)->roots->root, clipboard_atom, XCB_ATOM_STRING, 0, -1);
                xcb_get_property_reply_t *property_reply = xcb_get_property_reply(connection, property_cookie, nullptr);

                if (property_reply)
                {
                    int length = xcb_get_property_value_length(property_reply);
                    char *value = (char *)xcb_get_property_value(property_reply);

                    std::istringstream ss(std::string(value, length));
                    std::string line;
                    while (std::getline(ss, line))
                    {
                        if (line.rfind("file://", 0) == 0)
                        {
                            file_paths.push_back(line.substr(7));
                        }
                    }
                    free(property_reply);
                }
            }
        }
        free(event);
    }
    xcb_disconnect(connection);
    return file_paths;
}

void WriteFiles(const std::vector<std::string> &file_paths)
{
    xcb_connection_t *connection = xcb_connect(nullptr, nullptr);

    if (xcb_connection_has_error(connection))
    {
        return;
    }
    xcb_intern_atom_cookie_t clipboard_cookie = xcb_intern_atom(connection, 0, 9, "CLIPBOARD");
    xcb_intern_atom_reply_t *clipboard_reply = xcb_intern_atom_reply(connection, clipboard_cookie, nullptr);

    if (!clipboard_reply)
    {
        xcb_disconnect(connection);
        return;
    }
    xcb_atom_t clipboard_atom = clipboard_reply->atom;
    free(clipboard_reply);

    xcb_window_t window = xcb_generate_id(connection);
    xcb_create_window(connection, XCB_COPY_FROM_PARENT, window, xcb_get_setup(connection)->roots->root, 0, 0, 1, 1, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, XCB_COPY_FROM_PARENT, 0, nullptr);

    xcb_set_selection_owner(connection, window, clipboard_atom, XCB_CURRENT_TIME);
    xcb_flush(connection);

    std::ostringstream oss;
    oss << "copy\n";
    for (const auto &path : file_paths)
    {
        oss << "file://" << path << "\n";
    }
    std::string data = oss.str();

    xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window, clipboard_atom, XCB_ATOM_STRING, 8, data.size(), data.c_str());
    xcb_flush(connection);
    xcb_disconnect(connection);
}

void Clear()
{
    xcb_connection_t *connection = xcb_connect(nullptr, nullptr);

    if (xcb_connection_has_error(connection))
    {
        return;
    }
    xcb_intern_atom_cookie_t clipboard_cookie = xcb_intern_atom(connection, 0, 9, "CLIPBOARD");
    xcb_intern_atom_reply_t *clipboard_reply = xcb_intern_atom_reply(connection, clipboard_cookie, nullptr);

    if (!clipboard_reply)
    {
        xcb_disconnect(connection);
        return;
    }
    xcb_atom_t clipboard_atom = clipboard_reply->atom;
    free(clipboard_reply);

    xcb_window_t window = xcb_generate_id(connection);
    xcb_create_window(connection, XCB_COPY_FROM_PARENT, window, xcb_get_setup(connection)->roots->root, 0, 0, 1, 1, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, XCB_COPY_FROM_PARENT, 0, nullptr);

    xcb_set_selection_owner(connection, window, clipboard_atom, XCB_CURRENT_TIME);
    xcb_flush(connection);
    xcb_disconnect(connection);
}
