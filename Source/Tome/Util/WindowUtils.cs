// --------------------------------------------------------------------------------------------------------------------
// <copyright file="WindowUtils.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Util
{
    using System;
    using System.Windows;

    public static class WindowUtils
    {
        #region Public Methods and Operators

        /// <summary>
        ///   Shows a modal error message with the specified title and content.
        /// </summary>
        /// <param name="title">Title of the error message to show.</param>
        /// <param name="error">Content of the error message to show.</param>
        public static void ShowErrorMessage(string title, string error)
        {
            MessageBox.Show(error, title, MessageBoxButton.OK, MessageBoxImage.Error, MessageBoxResult.OK);
        }

        /// <summary>
        ///   Shows a window of the specified type. If the window has never
        ///   been created or has already been closed, a new one is created.
        ///   The window is shown and focused immediately.
        /// </summary>
        /// <typeparam name="T">Type of the window to show.</typeparam>
        /// <param name="currentWindow">Exisiting window of the specified type.</param>
        /// <param name="owner">Owner for the new window.</param>
        /// <returns>Existing or new window of the specified type.</returns>
        public static T ShowWindow<T>(T currentWindow, Window owner) where T : Window, new()
        {
            return ShowWindow(currentWindow, owner, null);
        }

        /// <summary>
        ///   Shows a window of the specified type. If the window has never
        ///   been created or has already been closed, a new one is created.
        ///   The window is shown and focused immediately.
        /// </summary>
        /// <typeparam name="T">Type of the window to show.</typeparam>
        /// <param name="currentWindow">Exisiting window of the specified type.</param>
        /// <param name="owner">Owner for the new window.</param>
        /// <param name="onClosed">Close callback for the new window.</param>
        /// <returns>Existing or new window of the specified type.</returns>
        public static T ShowWindow<T>(T currentWindow, Window owner, EventHandler onClosed) where T : Window, new()
        {
            if (currentWindow == null || !currentWindow.IsLoaded)
            {
                currentWindow = new T { Owner = owner, ShowInTaskbar = false };

                if (onClosed != null)
                {
                    currentWindow.Closed += onClosed;
                }
            }

            currentWindow.Show();
            currentWindow.Focus();
            return currentWindow;
        }

        #endregion
    }
}