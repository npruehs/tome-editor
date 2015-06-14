// --------------------------------------------------------------------------------------------------------------------
// <copyright file="EditFieldDefinitionWindow.xaml.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Fields.Windows
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Windows;
    using System.Windows.Controls;
    using System.Windows.Data;
    using System.Windows.Media;

    using Tome.Core.Conversion;
    using Tome.Core.Validation;
    using Tome.Fields.ViewModels;
    using Tome.Model.Fields;
    using Tome.Util;

    /// <summary>
    ///   Interaction logic for EditFieldDefinitionWindow.xaml
    /// </summary>
    public partial class EditFieldDefinitionWindow : Window
    {
        #region Constructors and Destructors

        public EditFieldDefinitionWindow()
        {
            this.InitializeComponent();

            this.FieldDefinitionViewModel = new FieldDefinitionViewModel();
            this.DataContext = this.FieldDefinitionViewModel;

            foreach (var fieldType in Enum.GetValues(typeof(FieldType)))
            {
                if (!Equals(fieldType, FieldType.None))
                {
                    this.ComboBoxFieldType.Items.Add(fieldType);
                }
            }
        }

        #endregion

        #region Properties

        public FieldDefinitionViewModel FieldDefinitionViewModel { get; private set; }

        public bool Result { get; set; }

        #endregion

        #region Public Methods and Operators

        public void SetExistingFieldIds(IEnumerable<string> existingFieldIds)
        {
            var bindingExpression = this.TextBoxId.GetBindingExpression(TextBox.TextProperty);
            var binding = bindingExpression.ParentBinding;

            binding.ValidationRules.Clear();
            binding.ValidationRules.Add(new StringNotEmptyValidationRule());
            binding.ValidationRules.Add(new UniqueIdValidationRule { ExistingIds = existingFieldIds });
        }

        public void SetFieldDefinitionFiles(List<FieldDefinitionFileViewModel> fieldDefinitionFiles)
        {
            this.ComboBoxFile.Items.Clear();

            foreach (var fieldDefinitionFile in fieldDefinitionFiles)
            {
                this.ComboBoxFile.Items.Add(fieldDefinitionFile);
            }
        }

        #endregion

        #region Methods

        private void OnClickCancel(object sender, RoutedEventArgs e)
        {
            this.Result = false;
            this.Close();
        }

        private void OnClickOk(object sender, RoutedEventArgs e)
        {
            // Validate data.
            var bindingExpression = this.TextBoxDisplayName.GetBindingExpression(TextBox.TextProperty);
            bindingExpression?.UpdateSource();

            bindingExpression = this.TextBoxId.GetBindingExpression(TextBox.TextProperty);
            bindingExpression?.UpdateSource();

            if (!ValidationUtils.IsValid(this))
            {
                return;
            }

            // Close window.
            this.Result = true;
            this.Close();
        }

        private void OnComboBoxFieldTypeChanged(object sender, RoutedEventArgs e)
        {
            var fieldType = (FieldType)this.ComboBoxFieldType.SelectedItem;

            // Convert current value to string.
            var currentDefaultValue = ConversionUtils.ToString(this.FieldDefinitionViewModel.DefaultValue);

            // Convert default value to new type.
            switch (fieldType)
            {
                case FieldType.Boolean:
                    this.FieldDefinitionViewModel.DefaultValue =
                        ConversionUtils.ConvertOrDefault<bool>(currentDefaultValue);
                    break;

                case FieldType.Color:
                    this.FieldDefinitionViewModel.DefaultValue =
                        ConversionUtils.ConvertOrDefault<Color>(currentDefaultValue);
                    break;

                case FieldType.Float:
                    this.FieldDefinitionViewModel.DefaultValue =
                        ConversionUtils.ConvertOrDefault<float>(currentDefaultValue);
                    break;

                case FieldType.Int:
                    this.FieldDefinitionViewModel.DefaultValue =
                        ConversionUtils.ConvertOrDefault<int>(currentDefaultValue);
                    break;

                case FieldType.Reference:
                    this.FieldDefinitionViewModel.DefaultValue = currentDefaultValue;
                    break;

                case FieldType.String:
                    this.FieldDefinitionViewModel.DefaultValue = currentDefaultValue;
                    break;
            }

            // Create control.
            var control = ControlUtils.CreateValueControl(this.FieldDefinitionViewModel, "DefaultValue", fieldType);
            control.Style = (Style)this.FindResource("ErrorLabelMargin");
            Validation.SetErrorTemplate(control, (ControlTemplate)this.FindResource("ErrorLabel"));

            // Add control to window.
            this.DockPanelDefaultValueUIElement.Children.Clear();
            this.DockPanelDefaultValueUIElement.Children.Add(control);
        }

        private void OnTextBoxDisplayNameChanged(object sender, RoutedEventArgs e)
        {
            this.FieldDefinitionViewModel.Id = StringUtils.ToPascalCase(this.TextBoxDisplayName.Text);
        }

        #endregion
    }
}