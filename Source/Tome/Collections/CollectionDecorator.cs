// --------------------------------------------------------------------------------------------------------------------
// <copyright file="CollectionDecorator.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Collections
{
    using System.Collections;
    using System.Collections.Generic;

    /// <summary>
    ///   Decorates a single collection, allowing to modify any or all of its
    ///   operations.
    /// </summary>
    /// <typeparam name="T">Type of the collection items.</typeparam>
    public abstract class CollectionDecorator<T> : ICollection<T>
    {
        #region Fields

        protected ICollection<T> Collection;

        #endregion

        #region Constructors and Destructors

        protected CollectionDecorator(ICollection<T> collection)
        {
            this.Collection = collection;
        }

        #endregion

        #region Properties

        public virtual int Count => this.Collection.Count;

        public virtual bool IsReadOnly => this.Collection.IsReadOnly;

        #endregion

        #region Public Methods and Operators

        public virtual void Add(T item)
        {
            this.Collection.Add(item);
        }

        public virtual void Clear()
        {
            this.Collection.Clear();
        }

        public virtual bool Contains(T item)
        {
            return this.Collection.Contains(item);
        }

        public virtual void CopyTo(T[] array, int arrayIndex)
        {
            this.Collection.CopyTo(array, arrayIndex);
        }

        public virtual IEnumerator<T> GetEnumerator()
        {
            return this.Collection.GetEnumerator();
        }

        public virtual bool Remove(T item)
        {
            return this.Collection.Remove(item);
        }

        #endregion

        #region Methods

        IEnumerator IEnumerable.GetEnumerator()
        {
            return this.GetEnumerator();
        }

        #endregion
    }
}