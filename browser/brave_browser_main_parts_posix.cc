/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/brave_browser_main_parts_posix.h"
#include "brave/browser/browsing_data/brave_clear_browsing_data.h"

void BraveBrowserMainPartsPosix::PreShutdown() {
  content::BraveClearBrowsingData::ClearOnExit();
  ChromeBrowserMainPartsPosix::PreShutdown();
}
