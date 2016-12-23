#ifndef TABLETYPE_H
#define TABLETYPE_H

#include <QString>

namespace Tome
{
    namespace TableType
    {
        enum TableType
        {
            None,
            Csv,
            GoogleSheets,
            Xlsx,
        };

        inline TableType fromString(QString tableType)
        {
            QString tableTypeLower = tableType.toLower();

            if (tableTypeLower == "csv")
            {
                return TableType::Csv;
            }
            else if (tableTypeLower == "googlesheets")
            {
                return TableType::GoogleSheets;
            }
            else if (tableTypeLower == "xlsx")
            {
                return TableType::Xlsx;
            }
            else
            {
                return TableType::None;
            }
        }

        inline const QString toString(TableType tableType)
        {
            switch (tableType)
            {
                case TableType::Csv:
                    return "CSV";

                case TableType::GoogleSheets:
                    return "GoogleSheets";

                case TableType::Xlsx:
                    return "XLSX";

                default:
                    return "None";
            }
        }
    }
}
#endif // TABLETYPE_H
