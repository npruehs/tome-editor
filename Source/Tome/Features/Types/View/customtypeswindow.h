#ifndef CUSTOMTYPESWINDOW_H
#define CUSTOMTYPESWINDOW_H

#include <QMainWindow>

class EnumerationWindow;
class ListWindow;

namespace Ui {
    class CustomTypesWindow;
}

namespace Tome
{
    class CustomType;
    class FieldDefinitionsController;
    class FindUsagesController;
    class TypesController;
}

class CustomTypesWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit CustomTypesWindow(Tome::TypesController& typesController, Tome::FieldDefinitionsController& fieldDefinitionsController, Tome::FindUsagesController& findUsagesController, QWidget *parent = 0);
        ~CustomTypesWindow();

    private slots:
        void on_actionNew_Custom_Type_triggered();
        void on_actionNew_List_triggered();

        void on_actionEdit_Custom_Type_triggered();
        void on_actionDelete_Custom_Type_triggered();

        void on_actionFind_Usages_triggered();

        void on_tableWidget_doubleClicked(const QModelIndex &index);

    private:
        Ui::CustomTypesWindow *ui;
        Tome::TypesController& typesController;
        Tome::FieldDefinitionsController& fieldDefinitionsController;
        Tome::FindUsagesController& findUsagesController;

        EnumerationWindow* enumerationWindow;
        ListWindow* listWindow;

        int getSelectedTypeIndex() const;
        QString getSelectedTypeName() const;

        void editEnumeration(QString typeName, const Tome::CustomType& type);
        void editList(QString typeName, const Tome::CustomType& type);

        void updateEnumeration(const QString& oldName, const QString& newName, const QStringList& enumeration);
        void updateList(const QString& oldName, const QString& name, const QString& itemType);
        void updateRow(const int index, const Tome::CustomType& type);
};

#endif // CUSTOMTYPESWINDOW_H
