/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_REWARDS_BROWSER_REWARDS_INTERNALS_INFO_H_
#define BRAVE_COMPONENTS_BRAVE_REWARDS_BROWSER_REWARDS_INTERNALS_INFO_H_

#include <string>

namespace brave_rewards {

struct RewardsInternalsInfo {
  RewardsInternalsInfo();
  ~RewardsInternalsInfo();
  RewardsInternalsInfo(const RewardsInternalsInfo& info) = default;

  std::string payment_id;
  std::string key_info_seed;
};

}  // namespace brave_rewards

#endif //BRAVE_COMPONENTS_BRAVE_REWARDS_BROWSER_REWARDS_INTERNALS_INFO_H_
