#ifndef FIELDDEFINITIONSTABLEMODEL_H
#define FIELDDEFINITIONSTABLEMODEL_H

#include <QAbstractTableModel>

#include "../Projects/project.h"


namespace Tome
{
    class FieldDefinitionsTableModel : public QAbstractTableModel
    {
            Q_OBJECT

        public:
            FieldDefinitionsTableModel(QSharedPointer<Tome::Project> project);

            int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE ;
            int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
            QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
            QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

        private:
            QSharedPointer<Tome::Project> project;
    };
}

#endif // FIELDDEFINITIONSTABLEMODEL_H
