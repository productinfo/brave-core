/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import { debounce } from '../../../common/debounce'

const keyName = 'rewards-internals-data'

const defaultState: RewardsInternals.State = {
  info: {
    keyInfoSeed: '',
    walletPaymentId: ''
  }
}

export const getLoadTimeData = (state: RewardsInternals.State): RewardsInternals.State => {
  state = { ...state }
  state.info = defaultState.info

  // Expected to be Strings
  ;['keyInfoSeed','walletPaymentId'].forEach((item) => {
    state.info[item] = chrome.getVariableValue(item)
  })

  return state
}

export const cleanData = (state: RewardsInternals.State): RewardsInternals.State => {
  return getLoadTimeData(state)
}

export const load = (): RewardsInternals.State => {
  const data = window.localStorage.getItem(keyName)
  let state: RewardsInternals.State = defaultState
  if (data) {
    try {
      state = JSON.parse(data)
    } catch (e) {
      console.error('Could not parse local storage data: ', e)
    }
  }
  return cleanData(state)
}

export const debouncedSave = debounce((data: RewardsInternals.State) => {
  if (data) {
    window.localStorage.setItem(keyName, JSON.stringify(cleanData(data)))
  }
}, 50)
