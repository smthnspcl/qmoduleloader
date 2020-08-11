//
// Created by nbdy on 11.08.20.
//

#ifndef QMODULELOADER_QMODULELOADER_H
#define QMODULELOADER_QMODULELOADER_H

#include <QObject>
#include <QtQml>

class ModuleLoader : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString moduleDirectory READ getModuleDirectory WRITE setModuleDirectory NOTIFY moduleDirectoryChanged)
    Q_PROPERTY(bool automaticallyLoadModules READ getAutomaticallyLoadModules WRITE setAutomaticallyLoadModules NOTIFY automaticallyLoadModulesChanged)
    Q_PROPERTY(bool modulesLoaded READ getModulesLoaded NOTIFY modulesLoadedChanged)

public:
    explicit ModuleLoader(QObject *parent = nullptr){};

    ~ModuleLoader() override {

    };

    QString getModuleDirectory() const {
        return moduleDirectory;
    }

    void setModuleDirectory(const QString& directory){
        setModulesLoaded(false);
        modules.clear();
        this->moduleDirectory = directory;
        emit moduleDirectoryChanged();
        if(automaticallyLoadModules) loadModules();
    }

    bool getAutomaticallyLoadModules() const {
        return this->automaticallyLoadModules;
    }

    void setAutomaticallyLoadModules(bool value) {
        this->automaticallyLoadModules = value;
        emit automaticallyLoadModulesChanged();
    }

    bool getModulesLoaded() const {
        return modulesLoaded;
    }

    Q_INVOKABLE void loadModules() {
        if(modulesLoaded) return;

        setModulesLoaded(true);
    }

    Q_INVOKABLE QObject* getModule(const QString& name) {
        if (!modules.contains(name)) return nullptr;
        return modules.value(name);
    }

signals:
    void moduleDirectoryChanged();
    void automaticallyLoadModulesChanged();
    void modulesLoadedChanged();

private:
    void setModulesLoaded(bool value){
        modulesLoaded = value;
        emit modulesLoadedChanged();
    }

    static QStringList listDirectory(const QString& directory){
        QStringList r;
        QDir dir(directory);
        for(const auto& a : dir.entryList(QDir::Filter::Dirs)) r.append(a);
        return r;
    }

    bool modulesLoaded = false;
    bool automaticallyLoadModules = false;
    QString moduleDirectory;
    QMap<QString, QObject*> modules;
};

#endif //QMODULELOADER_QMODULELOADER_H
