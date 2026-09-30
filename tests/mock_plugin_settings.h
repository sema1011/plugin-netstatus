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

#ifndef MOCK_PLUGIN_SETTINGS_H
#define MOCK_PLUGIN_SETTINGS_H

#include <QSettings>
#include <QString>
#include <QVariant>

#include "netstatussettings.h"

/**
 * \brief Mock implementation of SettingsStorage for unit testing.
 * 
 * Uses QSettings internally to provide persistent-like storage
 * without requiring the LXQt panel infrastructure.
 */
class MockPluginSettings : public SettingsStorage
{
public:
    explicit MockPluginSettings(const QString &filePath)
        : m_settings(filePath, QSettings::IniFormat)
    {
    }

    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const override
    {
        return m_settings.value(key, defaultValue);
    }

    void setValue(const QString &key, const QVariant &value) override
    {
        m_settings.setValue(key, value);
    }

private:
    QSettings m_settings;
};

#endif // MOCK_PLUGIN_SETTINGS_H
