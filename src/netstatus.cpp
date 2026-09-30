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

#include "netstatus.h"
#include "netstatussettingsdialog.h"

#include <QLoggingCategory>

Q_LOGGING_CATEGORY(LC_NETSTATUS, "netstatus")

NetStatus::NetStatus(const ILXQtPanelPluginStartupInfo &startupInfo) :
    QObject(),
    ILXQtPanelPlugin(startupInfo),
    m_widget(&m_settings)
{
    m_settings.init(settings());

    // Connect to NetworkManager notifier signals
    connect(NetworkManager::notifier(),
            &NetworkManager::Notifier::statusChanged,
            this,
            &NetStatus::onNetworkManagerStatusChanged);

    connect(NetworkManager::notifier(),
            &NetworkManager::Notifier::connectivityChanged,
            this,
            &NetStatus::onConnectivityChanged);

    // Update initial status
    updateWidgetFromNetworkManager();
}

NetStatus::~NetStatus()
{
}

QDialog *NetStatus::configureDialog()
{
    return new NetStatusSettingsDialog(&m_settings, nullptr);
}

void NetStatus::realign()
{
}

void NetStatus::settingsChanged()
{
    m_widget.setShowSpeed(m_settings.showSpeed());
    m_widget.setUpdateInterval(m_settings.updateInterval());
    m_widget.setFontSize(m_settings.fontSize());
    updateWidgetFromNetworkManager();
}

void NetStatus::activated(ActivationReason reason)
{
    if (reason == Trigger)
    {
        // Left click - refresh status
        updateWidgetFromNetworkManager();
    }
}

void NetStatus::onNetworkManagerStatusChanged(NetworkManager::Status status)
{
    qCDebug(LC_NETSTATUS) << "NetworkManager status changed:" << status;
    updateWidgetFromNetworkManager();
}

void NetStatus::onConnectivityChanged(NetworkManager::Connectivity connectivity)
{
    qCDebug(LC_NETSTATUS) << "Connectivity changed:" << connectivity;
    updateWidgetFromNetworkManager();
}

void NetStatus::updateWidgetFromNetworkManager()
{
    m_widget.updateStatus();
}
