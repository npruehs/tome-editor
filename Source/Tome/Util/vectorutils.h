#ifndef VECTORUTILS
#define VECTORUTILS

#include <QSharedPointer>
#include <QVector>

namespace Tome
{
    template<typename T>
    /**
     * @brief findInsertionIndex Assuming the passed list is sorted, returns the index to insert the specified item at to keep the list sorted.
     * @param list List to add the item to.
     * @param item Item to add.
     * @return Index to insert the specified item at to keep the list sorted.
     */
    inline int findInsertionIndex(const QList<T> list, T item)
    {
        for (int i = 0; i < list.size() - 1; ++i)
        {
            if (list.at(i) < item && list.at(i + 1) >= item)
            {
                return i + 1;
            }
        }

        return list.size();
    }

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
            if (vector.at(i) < item && vector.at(i + 1) >= item)
            {
                return i + 1;
            }
        }

        return vector.size();
    }

    template<typename T>
    inline int findInsertionIndex(const QVector<T>& vector, T item, bool(*lessThan)(const T& first, const T& second))
    {
        for (int i = 0; i < vector.size() - 1; ++i)
        {
            if (lessThan(vector.at(i), item) && !lessThan(vector.at(i + 1), item))
            {
                return i + 1;
            }
        }

        return vector.size();
    }
}

#endif // VECTORUTILS

