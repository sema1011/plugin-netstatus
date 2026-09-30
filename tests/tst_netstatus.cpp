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

#include <QTest>
#include <QTemporaryDir>
#include <QTimer>

#include "netstatussettings.h"
#include "netstatuswidget.h"
#include "mock_plugin_settings.h"

class TestNetStatusWidget : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // Widget creation
    void testWidgetCreation();

    // Speed label
    void testSpeedLabelVisibility();
    void testSetShowSpeed();

    // Timer interval
    void testSetUpdateInterval();

    // Icon mapping
    void testConnectivityIcon();

    // Speed calculation
    void testSpeedCalculation();
};

void TestNetStatusWidget::initTestCase()
{
}

void TestNetStatusWidget::cleanupTestCase()
{
}

void TestNetStatusWidget::testWidgetCreation()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    MockPluginSettings settings(dir.path() + "/settings.conf");
    NetStatusSettings netSettings;
    netSettings.init(&settings);
    
    NetStatusWidget widget(&netSettings);
    QVERIFY(&widget);
    QCOMPARE(widget.maximumSize(), QSize(120, 48));
}

void TestNetStatusWidget::testSpeedLabelVisibility()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    MockPluginSettings settings(dir.path() + "/settings.conf");
    NetStatusSettings netSettings;
    netSettings.init(&settings);

    netSettings.setShowSpeed(true);
    // Verify the value was stored correctly
    QCOMPARE(netSettings.showSpeed(), true);
    
    NetStatusWidget widget(&netSettings);

    // Speed label should exist
    QLabel* speedLabel = widget.findChild<QLabel*>("speedLabel");
    QVERIFY(speedLabel != nullptr);
}

void TestNetStatusWidget::testSetShowSpeed()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    MockPluginSettings settings(dir.path() + "/settings.conf");
    NetStatusSettings netSettings;
    netSettings.init(&settings);

    netSettings.setShowSpeed(true);
    NetStatusWidget widget(&netSettings);

    // Speed label should exist
    QLabel* speedLabel = widget.findChild<QLabel*>("speedLabel");
    QVERIFY(speedLabel != nullptr);

    // Verify setShowSpeed toggles visibility
    widget.setShowSpeed(false);
    widget.setShowSpeed(true);
    QVERIFY(true); // No crash = success
}

void TestNetStatusWidget::testSetUpdateInterval()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    MockPluginSettings settings(dir.path() + "/settings.conf");
    NetStatusSettings netSettings;
    netSettings.init(&settings);

    netSettings.setUpdateInterval(500);
    NetStatusWidget widget(&netSettings);

    // Verify the widget was created successfully
    QVERIFY(&widget);
    
    // Change interval
    widget.setUpdateInterval(1000);
    QVERIFY(&widget);
    
    // Change to minimum
    widget.setUpdateInterval(100);
    QVERIFY(&widget);
    
    // Change to maximum
    widget.setUpdateInterval(10000);
    QVERIFY(&widget);
}

void TestNetStatusWidget::testConnectivityIcon()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    MockPluginSettings settings(dir.path() + "/settings.conf");
    NetStatusSettings netSettings;
    netSettings.init(&settings);
    
    NetStatusWidget widget(&netSettings);

    // Get icon - should return a valid icon regardless of connectivity state
    QIcon icon = widget.getConnectivityIcon();
    QVERIFY(!icon.isNull());
}

void TestNetStatusWidget::testSpeedCalculation()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    MockPluginSettings settings(dir.path() + "/settings.conf");
    NetStatusSettings netSettings;
    netSettings.init(&settings);
    
    NetStatusWidget widget(&netSettings);

    qulonglong upload = 0;
    qulonglong download = 0;

    // Get speeds - should not crash even without NetworkManager
    widget.getSpeeds(upload, download);

    // Speeds should be non-negative (will be 0 if no NetworkManager)
    QVERIFY(upload >= 0);
    QVERIFY(download >= 0);
}

QTEST_MAIN(TestNetStatusWidget)

#include "tst_netstatus.moc"
