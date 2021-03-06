/*
    Copyright (C) 2013-2014 Harald Sitter <apachelogger@kubuntu.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License or (at your option) version 3 or any later version
    accepted by the membership of KDE e.V. (or its successor approved
    by the membership of KDE e.V.), which shall act as a proxy
    defined in Section 14 of version 3 of the license.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Module.h"
#include "ui_Module.h"

#include <KAboutData>
#include <KAuthAction>
#include <KLocalizedString>
#include <KPluginFactory>
#include <KToolInvocation>

#include <QFontMetrics>

#include "WhoopsiePreferences.h"
#include "Version.h"

Module::Module(QWidget *parent, const QVariantList &args)
    : KCModule(parent, args)
    , ui(new Ui::Module)
    , m_preferences(new com::ubuntu::WhoopsiePreferences(QLatin1String("com.ubuntu.WhoopsiePreferences"),
                                                         QLatin1String("/com/ubuntu/WhoopsiePreferences"),
                                                         QDBusConnection::systemBus(),
                                                         this))
{
    KAboutData *aboutData = new KAboutData("kcm-whoopsie",
                                        i18nc("@title", "Diagnostics"),
                                        global_s_versionStringFull,
                                        QStringLiteral(""),
                                        KAboutLicense::LicenseKey::GPL_V3,
                                        i18nc("@info:credit", "Copyright 2013-2014 Harald Sitter"));

    aboutData->addAuthor(i18nc("@info:credit", "Harald Sitter"),
                        i18nc("@info:credit", "Author"),
                        QStringLiteral("apachelogger@kubuntu.org"));


    setAboutData(aboutData);

    // Must NOT be set, as the builtin KAuth wiring expects an appropriate handler.
    // However since we use WhoopsiePreferences directly there is no KAuth handler
    // and turning on Authorization would only get in the way of manual handling in
    // save().
    // setNeedsAuthorization(true);
    ui->setupUi(this);

    ui->headingLabel->setText(xi18nc("@info",
                                    "Kubuntu can collect anonymous information"
                                    " that helps developers improve it. All"
                                    " information collected is covered by our"
                                    " <link url='%1'>privacy policy</link>.",
                                    QLatin1String("http://www.ubuntu.com/aboutus/privacypolicy?crashdb")));
    connect(ui->headingLabel, SIGNAL(linkActivated(QString)), this, SLOT(openUrl(QString)));

    // Qt internally has a spacing of 4...
    ui->crashesSpacer->changeSize(ui->crashesCheckBox->iconSize().width() + 4, 1);
    ui->metricsSpacer->changeSize(ui->metricsCheckBox->iconSize().width() + 4, 1);

    QFont descriptionFont = ui->crashesDescriptionLabel->font();
    descriptionFont.setItalic(true);
    ui->crashesDescriptionLabel->setFont(descriptionFont);
    ui->metricsDescriptionLabel->setFont(descriptionFont);

    // Space in height of one default line in between logical groups.
    // This avoids more excessive grouping techniques such as frames or boxes
    // which tend to look terribad in Oxygen.
    const QFontMetrics metrics(QApplication::font());
    layout()->setSpacing(metrics.height());

    connect(ui->crashesCheckBox, SIGNAL(toggled(bool)), this, SLOT(diff()));
    connect(ui->metricsCheckBox, SIGNAL(toggled(bool)), this, SLOT(diff()));
    connect(ui->autoCheckBox, SIGNAL(toggled(bool)), this, SLOT(diff()));

    connect(ui->crashesCheckBox, SIGNAL(toggled(bool)),
            ui->autoCheckBox, SLOT(setEnabled(bool)));

    // We have no help so remove the button from the buttons.
    setButtons(buttons() ^ KCModule::Help);

#warning metrics are not implemented in whoopsie as of 2013-12-09
    ui->metricsCheckBox->hide();
    ui->metricsSpacer->changeSize(0, 0);
    ui->metricsDescriptionLabel->hide();
}

Module::~Module()
{
    delete ui;
}

void Module::load()
{
    m_initialData.reportCrashes = m_preferences->reportCrashes();
    m_initialData.reportMetrics = m_preferences->reportMetrics();
    m_initialData.automaticallyReportCrashes = m_preferences->automaticallyReportCrashes();

    ui->crashesCheckBox->setChecked(m_initialData.reportCrashes);
    ui->metricsCheckBox->setChecked(m_initialData.reportMetrics);
    ui->autoCheckBox->setChecked(m_initialData.automaticallyReportCrashes);

    ui->autoCheckBox->setEnabled(ui->crashesCheckBox->isChecked());

    const QString reportsUrl =
            QString::fromLatin1("https://errors.ubuntu.com/user/%1").arg(m_preferences->GetIdentifier());
    ui->previousReportsLabel->setText(xi18nc("@info",
                                            "<link url='%1'>Previous Reports</link>", reportsUrl));
    connect(ui->previousReportsLabel, SIGNAL(linkActivated(QString)), this, SLOT(openUrl(QString)));
}

void Module::save()
{
    diff();
    KAuth::Action a("com.ubuntu.whoopsiepreferences.change");
    a.execute();
    // Slightly awkward. If we do not deploy these calls in a blocking manner
    // they may not have finished by the time we get destructed (e.g. user clicks
    // 'Ok' button in kcmshell -> save() -> returns with pending calls -> qapp::quit()
    // -> pending calls may never be sent -> changes lost).
    // This happens in particular when KAuth is involved because the user doesn't
    // have authorization otherwise.
    m_preferences->SetReportCrashes(m_currentData.reportCrashes).waitForFinished();
    m_preferences->SetReportMetrics(m_currentData.reportMetrics).waitForFinished();
    m_preferences->SetAutomaticallyReportCrashes(m_currentData.automaticallyReportCrashes).waitForFinished();
    m_initialData = m_currentData;
    diff();
}

void Module::defaults()
{
}

void Module::diff()
{
    m_currentData.reportCrashes = ui->crashesCheckBox->isChecked();
    m_currentData.reportMetrics = ui->metricsCheckBox->isChecked();
    m_currentData.automaticallyReportCrashes = ui->autoCheckBox->isChecked();

    if (m_currentData == m_initialData)
        emit changed(false);
    else
        emit changed(true);
}

void Module::openUrl(const QString &url)
{
    KToolInvocation::invokeBrowser(url);
}
