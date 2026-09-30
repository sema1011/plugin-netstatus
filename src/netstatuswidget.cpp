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

#include <QLoggingCategory>
#include <QIcon>
#include <QDateTime>
#include <QLabel>
#include <QHBoxLayout>
#include <networkmanagerqt/device.h>
#include <networkmanagerqt/devicestatistics.h>
#include <networkmanagerqt/activeconnection.h>

Q_LOGGING_CATEGORY(LC_NETSTATUS_WIDGET, "netstatus.widget")

NetStatusWidget::NetStatusWidget(NetStatusSettings *settings, QWidget *parent):
    QWidget(parent),
    m_settings(settings),
    m_iconLabel(new QLabel(this)),
    m_speedLabel(new QLabel(this)),
    m_lastUpload(0),
    m_lastDownload(0),
    m_lastTime(QDateTime::currentMSecsSinceEpoch()),
    m_currentInterval(settings->updateInterval())
{
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(2, 0, 2, 0);
    layout->setSpacing(2);
    
    // Fix icon size so it doesn't stretch
    m_iconLabel->setFixedSize(32, 32);
    layout->addWidget(m_iconLabel, 0, Qt::AlignVCenter);
    
    m_speedLabel->setVisible(m_settings->showSpeed());
    m_speedLabel->setAlignment(Qt::AlignVCenter);
    m_speedLabel->setStyleSheet(QString("font-size: %1px;").arg(m_settings->fontSize()));
    m_speedLabel->setObjectName("speedLabel");
    layout->addWidget(m_speedLabel, 0, Qt::AlignVCenter);
    
    setMaximumSize(200, 48);
    
    // Initialize device statistics
    initializeStatistics();
    
    // Start speed calculation timer with configurable interval
    m_speedTimer = startTimer(m_currentInterval);
}

void NetStatusWidget::updateStatus()
{
    NetworkManager::Connectivity connectivity = NetworkManager::connectivity();
    
    m_iconLabel->setPixmap(getConnectivityIcon().pixmap(32, 32));
    
    // Set tooltip with connection details
    if (m_settings->showTooltip())
    {
        QString connName = activeConnectionName();
        QString ifaceName = activeInterfaceName();
        
        QString tooltip;
        switch (connectivity)
        {
            case NetworkManager::Connectivity::Full:
                tooltip = tr("Connected");
                break;
            case NetworkManager::Connectivity::Limited:
                tooltip = tr("Limited");
                break;
            case NetworkManager::Connectivity::Portal:
                tooltip = tr("Portal");
                break;
            default:
                tooltip = tr("Disconnected");
                break;
        }
        
        if (!connName.isEmpty())
            tooltip += "\n" + connName;
        if (!ifaceName.isEmpty())
            tooltip += "\n" + ifaceName;
        
        setToolTip(tooltip);
    }
    else
    {
        setToolTip({});
    }
}

void NetStatusWidget::setShowSpeed(bool show)
{
    m_speedLabel->setVisible(show);
}

void NetStatusWidget::setUpdateInterval(int interval)
{
    if (interval != m_currentInterval && interval > 0)
    {
        if (m_speedTimer)
            killTimer(m_speedTimer);
        m_currentInterval = interval;
        m_speedTimer = startTimer(m_currentInterval);
    }
}

void NetStatusWidget::setFontSize(int size)
{
    if (size > 0)
        m_speedLabel->setStyleSheet(QString("font-size: %1px;").arg(size));
}

void NetStatusWidget::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_speedTimer && m_speedLabel->isVisible())
    {
        qulonglong upload = 0;
        qulonglong download = 0;
        getSpeeds(upload, download);
        
        qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
        qint64 timeDelta = currentTime - m_lastTime;
        
        if (timeDelta > 0 && m_lastTime > 0)
        {
            qreal uploadSpeed = (upload - m_lastUpload) * 1000.0 / timeDelta;
            qreal downloadSpeed = (download - m_lastDownload) * 1000.0 / timeDelta;
            
            if (uploadSpeed > 0 || downloadSpeed > 0)
            {
                m_speedLabel->setText(QString("%1↑ %2↓")
                    .arg(downloadSpeed > 1024 ? 
                         QString::number(downloadSpeed / 1024, 'f', 1) + "K" : 
                         QString::number(downloadSpeed, 'f', 0) + "B")
                    .arg(uploadSpeed > 1024 ? 
                         QString::number(uploadSpeed / 1024, 'f', 1) + "K" : 
                         QString::number(uploadSpeed, 'f', 0) + "B"));
            }
        }
        
        m_lastUpload = upload;
        m_lastDownload = download;
        m_lastTime = currentTime;
    }
    
    QWidget::timerEvent(event);
}

void NetStatusWidget::getSpeeds(qulonglong &upload, qulonglong &download) const
{
    upload = 0;
    download = 0;

    if (m_statistics)
    {
        upload += m_statistics->txBytes();
        download += m_statistics->rxBytes();
    }
}

void NetStatusWidget::initializeStatistics()
{
    if (m_statsInitialized)
        return;

    // Prefer wired/wireless devices over loopback
    const auto devices = NetworkManager::networkInterfaces();

    // First pass: look for active wired or wireless devices
    for (const QSharedPointer<NetworkManager::Device> &device : devices)
    {
        if (!device->managed() || device->state() != NetworkManager::Device::State::Activated)
            continue;

        const QString iface = device->interfaceName();
        if (iface == "lo")
            continue;

        auto stats = device->deviceStatistics();
        if (stats)
        {
            stats->setRefreshRateMs(1000);
            m_statistics = stats;
            m_statsInitialized = true;
            qCDebug(LC_NETSTATUS_WIDGET) << "Using statistics for device:" << iface;
            return;
        }
    }

    // Second pass: fall back to any activated device with statistics
    for (const QSharedPointer<NetworkManager::Device> &device : devices)
    {
        if (!device->managed() || device->state() != NetworkManager::Device::State::Activated)
            continue;

        auto stats = device->deviceStatistics();
        if (stats)
        {
            stats->setRefreshRateMs(1000);
            m_statistics = stats;
            m_statsInitialized = true;
            qCDebug(LC_NETSTATUS_WIDGET) << "Using statistics for device:" << device->interfaceName();
            return;
        }
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

QString NetStatusWidget::activeConnectionName() const
{
    const auto connections = NetworkManager::activeConnections();
    for (const QSharedPointer<NetworkManager::ActiveConnection> &conn : connections)
    {
        QString id = conn->id();
        if (!id.isEmpty())
            return id;
    }
    return {};
}

QString NetStatusWidget::activeInterfaceName() const
{
    const auto connections = NetworkManager::activeConnections();
    for (const QSharedPointer<NetworkManager::ActiveConnection> &conn : connections)
    {
        const auto deviceList = conn->devices();
        if (!deviceList.isEmpty())
        {
            auto device = NetworkManager::findNetworkInterface(deviceList.first());
            if (device)
            {
                const QString iface = device->interfaceName();
                if (iface != "lo")
                    return iface;
            }
        }
    }
    return {};
}
