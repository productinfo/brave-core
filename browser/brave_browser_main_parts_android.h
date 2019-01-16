/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_BRAVE_BROWSER_MAIN_PARTS_ANDROID_H_
#define BRAVE_BROWSER_BRAVE_BROWSER_MAIN_PARTS_ANDROID_H_

#include "chrome/browser/chrome_browser_main_android.h"

class BraveBrowserMainPartsAndroid : public ChromeBrowserMainPartsAndroid {
 public:
  using ChromeBrowserMainPartsAndroid::ChromeBrowserMainPartsAndroid;
  ~BraveBrowserMainPartsAndroid() override = default;

 private:
  // ChromeBrowserMainPartsAndroid overrides:
  void PreShutdown() override;
};

#endif  // BRAVE_BROWSER_BRAVE_BROWSER_MAIN_PARTS_ANDROID_H_
