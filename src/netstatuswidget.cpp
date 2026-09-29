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
#include <QVBoxLayout>

Q_LOGGING_CATEGORY(LC_NETSTATUS_WIDGET, "netstatus.widget")

NetStatusWidget::NetStatusWidget(NetStatusSettings *settings, QWidget *parent):
    QWidget(parent),
    m_settings(settings),
    m_iconLabel(new QLabel(this)),
    m_speedLabel(new QLabel(this)),
    m_lastUpload(0),
    m_lastDownload(0),
    m_lastTime(QDateTime::currentMSecsSinceEpoch())
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(2, 0, 2, 0);
    layout->setSpacing(0);
    
    // Fix icon size so it doesn't stretch
    m_iconLabel->setFixedSize(32, 32);
    m_iconLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_iconLabel);
    
    m_speedLabel->setVisible(m_settings->showSpeed());
    m_speedLabel->setAlignment(Qt::AlignCenter);
    m_speedLabel->setStyleSheet("font-size: 10px;");
    layout->addWidget(m_speedLabel);
    
    setMaximumSize(48, 48);
    
    // Start speed calculation timer
    m_speedTimer = startTimer(1000);
}

void NetStatusWidget::updateStatus()
{
    NetworkManager::Connectivity connectivity = NetworkManager::connectivity();
    
    m_iconLabel->setPixmap(getConnectivityIcon().pixmap(32, 32));
}

void NetStatusWidget::setShowSpeed(bool show)
{
    m_speedLabel->setVisible(show);
}

void NetStatusWidget::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_speedTimer)
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
