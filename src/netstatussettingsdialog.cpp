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

#include "netstatussettingsdialog.h"
#include "netstatussettings.h"

#include <QDialogButtonBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLoggingCategory>
#include <QPushButton>

Q_LOGGING_CATEGORY(LC_NETSTATUS_DIALOG, "netstatus.dialog")

NetStatusSettingsDialog::NetStatusSettingsDialog(NetStatusSettings *settings, QWidget *parent):
    QDialog(parent),
    m_settings(settings),
    m_modified(false)
{
    setWindowTitle(tr("NetStatus Settings"));
    setAttribute(Qt::WA_DeleteOnClose);

    // Create UI
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Display group
    QGroupBox *displayGroup = new QGroupBox(tr("Display"), this);
    QVBoxLayout *displayLayout = new QVBoxLayout(displayGroup);

    m_showSpeedCheckBox = new QCheckBox(tr("Show speed indicator"), this);
    m_showSpeedCheckBox->setChecked(m_settings->showSpeed());
    displayLayout->addWidget(m_showSpeedCheckBox);

    m_showTooltipCheckBox = new QCheckBox(tr("Show tooltip"), this);
    m_showTooltipCheckBox->setChecked(m_settings->showTooltip());
    displayLayout->addWidget(m_showTooltipCheckBox);

    layout->addWidget(displayGroup);

    // Update interval group
    QGroupBox *updateGroup = new QGroupBox(tr("Update Interval"), this);
    QVBoxLayout *updateLayout = new QVBoxLayout(updateGroup);

    m_intervalSpinBox = new QSpinBox(this);
    m_intervalSpinBox->setMinimum(100);
    m_intervalSpinBox->setMaximum(10000);
    m_intervalSpinBox->setSingleStep(100);
    m_intervalSpinBox->setSuffix(" ms");
    m_intervalSpinBox->setValue(m_settings->updateInterval());
    updateLayout->addWidget(m_intervalSpinBox);

    layout->addWidget(updateGroup);

    // Font size group
    QGroupBox *fontGroup = new QGroupBox(tr("Font Size"), this);
    QVBoxLayout *fontLayout = new QVBoxLayout(fontGroup);

    m_fontSizeSpinBox = new QSpinBox(this);
    m_fontSizeSpinBox->setMinimum(6);
    m_fontSizeSpinBox->setMaximum(36);
    m_fontSizeSpinBox->setSingleStep(1);
    m_fontSizeSpinBox->setSuffix(" px");
    m_fontSizeSpinBox->setValue(m_settings->fontSize());
    fontLayout->addWidget(m_fontSizeSpinBox);

    layout->addWidget(fontGroup);

    layout->addStretch();

    // Buttons
    buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Apply | QDialogButtonBox::Close, this);
    layout->addWidget(buttonBox);

    setupConnections();
}

void NetStatusSettingsDialog::setupConnections()
{
    connect(m_showSpeedCheckBox, &QCheckBox::toggled, this, [this](bool) {
        m_modified = true;
    });

    connect(m_showTooltipCheckBox, &QCheckBox::toggled, this, [this](bool) {
        m_modified = true;
    });

    connect(m_intervalSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int) {
        m_modified = true;
    });

    connect(m_fontSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int) {
        m_modified = true;
    });

    connect(buttonBox, &QDialogButtonBox::clicked, this, [this](QAbstractButton *button) {
        if (button == static_cast<QAbstractButton*>(buttonBox->button(QDialogButtonBox::Apply)))
            onApply();
        else if (button == static_cast<QAbstractButton*>(buttonBox->button(QDialogButtonBox::Close)))
            close();
    });
}

void NetStatusSettingsDialog::onApply()
{
    m_settings->setShowSpeed(m_showSpeedCheckBox->isChecked());
    m_settings->setShowTooltip(m_showTooltipCheckBox->isChecked());
    m_settings->setUpdateInterval(m_intervalSpinBox->value());
    m_settings->setFontSize(m_fontSizeSpinBox->value());

    m_modified = false;

    qCDebug(LC_NETSTATUS_DIALOG) << "Settings applied";
}
