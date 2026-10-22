// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_0B6320.CPP: filling a buffer with the next bytes of an upload (built
   with SSE, apart from its neighbours in unknown_0b66c0.cpp) */

#include "transaction_storage.h"
#include "unknown_11c920.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <xtl.h>
#include <stdlib.h>
#include "unknown_0b4da0.h"

struct s_upload_file_ab
{
    byte unknown00[0x108];
    HANDLE handle;
    dword position;
};

void function_b65f0(s_upload_ab *upload);

static inline bool upload_read_ab(s_upload_file_ab *file, long position, void *buffer, long count)
{
    if (file->position != position)
    {
        file->position = SetFilePointer(file->handle, position, 0, FILE_BEGIN);
        if (file->position == 0xffffffff)
            return false;
    }
    dword received;
    bool result = false;
    if (ReadFile(file->handle, buffer, count, &received, 0))
    {
        if (received == count)
            result = true;
        else
            SetLastError(0x26);
    }
    file->position += received;
    return result;
}

// @retail 0xb6320
bool function_b6320(s_upload_ab *upload, byte *buffer, long maximum, long *written)
{
    s_upload_ab &state = *upload;
    bool result = false;
    byte *out = buffer;
    long remaining = maximum;
    bool valid = true;
    if (state.kind != 2 && state.kind != 1)
        valid = false;
    char const *path = state.path;
    unsigned long length = 0;
    for (; length < 0x7f; ++length)
        if (!*path++) break;
    if (length && valid)
    {
        if (!state.position)
            function_b65f0(&state);
        result = true;
        while (state.position != state.ending_length + state.file_length + state.part_length + state.header_length && remaining > 0)
        {
            long position = state.position;
            long count;
            if (position < state.header_length)
            {
                count = remaining > state.header_length - position ? state.header_length - position : remaining;
                memcpy(out, state.header + position, count);
            }
            else if ((position -= state.header_length) < state.part_length)
            {
                count = remaining > state.part_length - position ? state.part_length - position : remaining;
                memcpy(out, state.part + position, count);
            }
            else if ((position -= state.part_length) < state.file_length)
            {
                count = remaining > state.file_length - position ? state.file_length - position : remaining;
                if (state.kind == 1)
                    memcpy(out, state.memory + position, count);
                else
                {
                    s_upload_file_ab *file = state.file;
                    bool success = true;
                    if (file->position != position)
                    {
                        file->position = SetFilePointer(file->handle, position, 0, FILE_BEGIN);
                        success = file->position != 0xffffffff;
                    }
                    if (success)
                    {
                        dword received;
                        bool all_received = false;
                        if (ReadFile(file->handle, out, count, &received, 0))
                        {
                            if (received == count) all_received = true;
                            else SetLastError(0x26);
                        }
                        file->position += received;
                        success = all_received;
                    }
                    if (!success)
                    {
                        GetLastError();
                        SetLastError(0);
                        result = false;
                    }
                }
            }
            else if ((position -= state.file_length) < state.ending_length)
            {
                count = remaining > state.ending_length - position ? state.ending_length - position : remaining;
                memcpy(out, state.ending + position, count);
            }
            else
            {
                result = false;
                break;
            }
            out += count;
            state.position += count;
            remaining = maximum - (out - buffer);
        }
        *written = out - buffer;
    }
    return result;
}
