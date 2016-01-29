#ifndef VECTORUTILS
#define VECTORUTILS

#include <QDir>
#include <QVector>

namespace Tome
{
    template<typename T>
    /**
     * @brief findInsertionIndex Assuming the passed vector is sorted, returns the index to insert the specified item at to keep the vector sorted.
     * @param vector Vector to add the item to.
     * @param item Item to add.
     * @return Index to insert the specified item at to keep the vector sorted.
     */
    inline int findInsertionIndex(const QVector<T> vector, T item)
    {
        for (int i = 0; i < vector.size() - 1; ++i)
        {
            if (vector.at(i) < item && vector.at(i + 1) > item)
            {
                return i + 1;
            }
        }

        return 0;
    }
}

#endif // VECTORUTILS

