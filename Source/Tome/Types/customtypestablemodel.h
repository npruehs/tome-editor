#ifndef CUSTOMFIELDTYPESITEMMODEL_H
#define CUSTOMFIELDTYPESITEMMODEL_H

#include <QStandardItemModel>

#include "../Projects/project.h"

namespace Tome
{
    class CustomTypesTableModel : public QAbstractTableModel
    {
        public:
            CustomTypesTableModel(QObject *parent, QSharedPointer<Tome::Project> project);

            int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE ;
            int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
            QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
            QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

            bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) Q_DECL_OVERRIDE;
            bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) Q_DECL_OVERRIDE;

            void addEnumeration(const QString& name, const QStringList& enumeration);
            void addList(const QString& name, const QString& itemType);

            void updateEnumeration(const int index, const QString& name, const QStringList& enumeration);
            void updateList(const int index, const QString& name, const QString& itemType);

            void removeCustomType(const int index);

        private:
            QSharedPointer<Tome::Project> project;
    };
}

#endif // CUSTOMFIELDTYPESITEMMODEL_H
