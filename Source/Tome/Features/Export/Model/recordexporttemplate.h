#ifndef RECORDEXPORTTEMPLATE_H
#define RECORDEXPORTTEMPLATE_H

#include <QMap>
#include <QString>

namespace Tome
{
    /**
     * @brief Template for exporting record data.
     */
    class RecordExportTemplate
    {
        public:
            /**
             * @brief Delimiter to insert between two components.
             */
            QString componentDelimiter;

            /**
             * @brief Template to apply for exporting components.
             */
            QString componentTemplate;

            /**
             * @brief Whether to fill each record export so that all fields are exported for all records.
             */
            bool exportAsTable;

            /**
             * @brief Whether to export records without parent.
             */
            bool exportRoots;

            /**
             * @brief Whether to export records with parent and children.
             */
            bool exportInnerNodes;

            /**
             * @brief Whether to export records without children.
             */
            bool exportLeafs;

            /**
             * @brief Whether to skip all fields whose type does not have the \ref LocalizedStringFacet applied.
             */
            bool exportLocalizedFieldsOnly;

            /**
             * @brief Delimiter to insert between two field values.
             */
            QString fieldValueDelimiter;

            /**
             * @brief Template to apply for exporting field values.
             */
            QString fieldValueTemplate;

            /**
             * @brief Extension of the data files to write the exported record data to.
             */
            QString fileExtension;

            /**
             * @brief Template to apply for exporting list field values.
             */
            QString listTemplate;

            /**
             * @brief Template to apply for exporting list field items.
             */
            QString listItemTemplate;

            /**
             * @brief Delimiter to insert between two list field items.
             */
            QString listItemDelimiter;

            /**
             * @brief Template to apply for exporting all values of fields whose type has the \ref LocalizedStringFacet applied.
             */
            QString localizedFieldValueTemplate;

            /**
             * @brief Template to apply for exporting map field values.
             */
            QString mapTemplate;

            /**
             * @brief Template to apply for exporting map field items.
             */
            QString mapItemTemplate;

            /**
             * @brief Delimiter to insert between two map field items.
             */
            QString mapItemDelimiter;

            /**
             * @brief Name of this export template, as shown in the UI.
             */
            QString name;

            /**
             * @brief Relative path of this export template on disk.
             */
            QString path;

            /**
             * @brief Delimiter to insert between two records.
             */
            QString recordDelimiter;

            /**
             * @brief Main export template that recursively applies all other templates.
             */
            QString recordFileTemplate;

            /**
             * @brief Template to apply for exporting records.
             */
            QString recordTemplate;

            /**
             * @brief Map that specifies which strings to replace by which ones during export.
             */
            QMap<QString, QString> stringReplacementMap;

            /**
             * @brief Map that specifies which type names to replace on export.
             */
            QMap<QString, QString> typeMap;

            /**
             * @brief Records to skip while exporting.
             */
            QStringList ignoredRecords;

            /**
             * @brief Fields to skip while exporting.
             */
            QStringList ignoredFields;
    };
}

#endif // RECORDEXPORTTEMPLATE_H
