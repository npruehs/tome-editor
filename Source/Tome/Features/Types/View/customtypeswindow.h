#ifndef CUSTOMTYPESWINDOW_H
#define CUSTOMTYPESWINDOW_H

#include <QMainWindow>

class DerivedTypeWindow;
class EnumerationWindow;
class ListWindow;
class MapWindow;

namespace Ui {
    class CustomTypesWindow;
}

namespace Tome
{
    class CustomType;
    class FacetsController;
    class FieldDefinitionsController;
    class FindUsagesController;
    class RecordsController;
    class TypesController;
}

class CustomTypesWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit CustomTypesWindow(Tome::TypesController& typesController,
                                   Tome::FacetsController& facetsController,
                                   Tome::FieldDefinitionsController& fieldDefinitionsController,
                                   Tome::FindUsagesController& findUsagesController,
                                   Tome::RecordsController& recordsController,
                                   QWidget *parent = 0);
        ~CustomTypesWindow();

    protected:
        void showEvent(QShowEvent * event);

    private slots:
        void on_actionNew_Derived_Type_triggered();
        void on_actionNew_Custom_Type_triggered();
        void on_actionNew_List_triggered();
        void on_actionNew_Map_triggered();

        void on_actionEdit_Custom_Type_triggered();
        void on_actionDelete_Custom_Type_triggered();

        void on_actionFind_Usages_triggered();

        void on_tableWidget_doubleClicked(const QModelIndex &index);

    private:
        Ui::CustomTypesWindow *ui;

        Tome::TypesController& typesController;
        Tome::FacetsController& facetsController;
        Tome::FieldDefinitionsController& fieldDefinitionsController;
        Tome::FindUsagesController& findUsagesController;
        Tome::RecordsController& recordsController;

        DerivedTypeWindow* derivedTypeWindow;
        EnumerationWindow* enumerationWindow;
        ListWindow* listWindow;
        MapWindow* mapWindow;

        int getSelectedTypeIndex() const;
        QString getSelectedTypeName() const;

        void editDerivedType(QString typeName, const Tome::CustomType& type);
        void editEnumeration(QString typeName, const Tome::CustomType& type);
        void editList(QString typeName, const Tome::CustomType& type);
        void editMap(QString typeName, const Tome::CustomType& type);

        void updateDerivedType(const QString& oldName, const QString& newName, const QString& baseType, const QVariantMap facets, const QString& typeSetName);
        void updateEnumeration(const QString& oldName, const QString& newName, const QStringList& enumeration, const QString& typeSetName);
        void updateList(const QString& oldName, const QString& name, const QString& itemType, const QString& typeSetName);
        void updateMap(const QString& oldName, const QString& newName, const QString& keyType, const QString& valueType, const QString& typeSetName);

        void updateRow(const int index, const Tome::CustomType& type);
        void updateTable();
};

#endif // CUSTOMTYPESWINDOW_H
