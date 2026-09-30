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

#ifndef NETSTATUSSETTINGS_DIALOG_H
#define NETSTATUSSETTINGS_DIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QAbstractButton>

class NetStatusSettings;

class NetStatusSettingsDialog: public QDialog
{
    Q_OBJECT

public:
    explicit NetStatusSettingsDialog(NetStatusSettings *settings, QWidget *parent = nullptr);

private Q_SLOTS:
    void onApply();

private:
    void setupConnections();

    NetStatusSettings *m_settings;
    bool m_modified;
    
    // UI widgets
    QCheckBox *m_showSpeedCheckBox;
    QCheckBox *m_showTooltipCheckBox;
    QSpinBox *m_intervalSpinBox;
    QSpinBox *m_fontSizeSpinBox;
    QDialogButtonBox *buttonBox;
};

#endif // NETSTATUSSETTINGS_DIALOG_H
