// --------------------------------------------------------------------------------------------------------------------
// <copyright file="EditFieldDefinitionWindow.xaml.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Fields.Windows
{
    using System;
    using System.Collections.Generic;
    using System.Windows;
    using System.Windows.Controls;
    using System.Windows.Data;

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

        public FieldDefinitionViewModel FieldDefinitionViewModel { get; set; }

        public bool Result { get; set; }

        #endregion

        #region Public Methods and Operators

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
            var currentDefaultValue = this.FieldDefinitionViewModel.DefaultValue;

            // Convert default value to new type.
            switch (fieldType)
            {
                case FieldType.Int:
                    var newDefaultValue = 0;

                    if (currentDefaultValue != null)
                    {
                        int.TryParse(currentDefaultValue.ToString(), out newDefaultValue);
                    }

                    this.FieldDefinitionViewModel.DefaultValue = newDefaultValue;
                    break;

                case FieldType.String:
                    this.FieldDefinitionViewModel.DefaultValue = currentDefaultValue != null
                        ? currentDefaultValue.ToString()
                        : string.Empty;
                    break;
            }

            // Create control.
            var binding = new Binding("DefaultValue");
            binding.Source = this.FieldDefinitionViewModel;
            binding.ValidationRules.Add(new StringNotEmptyValidationRule());

            var textBox = new TextBox();
            textBox.SetBinding(TextBox.TextProperty, binding);
            textBox.Style = (Style)this.FindResource("ErrorLabelMargin");
            Validation.SetErrorTemplate(textBox, (ControlTemplate)this.FindResource("ErrorLabel"));

            this.DockPanelDefaultValueUIElement.Children.Clear();
            this.DockPanelDefaultValueUIElement.Children.Add(textBox);

            switch (fieldType)
            {
                case FieldType.Int:
                    binding.ValidationRules.Add(new CanConvertToTypeValidationRule(typeof(int)));
                    break;

                case FieldType.String:
                    binding.ValidationRules.Add(new CanConvertToTypeValidationRule(typeof(string)));
                    break;
            }
        }

        private void OnTextBoxDisplayNameChanged(object sender, RoutedEventArgs e)
        {
            this.FieldDefinitionViewModel.Id = StringUtils.ToPascalCase(this.TextBoxDisplayName.Text);
        }

        #endregion
    }
}