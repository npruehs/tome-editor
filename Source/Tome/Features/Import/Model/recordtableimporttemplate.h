#ifndef RECORDTABLEIMPORTTEMPLATE_H
#define RECORDTABLEIMPORTTEMPLATE_H

#include <QMap>
#include <QString>
#include <QStringList>

#include "tabletype.h"

namespace Tome
{
    /**
     * @brief Template for importing record data.
     */
    class RecordTableImportTemplate
    {
        public:
            /**
             * @brief Name of this import template, as shown in the UI.
             */
            QString name;

            /**
             * @brief Relative path of this impoirt template on disk.
             */
            QString path;

            /**
             * @brief Header the table column which contains the record IDs.
             */
            QString idColumn;

            /**
             * @brief Entries of the ID column that should cause the importer to skip the respective row.
             */
            QStringList ignoredIds;

            /**
             * @brief Maps column headers to field IDs (defaults to just use the column header).
             */
            QMap<QString, QString> columnMap;

            /**
             * @brief Type of the import data source.
             */
            TableType::TableType sourceType;

            /**
             * @brief Id of the record to parent all imported new records to.
             */
            QString rootRecordId;

            /**
             * @brief Optional additional import parameters (e.g. which table to import).
             */
            QMap<QString, QString> parameters;

            /**
             * @brief Map that specifies which strings to replace by which ones during import.
             */
            QMap<QString, QString> stringReplacementMap;
    };
}

#endif // RECORDTABLEIMPORTTEMPLATE_H
