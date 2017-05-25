#ifndef IMPORTCONTROLLER_H
#define IMPORTCONTROLLER_H

#include <QString>
#include <QVariant>

#include "../Model/recordtableimporttemplatelist.h"
#include "../../Records/Model/recordfieldvaluemap.h"

namespace Tome
{
    class FieldDefinitionsController;
    class RecordsController;
    class TypesController;

    /**
     * @brief Controller for importing record data.
     */
    class ImportController : public QObject
    {
            Q_OBJECT

        public:
            /**
             * @brief Constructs a new controller for importing record data.
             * @param fieldDefinitionsController Controller for adding, updating and removing field definitions.
             * @param recordsController Controller for adding, updating and removing records.
             * @param typesController Controller for adding, updating and removing custom types.
             */
            ImportController(FieldDefinitionsController& fieldDefinitionsController,
                             RecordsController& recordsController,
                             TypesController& typesController);

            /**
             * @brief Adds the specified record import template to the project.
             * @param importTemplate Import template to add.
             */
            void addRecordImportTemplate(const RecordTableImportTemplate& importTemplate);

            /**
             * @brief Gets the record import template with the specified name.
             *
             * @exception std::out_of_range if the record import template could not be found.
             *
             * @param name Name of the record import template to get.
             * @return Record import template with the specified name.
             */
            const RecordTableImportTemplate getRecordTableImportTemplate(const QString& name) const;

            /**
             * @brief Gets a list of all record import templates in the project.
             * @return List of all record import templates in the project.
             */
            const RecordTableImportTemplateList getRecordTableImportTemplates() const;

            /**
             * @brief Begins importing records asynchronously using the specified template.
             * @param importTemplate Template to use for importing the record data.
             * @param context Context to import the data in (e.g. source file name).
             */
            void importRecords(const RecordTableImportTemplate& importTemplate, const QVariant& context);

            /**
             * @brief Removes the record import template with the specified name from the project.
             * @param name Name of the record import template to remove.
             * @return true, if the import template was removed, and false otherwise.
             */
            bool removeImportTemplate(const QString& name);

            /**
             * @brief Uses the specified list of record import templates as model for this controller.
             * @param importTemplates New list of record import templates to use as model for this controller.
             */
            void setRecordTableImportTemplates(RecordTableImportTemplateList& importTemplates);

        signals:
            /**
             * @brief Importing records asynchronously has failed.
             * @param error Localized error message.
             */
            void importError(const QString& error) const;

            /**
             * @brief Importing records asynchronously has finished.
             */
            void importFinished() const;

            /**
             * @brief Importing records asynchronously has started.
             */
            void importStarted() const;

            /**
             * @brief Available record import templates have changed.
             */
            void importTemplatesChanged();

            /**
             * @brief Importing records asynchronously has made notable progress.
             * @param title Localized title of the progress message.
             * @param text Localized text of the progress message.
             * @param currentValue Current progress value (e.g. imported record count).
             * @param maximumValue Maximum progress value (e.g. maximum record count).
             */
            void progressChanged(const QString title, const QString text, const int currentValue, const int maximumValue) const;

        private slots:
            void onDataAvailable(const QString& importTemplateName, const QVariant& context, const QMap<QString, RecordFieldValueMap>& data) const;
            void onDataUnavailable(const QString& importTemplateName, const QVariant& context, const QString& error) const;
            void onProgressChanged(const QString title, const QString text, const int currentValue, const int maximumValue) const;

        private:
            FieldDefinitionsController& fieldDefinitionsController;
            RecordsController& recordsController;
            TypesController& typesController;

            RecordTableImportTemplateList* model;
    };
}

#endif // IMPORTCONTROLLER_H
