#ifndef FILEPICKERWIDGET_H
#define FILEPICKERWIDGET_H

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QVariant>
#include <QWidget>


namespace Tome
{
    class FilePickerWidget : public QWidget
    {
            Q_OBJECT

        public:
            explicit FilePickerWidget(QWidget* parent = 0);
            ~FilePickerWidget();

            QVariant getFileName() const;
            void setFileName(const QVariant& v);

        private slots:
            void onBrowseButtonClicked(bool checked);

        private:
            QHBoxLayout* layout;

            QLineEdit* lineEdit;
            QPushButton* button;
    };
}

#endif // FILEPICKERWIDGET_H
