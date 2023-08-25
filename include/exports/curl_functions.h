#pragma once

#include <code/tcpgecko.h>

// Based on PureClient
enum cUrlOPT : int {
    WRITEDATA = 0x2711, URL = 0x2712, WRITEFUNCTION = 0x4e2b, 
};

EXPORT_DECL(void*, curl_easy_init, void);
EXPORT_DECL(void, curl_easy_cleanup, void*);
EXPORT_DECL(int, curl_easy_perform, void*);
EXPORT_DECL(void, curl_easy_setopt, void*, cUrlOPT, const void*);

void InitLibCurlFunctionPointers(void) {
    unsigned int *funcPointer = 0;
    uint32_t libcurl_handle;
    OSDynLoad_Acquire("nlibcurl.rpl", &libcurl_handle);
    OS_FIND_EXPORT(libcurl_handle, curl_easy_init);
    OS_FIND_EXPORT(libcurl_handle, curl_easy_cleanup);
    OS_FIND_EXPORT(libcurl_handle, curl_easy_perform);
    OS_FIND_EXPORT(libcurl_handle, curl_easy_setopt);
}