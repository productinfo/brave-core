// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

const customCurrentWebUINameMap = {
  extensions: 'settings',
  sync: 'settings',
}

Polymer({
  is: 'br-toolbar',

  properties: {
    // Name to display in the toolbar, in titlecase.
    pageName: String,

    // Prompt text to display in the search field.
    searchPrompt: String,

    // Tooltip to display on the clear search button.
    clearLabel: String,

    // Tooltip to display on the menu button.
    menuLabel: String,

    // Promotional toolstip string, shown in narrow mode if showMenuPromo is
    // true.
    menuPromo: String,

    // Value is proxied through to cr-toolbar-search-field. When true,
    // the search field will show a processing spinner.
    spinnerActive: Boolean,

    // Controls whether the menu button is shown at the start of the menu.
    showMenu: {type: Boolean, value: false},

    // Whether to show menu promo tooltip.
    showMenuPromo: {
      type: Boolean,
      value: false,
    },

    // Controls whether the search field is shown.
    showSearch: {type: Boolean, value: true},

    // True when the toolbar is displaying in narrow mode.
    narrow: {
      type: Boolean,
      reflectToAttribute: true,
      readonly: true,
      notify: true,
    },

    /**
     * The threshold at which the toolbar will change from normal to narrow
     * mode, in px.
     */
    narrowThreshold: {
      type: Number,
      value: 1,
    },

    closeMenuPromo: String,

    /** @private */
    showingSearch_: {
      type: Boolean,
      reflectToAttribute: true,
    },
  },

  observers: [
    'possiblyShowMenuPromo_(showMenu, showMenuPromo, showingSearch_)',
  ],

  /** @return {!CrToolbarSearchFieldElement} */
  getSearchField: function() {
    return this.$.search;
  },

  /** @private */
  onClosePromoTap_: function() {
    this.fire('cr-toolbar-menu-promo-close');
  },

  /** @private */
  onMenuTap_: function() {
    this.fire('cr-toolbar-menu-tap');
  },

  /** @private */
  possiblyShowMenuPromo_: function() {
    Polymer.RenderStatus.afterNextRender(this, function() {
      if (this.showMenu && this.showMenuPromo && !this.showingSearch_) {
        this.$$('#menuPromo')
            .animate(
                {
                  opacity: [0, .9],
                },
                /** @type {!KeyframeEffectOptions} */ ({
                  duration: 500,
                  fill: 'forwards'
                }));
        this.fire('cr-toolbar-menu-promo-shown');
      }
    }.bind(this));
  },

  /**
   * @param {string} title
   * @param {boolean} showMenuPromo
   * @return {string} The title if the menu promo isn't showing, else "".
   */
  titleIfNotShowMenuPromo_: function(title, showMenuPromo) {
    return showMenuPromo ? '' : title;
  },

  historyTitle: loadTimeData.getString('brToolbarHistoryTitle'),
  settingsTitle: loadTimeData.getString('brToolbarSettingsTitle'),
  bookmarksTitle: loadTimeData.getString('brToolbarBookmarksTitle'),
  downloadsTitle: loadTimeData.getString('brToolbarDownloadsTitle'),

  getNavItemSelectedClassName: function(itemName) {
    // which navigation item is the current page?
    let currentWebUIName = document.location.hostname
    // override name from hostname, if applicable
    if (customCurrentWebUINameMap[currentWebUIName])
      currentWebUIName = customCurrentWebUINameMap[currentWebUIName]
    // does it match the item calling this function?
    const itemWebUIName = itemName
    if (itemName === document.location.hostname)
      return '-selected'
    // not selected
    return ''
  }
});
