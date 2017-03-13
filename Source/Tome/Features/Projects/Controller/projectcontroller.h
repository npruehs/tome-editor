#ifndef PROJECTCONTROLLER_H
#define PROJECTCONTROLLER_H

#include <QSharedPointer>

#include "../Model/recordidtype.h"

namespace Tome
{
    class ComponentSet;
    class CustomTypeSet;
    class FieldDefinitionSet;
    class Project;
    class RecordExportTemplate;
    class RecordSet;
    class RecordSetSerializer;
    class RecordTableImportTemplate;

    class ProjectController : public QObject
    {
             Q_OBJECT

        public:
            static const QString ComponentFileExtension;
            static const QString FieldDefinitionFileExtension;
            static const QString ProjectFileExtension;
            static const QString RecordExportComponentDelimiterExtension;
            static const QString RecordExportComponentTemplateExtension;
            static const QString RecordExportFieldValueTemplateExtension;
            static const QString RecordExportFieldValueDelimiterExtension;
            static const QString RecordExportListTemplateExtension;
            static const QString RecordExportListItemTemplateExtension;
            static const QString RecordExportListItemDelimiterExtension;
            static const QString RecordExportLocalizedFieldValueTemplateExtension;
            static const QString RecordExportMapTemplateExtension;
            static const QString RecordExportMapItemTemplateExtension;
            static const QString RecordExportMapItemDelimiterExtension;
            static const QString RecordExportRecordFileTemplateExtension;
            static const QString RecordExportRecordTemplateExtension;
            static const QString RecordExportRecordDelimiterExtension;
            static const QString RecordExportTemplateFileExtension;
            static const QString RecordFileExtension;
            static const QString RecordImportTemplateFileExtension;
            static const QString TypeFileExtension;

            ProjectController();
            ~ProjectController();

            QString buildFullFilePath(QString filePath, QString projectPath, QString desiredExtension) const;
            void createProject(const QString& projectName, const QString& projectPath, const Tome::RecordIdType::RecordIdType recordIdType);
            const QString getFullProjectPath() const;
            const QString getProjectName() const;
            const QString getProjectPath() const;
            RecordIdType::RecordIdType getProjectRecordIdType() const;
            bool getProjectIgnoreReadOnly() const;
            bool isProjectLoaded() const;
            void loadComponentSet(const QString& projectPath, ComponentSet& componentSet) const;
            void loadCustomTypeSet(const QString& projectPath, CustomTypeSet& customTypeSet) const;
            void loadExportTemplate(const QString& projectPath, RecordExportTemplate& exportTemplate) const;
            void loadFieldDefinitionSet(const QString& projectPath, FieldDefinitionSet& fieldDefinitionSet) const;
            void loadImportTemplate(const QString& projectPath, RecordTableImportTemplate& importTemplate) const;
            void loadRecordSet(const QString& projectPath, RecordSet& recordSet) const;
            void openProject(const QString& projectFileName);
            void saveProject() const;

        signals:
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue);
            void projectChanged(QSharedPointer<Tome::Project> project);

        private slots:
            void onProgressChanged(const QString title, const QString text, const int currentValue, const int maximumValue);

        private:
            QSharedPointer<Project> project;

            RecordSetSerializer* recordSetSerializer;

            const QString getFullProjectPath(QSharedPointer<Project> project) const;
            QString readFile(const QString& fullPath) const;
            void saveProject(QSharedPointer<Project> project) const;
            void setProject(QSharedPointer<Project> project);
    };
}

#endif // PROJECTCONTROLLER_H
