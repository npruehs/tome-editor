// --------------------------------------------------------------------------------------------------------------------
// <copyright file="ValidationUtils.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Util
{
    using System.Windows;
    using System.Windows.Controls;
    using System.Windows.Input;

    public static class ValidationUtils
    {
        #region Public Methods and Operators

        public static bool IsValid(DependencyObject node)
        {
            if (node == null)
            {
                return true;
            }

            // Check if dependency object is valid. 
            // Validation.GetHasError works for controls that have validation rules attached.
            var isValid = !Validation.GetHasError(node);
            if (!isValid)
            {
                // If the dependency object is invalid, and it can receive the focus, set the focus.
                var inputElement = node as IInputElement;
                if (inputElement != null)
                {
                    Keyboard.Focus(inputElement);
                }

                return false;
            }

            // If the passed dependency object is valid, check all child dependency objects.
            foreach (var childNode in LogicalTreeHelper.GetChildren(node))
            {
                var dependencyObject = childNode as DependencyObject;
                if (dependencyObject != null)
                {
                    // If a child dependency object is invalid, return false immediately, otherwise keep checking.
                    if (!IsValid(dependencyObject))
                    {
                        return false;
                    }
                }
            }

            // All dependency objects are valid.
            return true;
        }

        #endregion
    }
}