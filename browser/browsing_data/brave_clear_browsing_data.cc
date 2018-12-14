/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this file,
* You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/browsing_data/brave_clear_browsing_data.h"
#include "chrome/browser/browsing_data/chrome_browsing_data_remover_delegate.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_list.h"
#include "chrome/common/pref_names.h"
#include "components/prefs/pref_service.h"
#include "components/browsing_data/core/pref_names.h"

namespace {

// Gets the count of sessions with the given |profile|. Off-the-record sessions
// aren't counted as they clean up after themselves.
bool IsLastActiveSessionForProfile(Profile* profile) {
  BrowserList* list = BrowserList::GetInstance();
  int count =
      std::count_if(list->begin(), list->end(), [profile](Browser* browser) {
        return !browser->profile()->IsOffTheRecord() &&
               browser->profile()->IsSameProfile(profile);
      });
  return (count == 0);
}

bool GetClearBrowsingDataOnExitSettings(const Profile* profile,
                                        int& remove_mask,
                                        int& origin_mask) {
  const PrefService* prefs = profile->GetPrefs();
  remove_mask = 0;
  origin_mask = 0;

  int site_data_mask = ChromeBrowsingDataRemoverDelegate::DATA_TYPE_SITE_DATA;
  // Don't try to clear LSO data if it's not supported.
  if (!prefs->GetBoolean(prefs::kClearPluginLSODataEnabled))
    site_data_mask &= ~ChromeBrowsingDataRemoverDelegate::DATA_TYPE_PLUGIN_DATA;

  if (prefs->GetBoolean(browsing_data::prefs::kDeleteBrowsingHistoryOnExit) &&
      prefs->GetBoolean(prefs::kAllowDeletingBrowserHistory))
    remove_mask |= ChromeBrowsingDataRemoverDelegate::DATA_TYPE_HISTORY;

  if (prefs->GetBoolean(browsing_data::prefs::kDeleteDownloadHistoryOnExit) &&
      prefs->GetBoolean(prefs::kAllowDeletingBrowserHistory))
    remove_mask |= content::BrowsingDataRemover::DATA_TYPE_DOWNLOADS;

  if (prefs->GetBoolean(browsing_data::prefs::kDeleteCacheOnExit))
    remove_mask |= content::BrowsingDataRemover::DATA_TYPE_CACHE;

  if (prefs->GetBoolean(browsing_data::prefs::kDeleteCookiesOnExit)) {
    remove_mask |= site_data_mask;
    origin_mask |= content::BrowsingDataRemover::ORIGIN_TYPE_UNPROTECTED_WEB;
  }

  if (prefs->GetBoolean(browsing_data::prefs::kDeletePasswordsOnExit))
    remove_mask |= ChromeBrowsingDataRemoverDelegate::DATA_TYPE_PASSWORDS;

  if (prefs->GetBoolean(browsing_data::prefs::kDeleteFormDataOnExit))
    remove_mask |= ChromeBrowsingDataRemoverDelegate::DATA_TYPE_FORM_DATA;

  if (prefs->GetBoolean(browsing_data::prefs::kDeleteHostedAppsDataOnExit)) {
    remove_mask |= site_data_mask;
    origin_mask |= content::BrowsingDataRemover::ORIGIN_TYPE_PROTECTED_WEB;
  }

  if (prefs->GetBoolean(browsing_data::prefs::kDeleteMediaLicensesOnExit))
    remove_mask |= content::BrowsingDataRemover::DATA_TYPE_MEDIA_LICENSES;

  if (prefs->GetBoolean(browsing_data::prefs::kDeleteSiteSettingsOnExit))
    remove_mask |=
      ChromeBrowsingDataRemoverDelegate::DATA_TYPE_CONTENT_SETTINGS;

  return (remove_mask != 0);
}

} // namespace

namespace content {

BraveClearBrowsingData::OnExitTestingCallback*
    BraveClearBrowsingData::on_exit_testing_callback_ = nullptr;

//static
void BraveClearBrowsingData::ClearOnExit(Profile* profile) {
  DCHECK(profile);

  // Off-the-record profiles clean up after themselves.
  if (profile->IsOffTheRecord())
    return;

  // Check if any settings to clear data on exit have been turned on.
  int remove_mask = 0;
  int origin_mask = 0;
  if (!GetClearBrowsingDataOnExitSettings(profile, remove_mask, origin_mask))
    return;

  // Check if this is the last browser for this profile.
  if (!IsLastActiveSessionForProfile(profile))
    return;

  // Get data remover for this profile.
  content::BrowsingDataRemover* remover =
    content::BrowserContext::GetBrowsingDataRemover(profile);

  // If testing, let the test decide if it will do the actual removal and wait.
  if (!on_exit_testing_callback_ ||
      !on_exit_testing_callback_->BeforeClearOnExitRemoveData(
          remover, remove_mask, origin_mask))
    remover->Remove(base::Time(), base::Time::Max(), remove_mask, origin_mask);
}

//static
void BraveClearBrowsingData::SetOnExitTestingCallback(
    OnExitTestingCallback* callback) {
  on_exit_testing_callback_ = callback;
}

} // namespace content

