#ifndef LABELEDPIXMAPWIDGET_H
#define LABELEDPIXMAPWIDGET_H

#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QWidget>

namespace Tome
{
    class LabeledPixmapWidget : public QWidget
    {
            Q_OBJECT

        public:
            explicit LabeledPixmapWidget(QWidget* parent = 0);
            ~LabeledPixmapWidget();

            QLabel& getTextLabel();
            QLabel& getPixmapLabel();

            void setPixmap(const QPixmap& pixmap);
            void setText(const QString& text);

        private:
            static const int PixmapSize;

            QHBoxLayout* layout;

            QLabel* pixmapLabel;
            QLabel* textLabel;
    };
}


#endif // LABELEDPIXMAPWIDGET_H
