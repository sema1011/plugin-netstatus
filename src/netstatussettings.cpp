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

#include "netstatussettings.h"
#include <lxqt/pluginsettings.h>

/**
 * \brief Internal adapter that wraps PluginSettings as SettingsStorage.
 */
class PluginSettingsAdapter : public SettingsStorage
{
public:
    explicit PluginSettingsAdapter(PluginSettings *settings) : m_settings(settings) {}
    
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const override
    {
        if (!m_settings)
            return defaultValue;
        return m_settings->value(key, defaultValue);
    }

    void setValue(const QString &key, const QVariant &value) override
    {
        if (m_settings)
            m_settings->setValue(key, value);
    }

private:
    PluginSettings *m_settings{nullptr};
};

NetStatusSettings::NetStatusSettings()
{
}

NetStatusSettings::~NetStatusSettings()
{
    delete m_settings;
}

void NetStatusSettings::init(PluginSettings *settings)
{
    // Create adapter that wraps PluginSettings
    m_settings = new PluginSettingsAdapter(settings);
}

bool NetStatusSettings::showSpeed() const
{
    if (!m_settings)
        return true;
    return m_settings->value("Display/showSpeed", true).toBool();
}

bool NetStatusSettings::showTooltip() const
{
    if (!m_settings)
        return true;
    return m_settings->value("Display/showTooltip", true).toBool();
}

int NetStatusSettings::updateInterval() const
{
    if (!m_settings)
        return 1000;
    return m_settings->value("Display/updateInterval", 1000).toInt();
}

int NetStatusSettings::fontSize() const
{
    if (!m_settings)
        return 10;
    return m_settings->value("Display/fontSize", 10).toInt();
}

void NetStatusSettings::setShowSpeed(bool show)
{
    if (m_settings)
        m_settings->setValue("Display/showSpeed", show);
}

void NetStatusSettings::setShowTooltip(bool show)
{
    if (m_settings)
        m_settings->setValue("Display/showTooltip", show);
}

void NetStatusSettings::setUpdateInterval(int interval)
{
    if (m_settings)
        m_settings->setValue("Display/updateInterval", interval);
}

void NetStatusSettings::setFontSize(int size)
{
    if (m_settings)
        m_settings->setValue("Display/fontSize", size);
}
