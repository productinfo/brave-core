/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/browsing_data/brave_clear_browsing_data.h"
#include "base/run_loop.h"
#include "base/scoped_observer.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/browsing_data/chrome_browsing_data_remover_delegate.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/common/pref_names.h"
#include "components/browsing_data/core/pref_names.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/browsing_data_remover.h"

namespace {

using content::BraveClearBrowsingData;

class BrowsingDataRemovalWatcher
    : public content::BrowsingDataRemover::Observer {
 public:
  BrowsingDataRemovalWatcher() : observer_(this) {}

  void ClearBrowsingDataForLoadedProfiles(
      BraveClearBrowsingData::OnExitTestingCallback* testing_callback);

  // BrowsingDataRemover::Observer implementation.
  void OnBrowsingDataRemoverDone() override;

 private:
  bool GetClearBrowsingDataOnExitSettings(const Profile* profile,
                                          int& remove_mask,
                                          int& origin_mask);
  void Wait();

  int num_profiles_to_clear_ = 0;
  base::RunLoop run_loop_;
  ScopedObserver<content::BrowsingDataRemover,
                 content::BrowsingDataRemover::Observer>
      observer_;
};

bool BrowsingDataRemovalWatcher::GetClearBrowsingDataOnExitSettings(
    const Profile* profile,
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

void BrowsingDataRemovalWatcher::ClearBrowsingDataForLoadedProfiles(
    BraveClearBrowsingData::OnExitTestingCallback* testing_callback) {
  ProfileManager* profile_manager = g_browser_process->profile_manager();
  DCHECK(profile_manager);

  std::vector<Profile*> profiles = profile_manager->GetLoadedProfiles();
  for (Profile* profile : profiles) {
    if (profile->IsOffTheRecord())
      continue;
    int remove_mask;
    int origin_mask;
    if (!GetClearBrowsingDataOnExitSettings(profile, remove_mask, origin_mask))
      continue;
    ++num_profiles_to_clear_;
    content::BrowsingDataRemover* remover =
        content::BrowserContext::GetBrowsingDataRemover(profile);
    observer_.Add(remover);
    if (testing_callback)
      testing_callback->BeforeClearOnExitRemoveData(remover, remove_mask,
                                                    origin_mask);
    remover->RemoveAndReply(base::Time(), base::Time::Max(), remove_mask,
                            origin_mask, this);
  }

  Wait();
}

void BrowsingDataRemovalWatcher::Wait() {
  if (num_profiles_to_clear_ > 0)
    run_loop_.Run();
}

void BrowsingDataRemovalWatcher::OnBrowsingDataRemoverDone() {
  --num_profiles_to_clear_;
  if (num_profiles_to_clear_ > 0)
    return;

  observer_.RemoveAll();
  run_loop_.Quit();
}

}  // namespace

namespace content {

BraveClearBrowsingData::OnExitTestingCallback*
    BraveClearBrowsingData::on_exit_testing_callback_ = nullptr;

// static
void BraveClearBrowsingData::ClearOnExit() {
  BrowsingDataRemovalWatcher watcher;
  watcher.ClearBrowsingDataForLoadedProfiles(on_exit_testing_callback_);
}

// static
void BraveClearBrowsingData::SetOnExitTestingCallback(
    OnExitTestingCallback* callback) {
  on_exit_testing_callback_ = callback;
}

}  // namespace content
