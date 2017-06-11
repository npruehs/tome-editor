#ifndef LABELEDPIXMAPWIDGET_H
#define LABELEDPIXMAPWIDGET_H

#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QWidget>

namespace Tome
{
    /**
     * @brief Widget that shows an image next to a short text.
     */
    class LabeledPixmapWidget : public QWidget
    {
            Q_OBJECT

        public:
            /**
             * @brief Constructs a new widget that shows an image next to a short text.
             * @param parent Optional owner widget.
             */
            explicit LabeledPixmapWidget(QWidget* parent = 0);
            ~LabeledPixmapWidget();

            /**
             * @brief Gets the label for showing the text.
             * @return Label for showing the text.
             */
            QLabel& getTextLabel();

            /**
             * @brief Gets the label for showing the image.
             * @return Label for showing the image.
             */
            QLabel& getPixmapLabel();

            /**
             * @brief Sets the image to show.
             * @param pixmap Image to show.
             */
            void setPixmap(const QPixmap& pixmap);

            /**
             * @brief Sets the text to show.
             * @param text Text to show.
             */
            void setText(const QString& text);

        private:
            static const int PixmapSize;

            QHBoxLayout* layout;

            QLabel* pixmapLabel;
            QLabel* textLabel;
    };
}


#endif // LABELEDPIXMAPWIDGET_H
