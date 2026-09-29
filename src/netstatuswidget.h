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

#ifndef NETSTATUSWIDGET_H
#define NETSTATUSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <networkmanagerqt/manager.h>
#include <networkmanagerqt/device.h>
#include <networkmanagerqt/activeconnection.h>

class NetStatusSettings;

/**
 * \brief Widget displayed on the LXQt panel.
 *
 * Shows network connectivity icon and optional upload/download speeds.
 */
class NetStatusWidget: public QWidget
{
    Q_OBJECT

public:
    explicit NetStatusWidget(NetStatusSettings *settings, QWidget *parent = nullptr);

    /// Update widget with current network status
    void updateStatus();

signals:
    void activated();

private Q_SLOTS:
    void updateSpeed();

private:
    QIcon getConnectivityIcon() const;
    QString getConnectionName() const;
    void getSpeeds(qulonglong &upload, qulonglong &download) const;

    NetStatusSettings *m_settings;
    QLabel *m_iconLabel;
    QLabel *m_speedLabel;
    QTimer *m_speedTimer;
    qulonglong m_lastUpload;
    qulonglong m_lastDownload;
    qint64 m_lastTime;
};

#endif // NETSTATUSWIDGET_H
