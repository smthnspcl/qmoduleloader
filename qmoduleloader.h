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
    explicit ModuleLoader(QObject *parent = nullptr): QObject(parent){};

    ~ModuleLoader() override {
        modules.clear();
        libraries.clear();
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
        libraries.clear();
        QDir d(moduleDirectory);
        for(const auto& e : d.entryList(QStringList() << "*.so", QDir::Files)) libraries.append(loadModule(d.absoluteFilePath(e)));
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

    template<typename R> static R executeReturn(QLibrary* lib, const QString& functionName) {
        typedef R (*Function)();
        Function f = (Function) lib->resolve(functionName.toStdString().c_str());
        if(!f) qDebug() << lib->errorString();
        return f();
    };

    static QLibrary* loadModule(const QString &name){
        auto *lib = new QLibrary(name.toStdString().c_str());
        if(lib->load()) return lib;
        return nullptr;
    }

    bool modulesLoaded = false;
    bool automaticallyLoadModules = false;
    QString moduleDirectory;
    QMap<QString, QObject*> modules;
    QList<QLibrary*> libraries;
};

#endif //QMODULELOADER_QMODULELOADER_H
