#ifndef RECORD_H
#define RECORD_H

#include <QMap>
#include <QString>
#include <QVariant>

#include "recordfieldvaluemap.h"


namespace Tome
{
    /**
     * @brief Data record that combines the values of various fields.
     */
    class Record
    {
        public:
            /**
             * @brief Unique id of this record.
             */
            QVariant id;

            /**
             * @brief Display name of this record.
             */
            QString displayName;

            /**
             * @brief Id of the field to use as editor icon for this record.
             */
            QString editorIconFieldId;

            /**
             * @brief Values of all fields of this record.
             */
            RecordFieldValueMap fieldValues;

            /**
             * @brief Id of the parent of this record, or null if this record is a root of the record tree.
             */
            QVariant parentId;

            /**
             * @brief Whether to prevent this record from being edited, reparented or removed.
             */
            bool readOnly = false;

            /**
             * @brief Name of the set this record belongs to.
             */
            QString recordSetName;
    };

    inline bool operator==(const Record& lhs, const Record& rhs){ return lhs.id == rhs.id; }
    inline bool operator!=(const Record& lhs, const Record& rhs){ return !(lhs == rhs); }

    inline bool recordLessThanDisplayName(const Record& e1, const Record& e2)
    {
        return e1.displayName.toLower() < e2.displayName.toLower();
    }

    inline bool recordLessThanId(const Record& e1, const Record& e2)
    {
        return e1.id.toString().toLower() < e2.id.toString().toLower();
    }
}

#endif // RECORD_H
