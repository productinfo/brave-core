/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/renderer_host/brave_navigation_ui_data.h"
#include "brave/common/webui_url_constants.h"

#define ChromeNavigationUIData BraveNavigationUIData
#define Navigate Navigate_ChromiumImpl
#include "../../../../chrome/browser/ui/browser_navigator.cc"
#undef ChromeNavigationUIData
#undef Navigate

void Navigate(NavigateParams* params) {
  GURL& url = params->url;
  if (url.is_valid() && url.SchemeIs(content::kBraveUIScheme)) {
    GURL::Replacements replacements;
    replacements.SetSchemeStr(content::kChromeUIScheme);
    url = url.ReplaceComponents(replacements);
  }

   Navigate_ChromiumImpl(params);
}
