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

    /**
     * @brief Controller for creating, loading and saving projects.
     */
    class ProjectController : public QObject
    {
             Q_OBJECT

        public:
            /**
             * @brief File extension of Tome component files, including the dot.
             */
            static const QString ComponentFileExtension;

            /**
             * @brief File extension of Tome field definition files, including the dot.
             */
            static const QString FieldDefinitionFileExtension;

            /**
             * @brief File extension of Tome project files, including the dot.
             */
            static const QString ProjectFileExtension;

            /**
             * @brief File extension of Tome record export component delimiter files, including the dot.
             */
            static const QString RecordExportComponentDelimiterExtension;

            /**
             * @brief File extension of Tome record export component template files, including the dot.
             */
            static const QString RecordExportComponentTemplateExtension;

            /**
             * @brief File extension of Tome record export field value template files, including the dot.
             */
            static const QString RecordExportFieldValueTemplateExtension;

            /**
             * @brief File extension of Tome record export field value delimiter files, including the dot.
             */
            static const QString RecordExportFieldValueDelimiterExtension;

            /**
             * @brief File extension of Tome record export list template files, including the dot.
             */
            static const QString RecordExportListTemplateExtension;

            /**
             * @brief File extension of Tome record export list item template files, including the dot.
             */
            static const QString RecordExportListItemTemplateExtension;

            /**
             * @brief File extension of Tome record export list item delimiter files, including the dot.
             */
            static const QString RecordExportListItemDelimiterExtension;

            /**
             * @brief File extension of Tome record export localized field value template files, including the dot.
             */
            static const QString RecordExportLocalizedFieldValueTemplateExtension;

            /**
             * @brief File extension of Tome record export map template files, including the dot.
             */
            static const QString RecordExportMapTemplateExtension;

            /**
             * @brief File extension of Tome record export map item template files, including the dot.
             */
            static const QString RecordExportMapItemTemplateExtension;

            /**
             * @brief File extension of Tome record export map item delimiter files, including the dot.
             */
            static const QString RecordExportMapItemDelimiterExtension;

            /**
             * @brief File extension of Tome record export record file template files, including the dot.
             */
            static const QString RecordExportRecordFileTemplateExtension;

            /**
             * @brief File extension of Tome record export record template files, including the dot.
             */
            static const QString RecordExportRecordTemplateExtension;

            /**
             * @brief File extension of Tome record export record delimiter files, including the dot.
             */
            static const QString RecordExportRecordDelimiterExtension;

            /**
             * @brief File extension of Tome record export template files, including the dot.
             */
            static const QString RecordExportTemplateFileExtension;

            /**
             * @brief File extension of Tome record files, including the dot.
             */
            static const QString RecordFileExtension;

            /**
             * @brief File extension of Tome import template files, including the dot.
             */
            static const QString RecordImportTemplateFileExtension;

            /**
             * @brief File extension of Tome type files, including the dot.
             */
            static const QString TypeFileExtension;

            /**
             * @brief Constructs a new controller for creating, loading and saving projects.
             */
            ProjectController();
            ~ProjectController();

            /**
             * @brief Ensures the specified file path is an absolute file path with the passed extension.
             * @param filePath File path to convert.
             * @param projectPath Absolute path to the current project, without file name.
             * @param desiredExtension File extension to add, if missing.
             * @return Specified file path as absolute file path with the passed extension.
             */
            QString buildFullFilePath(QString filePath, QString projectPath, QString desiredExtension) const;

            /**
             * @brief Creates a new project with the specified name and path and saves it to disk.
             * @param projectName Name of the project to create.
             * @param projectPath Path to write the project files to.
             * @param recordIdType Type of the ids of the records of the new project.
             */
            void createProject(const QString& projectName, const QString& projectPath, const Tome::RecordIdType::RecordIdType recordIdType);

            /**
             * @brief Gets the absolute path of the current project, including project file name and extension.
             * @return Absolute full path of the current project file.
             */
            const QString getFullProjectPath() const;

            /**
             * @brief Gets the name of the current project.
             * @return Name of the current project.
             */
            const QString getProjectName() const;

            /**
             * @brief Gets the absolute path of the current project, without project file name.
             * @return Absolute path of the current project, without project file name.
             */
            const QString getProjectPath() const;

            /**
             * @brief Gets the type of the ids of the records of the current project.
             * @return Type of the ids of the records of the current project.
             */
            RecordIdType::RecordIdType getProjectRecordIdType() const;

            /**
             * @brief Checks whether the current project allows to modify read-only data.
             * @return true, if the current project allows to modify read-only data, and false otherwise.
             */
            bool getProjectIgnoreReadOnly() const;

            /**
             * @brief Checks whether any project is loaded.
             * @return true, if any project is currently loaded, and false otherwise.
             */
            bool isProjectLoaded() const;

            /**
             * @brief Loads the specified component set from disk.
             *
             * @exception std::runtime_error if the component set file could not be read.
             *
             * @param projectPath Absolute path to the project, without file name.
             * @param componentSet Component set to load from disk.
             */
            void loadComponentSet(const QString& projectPath, ComponentSet& componentSet) const;

            /**
             * @brief Loads the specified custom type set from disk.
             *
             * @exception std::runtime_error if the custom type set file could not be read.
             *
             * @param projectPath Absolute path to the project, without file name.
             * @param customTypeSet Custom type set to load from disk.
             */
            void loadCustomTypeSet(const QString& projectPath, CustomTypeSet& customTypeSet) const;

            /**
             * @brief Loads the specified export template from disk.
             *
             * @exception std::runtime_error if any of the export template files could not be read.
             *
             * @param projectPath Absolute path to the project, without file name.
             * @param exportTemplate Export template to load from disk.
             */
            void loadExportTemplate(const QString& projectPath, RecordExportTemplate& exportTemplate) const;

            /**
             * @brief Loads the specified field definition set from disk.
             *
             * @exception std::runtime_error if the field definition set file could not be read.
             *
             * @param projectPath Absolute path to the project, without file name.
             * @param fieldDefinitionSet Field definition set to load from disk.
             */
            void loadFieldDefinitionSet(const QString& projectPath, FieldDefinitionSet& fieldDefinitionSet) const;

            /**
             * @brief Loads the specified import template set from disk.
             *
             * @exception std::runtime_error if the import template file could not be read.
             *
             * @param projectPath Absolute path to the project, without file name.
             * @param importTemplate Import template to load from disk.
             */
            void loadImportTemplate(const QString& projectPath, RecordTableImportTemplate& importTemplate) const;

            /**
             * @brief Loads the specified record set set from disk.
             *
             * @exception std::runtime_error if the record set file could not be read.
             *
             * @param projectPath Absolute path to the project, without file name.
             * @param recordSet Record set to load from disk.
             */
            void loadRecordSet(const QString& projectPath, RecordSet& recordSet) const;

            /**
             * @brief Opens the specified project file.
             *
             * @exception std::runtime_error if the project file could not be read.
             *
             * @param projectFileName Name of the project file to open.
             */
            void openProject(const QString& projectFileName);

            /**
             * @brief Saves the current project to disk, including all data and templates.
             *
             * @exception std::runtime_error if any of the project files could not be written.
             */
            void saveProject() const;

        signals:
            /**
             * @brief Progress of the current project operation has changed.
             * @param title Title of the current project operation step.
             * @param text Details of the current project operation step.
             * @param currentValue Current progress value of the current project operation step.
             * @param maximumValue Maximum progress value of the current project operation step.
             */
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue);

            /**
             * @brief Current project has changed (e.g. newly created or loaded from disk).
             * @param project Current project.
             */
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
