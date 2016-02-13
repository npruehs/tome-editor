#ifndef MEMORYUTILS
#define MEMORYUTILS

#include <QLayout>

namespace Tome
{
    /**
     * @brief deleteLayout Deletes the passed layout and all of its children.
     * @param layout Layout to delete.
     */
    inline void deleteLayout(QLayout* layout)
    {
        QLayoutItem* child;

        while ((child = layout->takeAt(0)) != 0)
        {

            delete child;
        }

        delete layout;
    }
}

#endif // MEMORYUTILS

