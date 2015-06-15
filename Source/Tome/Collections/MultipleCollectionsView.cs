// --------------------------------------------------------------------------------------------------------------------
// <copyright file="MultipleCollectionsView.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Collections
{
    using System.Collections;
    using System.Collections.Generic;
    using System.Collections.Specialized;
    using System.Linq;

    /// <summary>
    ///   Provides an observable, flattened view of multiple collections, that
    ///   is handled like a single collection containing the items of all of
    ///   the underlying collections.
    /// </summary>
    /// <typeparam name="T">Type of the collection items.</typeparam>
    public class MultipleCollectionsView<T> : IEnumerable<T>, INotifyCollectionChanged
    {
        #region Fields

        private readonly List<IEnumerable<T>> collections = new List<IEnumerable<T>>();

        #endregion

        #region Events

        public event NotifyCollectionChangedEventHandler CollectionChanged;

        #endregion

        #region Public Methods and Operators

        public void AddCollection(IEnumerable<T> collection)
        {
            // Register listener.
            var observableCollection = collection as INotifyCollectionChanged;

            if (observableCollection != null)
            {
                observableCollection.CollectionChanged += this.OnCollectionChanged;
            }

            // Add to view.
            this.collections.Add(collection);
        }

        public void ClearCollections()
        {
            // Remove listeners.
            foreach (var collection in this.collections)
            {
                var observableCollection = collection as INotifyCollectionChanged;

                if (observableCollection != null)
                {
                    observableCollection.CollectionChanged -= this.OnCollectionChanged;
                }
            }

            // Remove from view.
            this.collections.Clear();
        }

        public IEnumerator<T> GetEnumerator()
        {
            return this.collections.SelectMany(collection => collection).GetEnumerator();
        }

        public void RemoveCollection(IEnumerable<T> collection)
        {
            // Remove listener.
            var observableCollection = collection as INotifyCollectionChanged;

            if (observableCollection != null)
            {
                observableCollection.CollectionChanged -= this.OnCollectionChanged;
            }

            // Remove from view.
            this.collections.Remove(collection);
        }

        #endregion

        #region Methods

        IEnumerator IEnumerable.GetEnumerator()
        {
            return this.GetEnumerator();
        }

        private void OnCollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            // Notify listeners.
            var handler = this.CollectionChanged;
            handler?.Invoke(this, e);
        }

        #endregion
    }
}