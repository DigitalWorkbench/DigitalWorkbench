#include "app_version.h"
#include <extensionsystem/iplugin.h>
//#include <extensionsystem/pluginerroroverview.h>
#include <extensionsystem/pluginmanager.h>
#include <extensionsystem/pluginspec.h>
#include <utils/hostosinfo.h>

#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>

using namespace ExtensionSystem;

typedef QList<PluginSpec *> PluginSpecSet;

const char corePluginNameC[] = "Core";

static inline QStringList getPluginPaths()
{
    QStringList rc;
    // Figure out root:  Up one from 'bin'
    QDir rootDir = QCoreApplication::applicationDirPath();
    rootDir.cdUp();
    const QString rootDirPath = rootDir.canonicalPath();
    QString pluginPath;
    if (Utils::HostOsInfo::isMacHost()) {
        // 1) "PlugIns" (OS X)
        pluginPath = rootDirPath + QLatin1String("/PlugIns");
        rc.push_back(pluginPath);
    } else {
        // 2) "plugins" (Win/Linux)
        pluginPath = rootDirPath;
        pluginPath += QLatin1Char('/');
//        pluginPath += QLatin1String(IDE_LIBRARY_BASENAME);
        pluginPath += QLatin1String("lib");
        pluginPath += QLatin1String("/qes/plugins");
        rc.push_back(pluginPath);
    }
    // 3) <localappdata>/plugins/<ideversion>
    //    where <localappdata> is e.g.
    //    "%LOCALAPPDATA%\QtProject\qtcreator" on Windows Vista and later
    //    "$XDG_DATA_HOME/data/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator" on Linux
    //    "~/Library/Application Support/QtProject/Qt Creator" on Mac
    pluginPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
    pluginPath += QLatin1String("/data");
#endif
    pluginPath += QLatin1Char('/')
            + QLatin1String(Core::Constants::IDE_SETTINGSVARIANT_STR)
            + QLatin1Char('/');
    pluginPath += QLatin1String(Utils::HostOsInfo::isMacHost() ? "QES" : "qes");
    pluginPath += QLatin1String("/plugins/");
    pluginPath += QLatin1String(Core::Constants::IDE_VERSION_LONG);
    rc.push_back(pluginPath);
    return rc;
}

int main(int argc, char **argv)
{

    QCoreApplication app(argc, argv);

    PluginManager pluginManager;
    PluginManager::setPluginIID(QLatin1String("com.github.projectufa.QExtensionSystem"));

    QStringList customPluginPaths;

    const QStringList pluginPaths = getPluginPaths() + customPluginPaths;
    PluginManager::setPluginPaths(pluginPaths);

    const PluginSpecSet plugins = PluginManager::plugins();
    PluginSpec *coreplugin = 0;
    foreach (PluginSpec *spec, plugins) {
        if (spec->name() == QLatin1String(corePluginNameC)) {
            coreplugin = spec;
            break;
        }
    }
    if (!coreplugin) {
        QString nativePaths = QDir::toNativeSeparators(pluginPaths.join(QLatin1Char(',')));
        const QString reason = QCoreApplication::translate("Application", "Could not find Core plugin in %1").arg(nativePaths);
        qDebug(reason.toLocal8Bit());
//        displayError(msgCoreLoadFailure(reason));
        return 1;
    }
    if (!coreplugin->isEffectivelyEnabled()) {
        const QString reason = QCoreApplication::translate("Application", "Core plugin is disabled.");
        qDebug(reason.toLocal8Bit());
//        displayError(msgCoreLoadFailure(reason));
        return 1;
    }
    if (coreplugin->hasError()) {
        qDebug(coreplugin->errorString().toLocal8Bit());
//        displayError(msgCoreLoadFailure(coreplugin->errorString()));
        return 1;
    }

    PluginManager::loadPlugins();
    if (coreplugin->hasError()) {
        qDebug(coreplugin->errorString().toLocal8Bit());
//        displayError(msgCoreLoadFailure(coreplugin->errorString()));
        return 1;
    }

    // shutdown plugin manager on the exit
    QObject::connect(&app, SIGNAL(aboutToQuit()), &pluginManager, SLOT(shutdown()));

    return app.exec();

}
