/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "build/build_config.h"  // For OS_MACOSX
#include "chrome/browser/search/search.h"
#include "content/public/browser/browser_url_handler.h"

#if defined(OS_WIN)
#include "brave/browser/brave_browser_main_parts_win.h"
#undef ChromeBrowserMainPartsWin
#define ChromeBrowserMainPartsWin BraveBrowserMainPartsWin
#elif defined(OS_MACOSX)
#include "brave/browser/brave_browser_main_parts_mac.h"
#undef ChromeBrowserMainPartsMac
#define ChromeBrowserMainPartsMac BraveBrowserMainPartsMac
#elif defined(OS_LINUX)
#include "brave/browser/brave_browser_main_parts_linux.h"
#undef ChromeBrowserMainPartsLinux
#define ChromeBrowserMainPartsLinux BraveBrowserMainPartsLinux
#elif defined(OS_ANDROID)
#include "brave/browser/brave_browser_main_parts_android.h"
#undef ChromeBrowserMainPartsAndroid
#define ChromeBrowserMainPartsAndroid BraveBrowserMainPartsAndroid
#elif defined(OS_POSIX)
#include "brave/browser/brave_browser_main_parts_posix.h"
#undef ChromeBrowserMainPartsPosix
#define ChromeBrowserMainPartsPosix BraveBrowserMainPartsPosix
#else
#include "brave/browser/brave_browser_main_parts.h"
#undef ChromeBrowserMainParts
#define ChromeBrowserMainParts BraveBrowserMainParts
#endif

#if defined(OS_LINUX)
#include "brave/browser/ui/views/brave_browser_main_extra_parts_views_linux.h"
#define ChromeBrowserMainExtraPartsViewsLinux BraveBrowserMainExtraPartsViewsLinux
#endif

#define HandleNewTabURLRewrite HandleNewTabURLRewrite_ChromiumImpl
#define HandleNewTabURLReverseRewrite HandleNewTabURLReverseRewrite_ChromiumImpl

namespace search {
  bool HandleNewTabURLRewrite(GURL* url, content::BrowserContext* bc) {
    return false;
  }
  bool HandleNewTabURLReverseRewrite(GURL* url, content::BrowserContext* bc) {
    return false;
  }
}  // namespace search

#include "../../../../chrome/browser/chrome_content_browser_client.cc"

#undef HandleNewTabURLRewrite
#undef HandleNewTabURLReverseRewrite_ChromiumImpl

#if defined(OS_LINUX)
#undef ChromeBrowserMainExtraPartsViewsLinux
#endif
