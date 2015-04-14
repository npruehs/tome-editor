// --------------------------------------------------------------------------------------------------------------------
// <copyright file="EnumerableExtensions.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Collections
{
    using System.Collections.Generic;
    using System.Linq;

    public static class EnumerableExtensions
    {
        #region Public Methods and Operators

        /// <summary>
        ///   Returns the index of the specified element in the sequence.
        /// </summary>
        /// <typeparam name="T">Type of the elements of the seqeunce.</typeparam>
        /// <param name="sequence">Sequence to get the index in.</param>
        /// <param name="element">Element to get the index of.</param>
        /// <returns></returns>
        public static int IndexOf<T>(this IEnumerable<T> sequence, T element)
        {
            return sequence.TakeWhile(e => !Equals(e, element)).Count();
        }

        #endregion
    }
}