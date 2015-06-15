// --------------------------------------------------------------------------------------------------------------------
// <copyright file="EditRecordWindow.xaml.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Records.Windows
{
    using System.Collections.Generic;
    using System.Windows;
    using System.Windows.Controls;

    using Tome.Core.Validation;
    using Tome.Records.ViewModels;
    using Tome.Util;

    /// <summary>
    ///   Interaction logic for EditRecordWindow.xaml
    /// </summary>
    public partial class EditRecordWindow : Window
    {
        #region Constructors and Destructors

        public EditRecordWindow()
        {
            this.InitializeComponent();

            this.RecordViewModel = new RecordViewModel();
            this.DataContext = this.RecordViewModel;
        }

        #endregion

        #region Properties

        public RecordViewModel RecordViewModel { get; set; }

        public bool Result { get; set; }

        #endregion

        #region Public Methods and Operators

        public void SetExistingRecordIds(IEnumerable<string> existingRecordIds)
        {
            var bindingExpression = this.TextBoxId.GetBindingExpression(TextBox.TextProperty);
            var binding = bindingExpression.ParentBinding;

            binding.ValidationRules.Clear();
            binding.ValidationRules.Add(new StringNotEmptyValidationRule());
            binding.ValidationRules.Add(new UniqueIdValidationRule { ExistingIds = existingRecordIds });
        }

        public void SetRecordFiles(List<RecordFileViewModel> recordFiles)
        {
            this.ComboBoxFile.Items.Clear();

            foreach (var fieldDefinitionFile in recordFiles)
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

        private void OnTextBoxDisplayNameChanged(object sender, RoutedEventArgs e)
        {
            this.RecordViewModel.Id = StringUtils.ToPascalCase(this.TextBoxDisplayName.Text);
        }

        #endregion
    }
}