/*
 * Copyright (C) 2022-2023 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <gui/SurfaceComposerClient.h>
#include <utils/Errors.h>

using android::status_t;

extern "C" status_t _ZN7android21SurfaceComposerClient11Transaction5applyEbb(void* thisptr,
                                                                             bool synchronous,
                                                                             bool oneWay);

extern "C" status_t _ZN7android21SurfaceComposerClient11Transaction5applyEb(void* thisptr,
                                                                            bool synchronous) {
    return _ZN7android21SurfaceComposerClient11Transaction5applyEbb(thisptr, synchronous, false);
}

// OnePlus WFD blobs (e.g. libwfdmmsrc_system) call the pre-Android-17 signature
// SurfaceComposerClient::createVirtualDisplay(displayName, isSecure,
// optimizeForPower, uniqueId, requestedRefreshRate). Android 17 inserted a
// `uid_t ownerUid` parameter before requestedRefreshRate. Forward the old 5-arg
// symbol to the current 6-arg method, supplying the default owner uid.
// A plain C++ function with an asm() label is used (instead of extern "C") so the
// exported symbol carries the exact mangled name while still returning the
// non-POD android::sp<android::IBinder> via the normal C++ ABI.
__attribute__((visibility("default"))) android::sp<android::IBinder>
SurfaceComposerClient_createVirtualDisplay_compat(
        const std::string& displayName, bool isSecure, bool optimizeForPower,
        const std::string& uniqueId, float requestedRefreshRate)
        __asm__(
                "_ZN7android21SurfaceComposerClient20createVirtualDisplayERKNSt3__1"
                "12basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEbbS9_f");

android::sp<android::IBinder> SurfaceComposerClient_createVirtualDisplay_compat(
        const std::string& displayName, bool isSecure, bool optimizeForPower,
        const std::string& uniqueId, float requestedRefreshRate) {
    return android::SurfaceComposerClient::createVirtualDisplay(
            displayName, isSecure, optimizeForPower, uniqueId,
            android::gui::Uid::INVALID.val(), requestedRefreshRate);
}
