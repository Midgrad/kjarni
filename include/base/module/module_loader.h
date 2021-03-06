#ifndef MODULE_LOADER_H
#define MODULE_LOADER_H

#include "i_module.h"

#include <QMap>
#include <QPluginLoader>

namespace md::app
{
class ModuleLoader : public QObject
{
    Q_OBJECT

public:
    explicit ModuleLoader(QObject* parent = nullptr);
    ~ModuleLoader() override;

    QStringList discoveredModules() const;
    QStringList loadedModules() const;
    QJsonObject moduleMetaData(const QString& moduleId) const;
    IModule* module(const QString& moduleId) const;

public slots:
    void discoverModules();
    void loadModule(const QString& moduleId);
    void loadModules(const QStringList& moduleIds);
    void loadModules();
    void unloadModule(const QString& moduleId);
    void unloadAllModules();

signals:
    void moduleDiscovered(QString moduleId);
    void moduleLoaded(QString moduleId);
    void moduleUnloaded(QString moduleId);

private:
    QMap<QString, QPluginLoader*> m_discoveredLoaders;
    QMap<QString, IModule*> m_loadedModules;
};
} // namespace md::app

#endif // MODULE_LOADER_H
