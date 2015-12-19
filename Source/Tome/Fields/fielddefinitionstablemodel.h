#ifndef FIELDDEFINITIONSTABLEMODEL_H
#define FIELDDEFINITIONSTABLEMODEL_H

#include <QAbstractTableModel>

#include "../Fields/fieldtype.h"
#include "../Projects/project.h"


namespace Tome
{
    class FieldDefinitionsTableModel : public QAbstractTableModel
    {
            Q_OBJECT

        public:
            FieldDefinitionsTableModel(QObject *parent, QSharedPointer<Tome::Project> project);

            int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE ;
            int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
            QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
            QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

            bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) Q_DECL_OVERRIDE;
            bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) Q_DECL_OVERRIDE;

            void addFieldDefinition(const QString& id, const QString& displayName, const FieldType::FieldType& fieldType, const QString& defaultValue, const QString& description);
            void updateFieldDefinition(const int index, const QString& id, const QString& displayName, const FieldType::FieldType& fieldType, const QString& defaultValue, const QString& description);
            void removeFieldDefinition(const int index);

        private:
            QSharedPointer<Tome::Project> project;
    };
}

#endif // FIELDDEFINITIONSTABLEMODEL_H
