// --------------------------------------------------------------------------------------------------------------------
// <copyright file="ControlFactory.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Fields.Controls
{
    using System.Collections.Generic;
    using System.Windows.Controls;
    using System.Windows.Controls.Primitives;
    using System.Windows.Data;

    using Tome.Core.Conversion;
    using Tome.Core.Validation;
    using Tome.Model.Fields;

    using Xceed.Wpf.Toolkit;

    public class ControlFactory
    {
        #region Public Methods and Operators

        public static Control CreateControl(object viewModel, string bindingPath, FieldType fieldType)
        {
            return CreateControl(viewModel, bindingPath, fieldType, null);
        }

        public static Control CreateControl(object viewModel, string bindingPath, FieldType fieldType, object data)
        {
            Control control;

            // Create binding.
            var binding = new Binding(bindingPath);
            binding.Source = viewModel;

            // Create control.
            switch (fieldType)
            {
                case FieldType.Boolean:
                    control = new CheckBox();
                    control.SetBinding(ToggleButton.IsCheckedProperty, binding);
                    return control;

                case FieldType.Color:
                    control = new ColorCanvas();
                    control.SetBinding(ColorCanvas.SelectedColorProperty, binding);
                    return control;

                case FieldType.Float:
                    control = new TextBox();
                    binding.ValidationRules.Add(new CanConvertToTypeValidationRule(typeof(float)));
                    binding.Converter = new FieldValueConverter(typeof(float));
                    control.SetBinding(TextBox.TextProperty, binding);
                    return control;

                case FieldType.Int:
                    control = new TextBox();
                    binding.ValidationRules.Add(new CanConvertToTypeValidationRule(typeof(int)));
                    binding.Converter = new FieldValueConverter(typeof(int));
                    control.SetBinding(TextBox.TextProperty, binding);
                    return control;

                case FieldType.Reference:
                    control = new ComboBox();
                    control.SetBinding(Selector.SelectedItemProperty, binding);

                    if (data != null)
                    {
                        var comboBox = (ComboBox)control;
                        var options = (IEnumerable<string>)data;

                        // Allow resetting field value.
                        comboBox.Items.Add(string.Empty);

                        // Add all options.
                        foreach (var option in options)
                        {
                            comboBox.Items.Add(option);
                        }
                    }

                    return control;

                case FieldType.String:
                    control = new TextBox();
                    binding.ValidationRules.Add(new CanConvertToTypeValidationRule(typeof(string)));
                    binding.Converter = new FieldValueConverter(typeof(string));
                    control.SetBinding(TextBox.TextProperty, binding);
                    return control;

                case FieldType.Vector3F:
                    control = new Vector3FControl();
                    control.SetBinding(Vector3FControl.VectorProperty, binding);
                    return control;
            }

            return null;
        }

        #endregion
    }
}