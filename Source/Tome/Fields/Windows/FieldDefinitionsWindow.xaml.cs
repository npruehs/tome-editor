// --------------------------------------------------------------------------------------------------------------------
// <copyright file="FieldDefinitionsWindow.xaml.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Fields.Windows
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Windows;
    using System.Windows.Input;

    using Tome.Fields.ViewModels;
    using Tome.Model.Fields;
    using Tome.Util;

    public partial class FieldDefinitionsWindow : Window
    {
        #region Fields

        private FieldDefinition editedFieldDefinition;

        private EditFieldDefinitionWindow editFieldDefinitionWindow;

        #endregion

        #region Constructors and Destructors

        public FieldDefinitionsWindow()
        {
            this.InitializeComponent();
        }

        #endregion

        #region Properties

        public FieldDefinitionsViewModel FieldDefinitionsViewModel { get; private set; }

        private bool FieldDefinitionSelected
        {
            get
            {
                return this.FieldGrid.SelectedItem != null;
            }
        }

        #endregion

        #region Public Methods and Operators

        public void SetFieldDefinitions(List<FieldDefinitionFile> fieldDefinitionFiles)
        {
            this.FieldDefinitionsViewModel = new FieldDefinitionsViewModel(fieldDefinitionFiles);
            this.DataContext = this.FieldDefinitionsViewModel;
        }

        #endregion

        #region Methods

        private void CanExecuteClose(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void CanExecuteDelete(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void CanExecuteEdit(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = this.FieldDefinitionSelected;
        }

        private void CanExecuteNew(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void EditSelectedFieldDefinition()
        {
            if (!this.FieldDefinitionSelected)
            {
                return;
            }

            var field = (FieldDefinition)this.FieldGrid.SelectedItem;

            // Show window.
            this.editFieldDefinitionWindow = WindowUtils.ShowWindow(
                this.editFieldDefinitionWindow,
                this,
                this.OnEditFieldDefinitionWindowClosed);

            // Set edit mode.
            this.editedFieldDefinition = field;

            // Fill view model.
            var viewModel = this.editFieldDefinitionWindow.FieldDefinitionViewModel;
            viewModel.DisplayName = field.DisplayName;
            viewModel.Id = field.Id;
            viewModel.FieldType = field.FieldType;
            viewModel.DefaultValue = field.DefaultValue;
            viewModel.Description = field.Description;
            viewModel.File =
                this.FieldDefinitionsViewModel.FieldDefinitionFiles.FirstOrDefault(
                    file => file.FieldDefinitions.Contains(field));
            viewModel.ExistingFieldIds =
                this.FieldDefinitionsViewModel.FieldDefinitions.Select(existingField => existingField.Id)
                    .Where(fieldId => !Equals(fieldId, field.Id));

            // Set available field definition files.
            this.editFieldDefinitionWindow.SetFieldDefinitionFiles(this.FieldDefinitionsViewModel.FieldDefinitionFiles);
            this.editFieldDefinitionWindow.ComboBoxFile.IsEnabled = false;
        }

        private void ExecutedClose(object target, ExecutedRoutedEventArgs e)
        {
            this.Close();
        }

        private void ExecutedDelete(object target, ExecutedRoutedEventArgs e)
        {
            if (!this.FieldDefinitionSelected)
            {
                return;
            }

            var field = (FieldDefinition)this.FieldGrid.SelectedItem;

            // Remove field.
            foreach (var fieldDefinitionFile in this.FieldDefinitionsViewModel.FieldDefinitionFiles)
            {
                fieldDefinitionFile.FieldDefinitions.Remove(field);
            }
        }

        private void ExecutedEdit(object target, ExecutedRoutedEventArgs e)
        {
            this.EditSelectedFieldDefinition();
        }

        private void ExecutedNew(object target, ExecutedRoutedEventArgs e)
        {
            // Show window.
            this.editFieldDefinitionWindow = WindowUtils.ShowWindow(
                this.editFieldDefinitionWindow,
                this,
                this.OnEditFieldDefinitionWindowClosed);

            // Set edit mode.
            this.editedFieldDefinition = null;

            // Fill view model.
            var viewModel = this.editFieldDefinitionWindow.FieldDefinitionViewModel;
            viewModel.FieldType = FieldType.Int;
            viewModel.File = this.FieldDefinitionsViewModel.FieldDefinitionFiles[0];
            viewModel.ExistingFieldIds =
                this.FieldDefinitionsViewModel.FieldDefinitions.Select(field => field.Id);

            // Set available field definition files.
            this.editFieldDefinitionWindow.SetFieldDefinitionFiles(this.FieldDefinitionsViewModel.FieldDefinitionFiles);
        }

        private void OnEditFieldDefinitionWindowClosed(object sender, EventArgs e)
        {
            this.Focus();

            if (!this.editFieldDefinitionWindow.Result)
            {
                return;
            }

            var viewModel = this.editFieldDefinitionWindow.FieldDefinitionViewModel;
            var field = this.editedFieldDefinition;

            if (field == null)
            {
                var newFieldDefinition = new FieldDefinition
                {
                    DisplayName = viewModel.DisplayName,
                    Id = viewModel.Id,
                    FieldType = viewModel.FieldType,
                    DefaultValue = viewModel.DefaultValue,
                    Description = viewModel.Description
                };

                viewModel.File.FieldDefinitions.Add(newFieldDefinition);
            }
            else
            {
                field.DisplayName = viewModel.DisplayName;
                field.Id = viewModel.Id;
                field.FieldType = viewModel.FieldType;
                field.DefaultValue = viewModel.DefaultValue;
                field.Description = viewModel.Description;
            }

            this.FieldGrid.Items.Refresh();
        }

        private void OnMouseDoubleClickGrid(object sender, MouseButtonEventArgs args)
        {
            this.EditSelectedFieldDefinition();
        }

        #endregion
    }
}