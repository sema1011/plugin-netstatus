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

#ifndef NETSTATUS_H
#define NETSTATUS_H

#include <lxqt/ilxqtpanelplugin.h>
#include "netstatuswidget.h"
#include "netstatussettings.h"

#include <QTranslator>
#include <networkmanagerqt/manager.h>

class QDialog;

/**
 * \brief Main LXQt panel plugin for network status indicator.
 *
 * Displays network connection status and upload/download speeds
 * using NetworkManager-Qt.
 */
class NetStatus : public QObject, public ILXQtPanelPlugin
{
    Q_OBJECT
public:
    NetStatus(const ILXQtPanelPluginStartupInfo &startupInfo);
    ~NetStatus() override;

    virtual QString themeId() const override
    { return QStringLiteral("NetStatus"); }

    virtual ILXQtPanelPlugin::Flags flags() const override
    { return PreferRightAlignment | HaveConfigDialog; }

    virtual QWidget *widget() override
    { return &m_widget; }

    QDialog *configureDialog() override;
    virtual void realign() override;

protected slots:
    virtual void settingsChanged() override;
    void activated(ActivationReason reason) override;

private slots:
    void onNetworkManagerStatusChanged(NetworkManager::Status status);
    void onConnectivityChanged(NetworkManager::Connectivity connectivity);

private:
    void updateWidgetFromNetworkManager();

    NetStatusSettings m_settings;
    NetStatusWidget m_widget;
    QTranslator m_translator;
    QDialog *m_configDialog{nullptr};
};

#endif // NETSTATUS_H
