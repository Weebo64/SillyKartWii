#ifndef _DWC_NHTTP_WRAPPER_
#define _DWC_NHTTP_WRAPPER_

#include <types.hpp>
#include <core/rvl/NHTTP/NHTTP.hpp>

// Wrapper for NHTTP functions to be used without namespace
// These map to the NHTTP namespace functions

inline s32 NHTTPStartup(void* alloc, void* free, u32 param_3) {
    return NHTTP::Startup(alloc, free, param_3);
}

inline void* NHTTPCreateRequest(
    const char* url, int param_2, void* buffer, u32 length, void* callback,
    void* userdata) {
    return NHTTP::CreateRequest(url, param_2, buffer, length, callback, userdata);
}

inline s32 NHTTPSendRequestAsync(void* request) {
    return NHTTP::SendRequestAsync(request);
}

inline s32 NHTTPDestroyResponse(void* response) {
    return NHTTP::DestroyResponse(response);
}

#endif
