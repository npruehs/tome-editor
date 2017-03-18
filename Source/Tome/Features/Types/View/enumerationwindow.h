#ifndef ENUMERATIONWINDOW_H
#define ENUMERATIONWINDOW_H

#include <QDialog>

class EnumerationMemberWindow;

namespace Ui {
    class EnumerationWindow;
}

namespace Tome
{
    class CustomType;
}

class EnumerationWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit EnumerationWindow(QWidget *parent = 0);
        ~EnumerationWindow();

        QString getEnumerationName() const;
        QStringList getEnumerationMembers() const;
        QString getTypeSetName() const;

        void setEnumerationName(const QString& typeName);
        void setEnumerationMembers(const QStringList enumeration);
        void setTypeSetName(const QString& typeSet);
        void setTypeSetNames(const QStringList& typeSets);

    public slots:
        void accept();

    protected:
        virtual void showEvent(QShowEvent* event) Q_DECL_OVERRIDE;

    private slots:
        void on_actionNew_Member_triggered();
        void on_actionDelete_Member_triggered();

    private:
        Ui::EnumerationWindow *ui;

        EnumerationMemberWindow* enumerationMemberWindow;

        QStringList enumeration;

        bool validate();
};

#endif // ENUMERATIONWINDOW_H
