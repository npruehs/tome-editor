#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QSharedPointer>

#include "../Features/Projects/Model/project.h"


namespace Tome
{
    class ComponentsController;
    class ExportController;
    class FieldDefinitionsController;
    class RecordsController;
    class SettingsController;
    class TypesController;

    class Controller
    {
        public:
            Controller();
            ~Controller();

            ComponentsController& getComponentsController();
            FieldDefinitionsController& getFieldDefinitionsController();
            RecordsController& getRecordsController();
            ExportController& getExportController();
            SettingsController& getSettingsController();
            TypesController& getTypesController();

            void createProject(const QString& projectName, const QString& projectPath);
            const QString getFullProjectPath() const;
            const QString getProjectName() const;
            const QString getProjectPath() const;
            bool isProjectLoaded() const;
            void openProject(const QString& projectFileName);
            void saveProject();

        private:
            static const QString FieldDefinitionFileExtension;
            static const QString ProjectFileExtension;
            static const QString RecordExportComponentDelimiterExtension;
            static const QString RecordExportComponentTemplateExtension;
            static const QString RecordExportRecordFileTemplateExtension;
            static const QString RecordExportRecordTemplateExtension;
            static const QString RecordExportRecordDelimiterExtension;
            static const QString RecordExportFieldValueTemplateExtension;
            static const QString RecordExportFieldValueDelimiterExtension;
            static const QString RecordFileExtension;

            QSharedPointer<Project> project;

            ComponentsController* componentsController;
            FieldDefinitionsController* fieldDefinitionsController;
            RecordsController* recordsController;
            ExportController* exportController;
            SettingsController* settingsController;
            TypesController* typesController;

            const QString getFullProjectPath(QSharedPointer<Project> project) const;
            void saveProject(QSharedPointer<Project> project);
            void setProject(QSharedPointer<Project> project);
            QString readFile(const QString& path, const QString& fileName);
    };
}

#endif // CONTROLLER_H
