// --------------------------------------------------------------------------------------------------------------------
// <copyright file="ControlUtils.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Util
{
    using System.Windows.Controls;
    using System.Windows.Data;

    using Tome.Core.Conversion;
    using Tome.Core.Validation;
    using Tome.Model.Fields;

    public class ControlUtils
    {
        #region Public Methods and Operators

        public static TextBox CreateValueControl(object viewModel, string bindingPath, FieldType fieldType)
        {
            // Create binding.
            var binding = new Binding(bindingPath);
            binding.Source = viewModel;

            switch (fieldType)
            {
                case FieldType.Int:
                    binding.ValidationRules.Add(new CanConvertToTypeValidationRule(typeof(int)));
                    binding.Converter = new FieldValueConverter(typeof(int));
                    break;

                case FieldType.String:
                    binding.ValidationRules.Add(new CanConvertToTypeValidationRule(typeof(string)));
                    binding.Converter = new FieldValueConverter(typeof(string));
                    break;
            }

            // Create control.
            var textBox = new TextBox();
            textBox.SetBinding(TextBox.TextProperty, binding);
            return textBox;
        }

        /// <summary>
        ///   "It's a real pain for some strange reason."
        /// </summary>
        /// <see cref="http://stackoverflow.com/questions/413890/how-to-programmatically-select-an-item-in-a-wpf-treeview" />
        /// <see cref="http://blog.quantumbitdesigns.com/2008/07/22/programmatically-selecting-an-item-in-a-treeview/" />
        /// <see cref="http://alookonthecode.blogspot.de/2011/05/pogrammatically-selecting-items-in-wpf.html" />
        /// <param name="parentContainer">Container holding the item to select.</param>
        /// <param name="itemToSelect">Item to select and focus.</param>
        /// <returns></returns>
        public static bool ExpandAndSelectItem(ItemsControl parentContainer, object itemToSelect)
        {
            foreach (var item in parentContainer.Items)
            {
                // Check current container.
                var currentContainer = parentContainer.ItemContainerGenerator.ContainerFromItem(item) as TreeViewItem;

                if (item == itemToSelect && currentContainer != null)
                {
                    // Found it!
                    currentContainer.IsSelected = true;
                    currentContainer.BringIntoView();
                    currentContainer.Focus();
                    return true;
                }

                if (currentContainer != null && currentContainer.Items.Count > 0)
                {
                    // Force children expansion.
                    currentContainer.IsExpanded = true;
                    currentContainer.UpdateLayout();

                    // Check children.
                    if (ExpandAndSelectItem(currentContainer, itemToSelect))
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        #endregion
    }
}