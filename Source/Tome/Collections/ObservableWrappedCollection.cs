// --------------------------------------------------------------------------------------------------------------------
// <copyright file="ObservableWrappedCollection.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Collections
{
    using System;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using System.Collections.Specialized;

    /// <summary>
    ///   Decorates the <seealso cref="ICollection{T}.Add" />,
    ///   <seealso cref="ICollection{T}.Clear" /> and
    ///   <seealso cref="ICollection{T}.Remove" /> operations of a
    ///   <seealso cref="ICollection{T}" /> in order to notify interested
    ///   listeners.
    /// </summary>
    /// <remarks>
    ///   In contrast to <seealso cref="ObservableCollection{T}" />, this
    ///   collection does not copy all items of the underlying collection,
    ///   requiring updates of both collections, but holds a reference to it
    ///   instead. Thus, modifying this collection will modify the underlying
    ///   collection as well. However, note that modifying the underlying
    ///   collection directly won't raise the respective
    ///   <seealso cref="INotifyCollectionChanged" /> events.
    /// </remarks>
    /// <typeparam name="T">Type of the collection items.</typeparam>
    public class ObservableWrappedCollection<T> : CollectionDecorator<T>, INotifyCollectionChanged
    {
        #region Constructors and Destructors

        public ObservableWrappedCollection(ICollection<T> collection)
            : base(collection)
        {
        }

        #endregion

        #region Events

        public event NotifyCollectionChangedEventHandler CollectionChanged;

        #endregion

        #region Public Methods and Operators

        public override void Add(T item)
        {
            base.Add(item);

            var e = new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Add, new List<T> { item });
            this.OnCollectionChanged(e);
        }

        public override void Clear()
        {
            if (this.Count == 0)
            {
                return;
            }

            base.Clear();

            var e = new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset);
            this.OnCollectionChanged(e);
        }

        public override bool Remove(T item)
        {
            // Get index of removed item for CollectionChanged event.
            var itemIndex = this.Collection.IndexOf(item);

            // Remove item.
            var removed = base.Remove(item);

            if (!removed)
            {
                return false;
            }

            // Notify listeners.
            var e = new NotifyCollectionChangedEventArgs(
                NotifyCollectionChangedAction.Remove,
                new List<T> { item },
                itemIndex);
            this.OnCollectionChanged(e);

            return true;
        }

        public void Sort(Comparison<T> comparison)
        {
            if (this.Count == 0)
            {
                return;
            }

            // Sort underlying collection.
            var items = new List<T>(this.Collection);
            items.Sort(comparison);

            base.Clear();

            foreach (var item in items)
            {
                base.Add(item);
            }

            // Notify listeners.
            var e = new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset);
            this.OnCollectionChanged(e);
        }

        #endregion

        #region Methods

        private void OnCollectionChanged(NotifyCollectionChangedEventArgs e)
        {
            var handler = this.CollectionChanged;
            handler?.Invoke(this, e);
        }

        #endregion
    }
}