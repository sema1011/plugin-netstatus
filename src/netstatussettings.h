/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXQt - a lightweight, Qt based, desktop toolset
 * https://lxqt.org
 *
 * Copyright: 2026 LXQt team
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, see
 * <https://www.gnu.org/licenses/>.
 *
 * END_COMMON_COPYRIGHT_HEADER */

#ifndef NETSTATUSSETTINGS_H
#define NETSTATUSSETTINGS_H

#include <QString>
#include <QColor>

class PluginSettings;

/**
 * \brief Settings storage for the NetStatus plugin.
 *
 * Uses PluginSettings to persist user preferences across sessions.
 */
class NetStatusSettings
{
public:
    NetStatusSettings();
    ~NetStatusSettings();

    /**
     * \brief Initialize settings from PluginSettings.
     */
    void init(PluginSettings *settings);

    // Display options
    bool showSpeed() const;
    bool showTooltip() const;
    int updateInterval() const;

    // Setters
    void setShowSpeed(bool show);
    void setShowTooltip(bool show);
    void setUpdateInterval(int interval);

private:
    PluginSettings *m_settings{nullptr};
};

#endif // NETSTATUSSETTINGS_H
