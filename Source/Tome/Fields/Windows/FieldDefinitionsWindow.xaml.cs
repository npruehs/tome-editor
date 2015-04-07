// --------------------------------------------------------------------------------------------------------------------
// <copyright file="FieldDefinitionsWindow.xaml.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Fields.Windows
{
    using System;
    using System.Collections.Generic;
    using System.Windows;
    using System.Windows.Input;

    using Tome.Fields.ViewModels;
    using Tome.Model.Fields;
    using Tome.Util;

    /// <summary>
    ///   Interaction logic for FieldDefinitionsWindow.xaml
    /// </summary>
    public partial class FieldDefinitionsWindow : Window
    {
        #region Fields

        private EditFieldDefinitionWindow editFieldDefinitionWindow;

        private List<FieldDefinitionFile> fieldDefinitionFiles;

        #endregion

        #region Constructors and Destructors

        public FieldDefinitionsWindow()
        {
            this.InitializeComponent();

            this.FieldDefinitionsViewModel = new FieldDefinitionsViewModel();
            this.DataContext = this.FieldDefinitionsViewModel;
        }

        #endregion

        #region Properties

        public FieldDefinitionsViewModel FieldDefinitionsViewModel { get; }

        #endregion

        #region Public Methods and Operators

        public void SetFieldDefinitions(List<FieldDefinitionFile> fieldDefinitionFiles)
        {
            this.fieldDefinitionFiles = fieldDefinitionFiles;

            this.FieldDefinitionsViewModel.FieldDefinitions.Clear();

            foreach (var fieldDefinitionFile in fieldDefinitionFiles)
            {
                foreach (var fieldDefinition in fieldDefinitionFile.FieldDefinitions)
                {
                    this.FieldDefinitionsViewModel.FieldDefinitions.Add(fieldDefinition);
                }
            }
        }

        #endregion

        #region Methods

        private void CanExecuteClose(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void CanExecuteNew(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void ExecutedClose(object target, ExecutedRoutedEventArgs e)
        {
            this.Close();
        }

        private void ExecutedNew(object target, ExecutedRoutedEventArgs e)
        {
            // Show window.
            this.editFieldDefinitionWindow = WindowUtils.ShowWindow(
                this.editFieldDefinitionWindow,
                this,
                this.OnEditFieldDefinitionWindowClosed);

            // Set edit mode.
            this.editFieldDefinitionWindow.EditMode = EditFieldDefinitionWindow.FieldDefinitionEditMode.Add;
        }

        private void OnEditFieldDefinitionWindowClosed(object sender, EventArgs e)
        {
            this.Focus();

            if (!this.editFieldDefinitionWindow.Result)
            {
                return;
            }

            var viewModel = this.editFieldDefinitionWindow.FieldDefinitionViewModel;

            var newFieldDefinition = new FieldDefinition
            {
                DisplayName = viewModel.DisplayName,
                Id = viewModel.Id,
                FieldType = viewModel.FieldType,
                DefaultValue = viewModel.DefaultValue,
                Description = viewModel.Description
            };

            this.FieldDefinitionsViewModel.FieldDefinitions.Add(newFieldDefinition);
            this.fieldDefinitionFiles[0].FieldDefinitions.Add(newFieldDefinition);
        }

        private void OnMouseDoubleClickGrid(object sender, MouseButtonEventArgs args)
        {
            if (this.FieldGrid.SelectedItem != null)
            {
                var field = (FieldDefinition)this.FieldGrid.SelectedItem;
            }
        }

        #endregion
    }
}