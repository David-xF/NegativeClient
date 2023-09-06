#pragma once

#include <code/tcpgecko.h>

// OLD OPTIONS
// enum cUrlOPT : int {
//     WRITEDATA = 0x2711, URL = 0x2712, WRITEFUNCTION = 0x4e2b, 
// };

#define CURLOPTTYPE_LONG          0
#define CURLOPTTYPE_OBJECTPOINT   10000
#define CURLOPTTYPE_FUNCTIONPOINT 20000
#define CURLOPTTYPE_OFF_T         30000

#define CINIT(name, type, number) CURLOPT_##name = CURLOPTTYPE_##type + number

enum CURLOption {
    CINIT(FILE,          OBJECTPOINT,    1),
    CINIT(URL,           OBJECTPOINT,    2),
    CINIT(PORT,          LONG,           3),
    CINIT(WRITEFUNCTION, FUNCTIONPOINT, 11),
    CINIT(READFUNCTION,  FUNCTIONPOINT, 12),
};

EXPORT_DECL(void*, curl_easy_init, void);
EXPORT_DECL(void, curl_easy_cleanup, void*);
EXPORT_DECL(int, curl_easy_perform, void*);
EXPORT_DECL(void, curl_easy_setopt, void*, CURLOption, const void*);

void InitLibCurlFunctionPointers(void) {
    unsigned int* funcPointer = 0;
    uint32_t libcurl_handle;
    OSDynLoad_Acquire("nlibcurl.rpl", &libcurl_handle);
    OS_FIND_EXPORT(libcurl_handle, curl_easy_init);
    OS_FIND_EXPORT(libcurl_handle, curl_easy_cleanup);
    OS_FIND_EXPORT(libcurl_handle, curl_easy_perform);
    OS_FIND_EXPORT(libcurl_handle, curl_easy_setopt);
}