/*
    Copyright (C) 2013 Harald Sitter <apachelogger@kubuntu.org>

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

#ifndef MODULE_H
#define MODULE_H

#include <KCModule>

namespace Ui {
    class Module;
}

class ComUbuntuWhoopsiePreferencesInterface;
class DataStore;

struct DataStore
{
    bool reportCrashes;
    bool reportMetrics;
    bool automaticallyReportCrashes;

    bool operator ==(const DataStore &other) {
        return !(bool)((reportCrashes ^ other.reportCrashes)
                       ^ (reportMetrics ^ other.reportMetrics)
                       ^ (automaticallyReportCrashes ^ other.automaticallyReportCrashes));
    }
};

class Module : public KCModule
{
    Q_OBJECT
public:
    /**
     * Constructor.
     *
     * @param parent Parent widget of the module
     * @param args Arguments for the module
     */
    explicit Module(QWidget *parent, const QVariantList &args = QVariantList());

    /**
     * Destructor.
     */
    ~Module();

    /**
     * Overloading the KCModule load() function.
     */
    void load();

    /**
     * Overloading the KCModule save() function.
     */
    void save();

    /**
     * Overloading the KCModule defaults() function.
     */
    void defaults();

private slots:
    /// Updates currentData DataStore instance.
    void diff();

    void openUrl(const QString &url);

private:
    /// UI
    Ui::Module *ui;
    /// DBus Interface
    ComUbuntuWhoopsiePreferencesInterface *m_preferences;

    /// DataStore instance filled on load() to compare against later. Mutable.
    DataStore m_initialData;
    /// DataStore instance updated by diff(), representing the present state.
    DataStore m_currentData;
};

#endif // MODULE_H
