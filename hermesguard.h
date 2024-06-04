#include <Windows.h>

typedef unsigned long long u64;
typedef unsigned long u32;

// make sure to change HERMES_SPACE and HERMES_KEY to something else.
#define HERMES_ADD(name, type) u64 HERMES_OP_##name = (u64)&name; using HERMES_##name = type; HERMES_##name name; name = reinterpret_cast< type > (hermes::add_function(HERMES_OP_##name));
#define HERMES_SPACE 0x13376900
#define HERMES_TABLE_MAX 0xFE
#define HERMES_KEY 0x13376969

namespace hermes
{
    u64 hidden_function_table[HERMES_TABLE_MAX] = {};

    __forceinline u64 add_function(u64 fptr)
    {
        for (u64 i = 0; i < HERMES_TABLE_MAX; i++)
        {
            u64 p = hidden_function_table[i];
            if (p == 0)
            {
                hidden_function_table[i] = fptr ^ HERMES_KEY;
                return HERMES_SPACE + i;
            }
        }
    }

    __forceinline LONG WINAPI __hermes_handler(EXCEPTION_POINTERS* exception_info)
    {
        u64 access_type = exception_info->ExceptionRecord->ExceptionInformation[0];
        u64 access_address = exception_info->ExceptionRecord->ExceptionInformation[1];

        if (access_type == 8)
        {
            if (access_address >= HERMES_SPACE && access_address < HERMES_SPACE + (HERMES_TABLE_MAX * 8))
            {
                u64 table_index = access_address - HERMES_SPACE;
                exception_info->ContextRecord->Rip = hidden_function_table[table_index] ^ HERMES_KEY;
                return EXCEPTION_CONTINUE_EXECUTION;
            }
        }

        return EXCEPTION_EXECUTE_HANDLER;
    }

    // has to be called
    __forceinline void initialize()
    {
        AddVectoredExceptionHandler(1, __hermes_handler);
    }
}
