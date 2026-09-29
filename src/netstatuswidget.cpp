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

#include "netstatuswidget.h"
#include "netstatussettings.h"

#include <QMouseEvent>
#include <QToolTip>
#include <QLoggingCategory>
#include <QIcon>
#include <QDateTime>
#include <QLabel>
#include <QVBoxLayout>

Q_LOGGING_CATEGORY(LC_NETSTATUS_WIDGET, "netstatus.widget")

NetStatusWidget::NetStatusWidget(NetStatusSettings *settings, QWidget *parent):
    QWidget(parent),
    m_settings(settings),
    m_iconLabel(new QLabel(this)),
    m_speedLabel(new QLabel(this)),
    m_speedTimer(new QTimer(this)),
    m_lastTime(QDateTime::currentMSecsSinceEpoch())
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(2, 0, 2, 0);
    layout->setSpacing(0);
    layout->addWidget(m_iconLabel);
    layout->addWidget(m_speedLabel);
    
    m_speedLabel->setVisible(m_settings->showSpeed());
    
    setToolTip(tr("Network Status"));
    setMaximumSize(48, 48);
    
    // Setup speed update timer
    m_speedTimer->setInterval(m_settings->updateInterval());
    connect(m_speedTimer, &QTimer::timeout, this, &NetStatusWidget::updateSpeed);
    m_speedTimer->start();
}

void NetStatusWidget::updateStatus()
{
    NetworkManager::Connectivity connectivity = NetworkManager::connectivity();
    
    m_iconLabel->setPixmap(getConnectivityIcon().pixmap(32, 32));
    
    // Update tooltip with connection details
    QString tooltip = tr("Network: %1").arg(
        connectivity == NetworkManager::Connectivity::Full ? 
        tr("Connected") : 
        connectivity == NetworkManager::Connectivity::Limited ? 
        tr("Limited") : 
        connectivity == NetworkManager::Connectivity::Portal ? 
        tr("Portal") : 
        tr("Disconnected")
    );
    
    setToolTip(tooltip);
    
    qCDebug(LC_NETSTATUS_WIDGET) << "Status updated:" << connectivity;
}

void NetStatusWidget::setShowSpeed(bool show)
{
    m_speedLabel->setVisible(show);
}

void NetStatusWidget::updateSpeed()
{
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    qint64 timeDelta = currentTime - m_lastTime;
    
    if (timeDelta > 0)
    {
        m_lastTime = currentTime;
    }
}

QIcon NetStatusWidget::getConnectivityIcon() const
{
    NetworkManager::Connectivity connectivity = NetworkManager::connectivity();
    
    switch (connectivity)
    {
        case NetworkManager::Connectivity::Full:
            return QIcon::fromTheme("network-wireless-connected-symbolic");
        case NetworkManager::Connectivity::Limited:
            return QIcon::fromTheme("network-wireless-acquiring-symbolic");
        case NetworkManager::Connectivity::Portal:
            return QIcon::fromTheme("network-wireless-acquiring-symbolic");
        case NetworkManager::Connectivity::NoConnectivity:
        case NetworkManager::Connectivity::UnknownConnectivity:
            return QIcon::fromTheme("network-wireless-disconnected-symbolic");
        default:
            return QIcon::fromTheme("network-wireless-disconnected-symbolic");
    }
}
