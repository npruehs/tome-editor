// --------------------------------------------------------------------------------------------------------------------
// <copyright file="MainWindow.xaml.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Core.Windows
{
    using System;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using System.IO;
    using System.Linq;
    using System.Windows;
    using System.Windows.Input;

    using Microsoft.Win32;

    using Tome.Fields.Windows;
    using Tome.Help.Windows;
    using Tome.Model.Fields;
    using Tome.Model.Project;
    using Tome.Model.Records;
    using Tome.Project.Windows;
    using Tome.Records.ViewModels;
    using Tome.Records.Windows;
    using Tome.Util;

    public partial class MainWindow : Window
    {
        #region Fields

        private AboutWindow aboutWindow;

        private TomeProjectFile currentProject;

        private Record editedRecord;

        private EditRecordWindow editRecordWindow;

        private FieldDefinitionsWindow fieldDefinitionsWindow;

        private NewProjectWindow newProjectWindow;

        #endregion

        #region Constructors and Destructors

        public MainWindow()
        {
            this.InitializeComponent();

            this.DataContext = this;
        }

        #endregion

        #region Properties

        public RecordsViewModel RecordsViewModel { get; private set; }

        public ObservableCollection<RecordFieldViewModel> SelectedRecordFields { get; } =
            new ObservableCollection<RecordFieldViewModel>();

        private bool ProjectLoaded
        {
            get
            {
                return this.currentProject != null;
            }
        }

        private bool RecordSelected
        {
            get
            {
                return this.SelectedRecord != null;
            }
        }

        private Record SelectedRecord
        {
            get
            {
                return this.RecordsTreeView.SelectedItem as Record;
            }
        }

        private string TomeProjectFileFilter
        {
            get
            {
                return string.Format("Tome Project Files (*{0})|*{0}", TomeProjectFile.FileExtension);
            }
        }

        #endregion

        #region Methods

        private void CanExecuteAddRecord(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = this.ProjectLoaded;
        }

        private void CanExecuteClose(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void CanExecuteDeleteRecord(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = this.RecordSelected;
        }

        private void CanExecuteEditRecord(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = this.RecordSelected;
        }

        private void CanExecuteFieldDefinitions(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = this.ProjectLoaded;
        }

        private void CanExecuteHelp(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void CanExecuteNew(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void CanExecuteOpen(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void CanExecuteSave(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = this.ProjectLoaded;
        }

        private void EditSelectedRecordDefinition()
        {
            if (!this.RecordSelected)
            {
                return;
            }

            // Show window.
            this.editRecordWindow = WindowUtils.ShowWindow(this.editRecordWindow, this, this.OnEditRecordWindowClosed);

            // Set edit mode.
            this.editedRecord = this.SelectedRecord;

            // Fill view model.
            var viewModel = this.editRecordWindow.RecordViewModel;
            viewModel.DisplayName = this.SelectedRecord.DisplayName;
            viewModel.Id = this.SelectedRecord.Id;
            viewModel.File =
                this.RecordsViewModel.RecordFiles.FirstOrDefault(file => file.Records.Contains(this.SelectedRecord));

            foreach (
                var field in this.currentProject.Project.FieldDefinitionFiles.SelectMany(file => file.FieldDefinitions))
            {
                var fieldViewModel = new RecordFieldViewModel();
                fieldViewModel.FieldId = field.Id;
                fieldViewModel.Enabled = this.SelectedRecord.FieldValues.ContainsKey(field.Id);
                viewModel.RecordFields.Add(fieldViewModel);
            }

            // Set available record files.
            this.editRecordWindow.SetRecordFiles(this.RecordsViewModel.RecordFiles);
            this.editRecordWindow.ComboBoxFile.IsEnabled = false;

            // Enforce unique field ids.
            this.editRecordWindow.SetExistingRecordIds(
                this.RecordsViewModel.Records.Select(existingRecord => existingRecord.Id)
                    .Where(recordId => !Equals(recordId, this.SelectedRecord.Id)));
        }

        private void ExecutedAddRecord(object target, ExecutedRoutedEventArgs e)
        {
            // Show window.
            this.editRecordWindow = WindowUtils.ShowWindow(this.editRecordWindow, this, this.OnEditRecordWindowClosed);

            // Set edit mode.
            this.editedRecord = null;

            // Fill view model.
            var viewModel = this.editRecordWindow.RecordViewModel;

            viewModel.File = this.RecordsViewModel.RecordFiles[0];

            foreach (
                var field in this.currentProject.Project.FieldDefinitionFiles.SelectMany(file => file.FieldDefinitions))
            {
                var fieldViewModel = new RecordFieldViewModel();
                fieldViewModel.FieldId = field.Id;
                viewModel.RecordFields.Add(fieldViewModel);
            }

            // Set available record definition files.
            this.editRecordWindow.SetRecordFiles(this.RecordsViewModel.RecordFiles);

            // Enforce unique record ids.
            this.editRecordWindow.SetExistingRecordIds(this.RecordsViewModel.Records.Select(record => record.Id));
        }

        private void ExecutedClose(object target, ExecutedRoutedEventArgs e)
        {
            this.Close();
        }

        private void ExecutedDeleteRecord(object target, ExecutedRoutedEventArgs e)
        {
            if (!this.RecordSelected)
            {
                return;
            }

            // Remove record.
            foreach (var recordFile in this.RecordsViewModel.RecordFiles)
            {
                recordFile.Records.Remove(this.SelectedRecord);
            }
        }

        private void ExecutedEditRecord(object target, ExecutedRoutedEventArgs e)
        {
            this.EditSelectedRecordDefinition();
        }

        private void ExecutedFieldDefinitions(object target, ExecutedRoutedEventArgs e)
        {
            this.fieldDefinitionsWindow = WindowUtils.ShowWindow(
                this.fieldDefinitionsWindow,
                this,
                this.OnFieldDefinitionsWindowClosed);
            this.fieldDefinitionsWindow.SetFieldDefinitions(this.currentProject.Project.FieldDefinitionFiles);
        }

        private void ExecutedHelp(object target, ExecutedRoutedEventArgs e)
        {
            this.aboutWindow = WindowUtils.ShowWindow(this.aboutWindow, this);
        }

        private void ExecutedNew(object target, ExecutedRoutedEventArgs e)
        {
            this.newProjectWindow = WindowUtils.ShowWindow(this.newProjectWindow, this, this.OnNewProjectWindowClosed);
        }

        private void ExecutedOpen(object target, ExecutedRoutedEventArgs e)
        {
            // Show file dialog.
            var openFileDialog = new OpenFileDialog
            {
                CheckFileExists = true,
                CheckPathExists = true,
                Filter = this.TomeProjectFileFilter
            };

            var result = openFileDialog.ShowDialog(this);

            if (result != true)
            {
                return;
            }

            // Read project files.
            var serializer = new TomeProjectFileSerializer();
            var loadedProjectFile = serializer.Deserialize(openFileDialog.FileName);
            this.currentProject = loadedProjectFile;

            // Setup records.
            this.RecordsViewModel = new RecordsViewModel(this.currentProject.Project.RecordFiles);
            this.RecordsTreeView.ItemsSource = this.RecordsViewModel.RecordFiles;
        }

        private void ExecutedSave(object target, ExecutedRoutedEventArgs e)
        {
            // Write project files.
            var serializer = new TomeProjectFileSerializer();
            serializer.Serialize(this.currentProject);
        }

        private FieldDefinition GetFieldDefinition(string fieldId)
        {
            return
                this.currentProject.Project.FieldDefinitionFiles.SelectMany(file => file.FieldDefinitions)
                    .FirstOrDefault(field => Equals(field.Id, fieldId));
        }

        private void OnEditRecordWindowClosed(object sender, EventArgs e)
        {
            this.Focus();

            if (!this.editRecordWindow.Result)
            {
                return;
            }

            var viewModel = this.editRecordWindow.RecordViewModel;
            var record = this.editedRecord;

            if (record == null)
            {
                // Add new record.
                record = new Record { DisplayName = viewModel.DisplayName, Id = viewModel.Id };

                foreach (var field in viewModel.RecordFields.Where(f => f.Enabled))
                {
                    var fieldDefinition = this.GetFieldDefinition(field.FieldId);
                    record.FieldValues.Add(field.FieldId, fieldDefinition.DefaultValue);
                }

                viewModel.File.Records.Add(record);
            }
            else

            {
                // Edit existing record.
                record.DisplayName = viewModel.DisplayName;
                record.Id = viewModel.Id;

                foreach (var field in viewModel.RecordFields)
                {
                    if (field.Enabled && !record.FieldValues.ContainsKey(field.FieldId))
                    {
                        var fieldDefinition = this.GetFieldDefinition(field.FieldId);
                        record.FieldValues.Add(field.FieldId, fieldDefinition.DefaultValue);
                    }
                    else if (!field.Enabled && record.FieldValues.ContainsKey(field.FieldId))
                    {
                        record.FieldValues.Remove(field.FieldId);
                    }
                }
            }

            // Update view.
            this.RecordsTreeView.Items.Refresh();
            ControlUtils.ExpandAndSelectItem(this.RecordsTreeView, record);
        }

        private void OnFieldDefinitionsWindowClosed(object sender, EventArgs e)
        {
            this.Focus();
        }

        private void OnMouseDoubleClickGrid(object sender, MouseButtonEventArgs args)
        {
        }

        private void OnMouseDoubleClickTreeView(object sender, MouseButtonEventArgs args)
        {
            this.EditSelectedRecordDefinition();
        }

        private void OnNewProjectWindowClosed(object sender, EventArgs e)
        {
            this.newProjectWindow.Closed -= this.OnNewProjectWindowClosed;

            if (!this.newProjectWindow.Result)
            {
                return;
            }

            // Focus main window again.
            this.Focus();

            try
            {
                // Create new project.
                var newProject = new TomeProject(this.newProjectWindow.NewProjectViewModel.Name);
                var newProjectFile = new TomeProjectFile
                {
                    Path =
                        Path.Combine(
                            this.newProjectWindow.NewProjectViewModel.Path,
                            newProject.Name + TomeProjectFile.FileExtension),
                    Project = newProject
                };

                // Write project files.
                var serializer = new TomeProjectFileSerializer();
                serializer.Serialize(newProjectFile);

                // Set current project.
                this.currentProject = newProjectFile;
            }
            catch (ArgumentNullException exception)
            {
                WindowUtils.ShowErrorMessage("Error creating project", exception.Message);
            }
        }

        private void OnSelectedRecordChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            var record = e.NewValue as Record;

            this.SelectedRecordFields.Clear();

            if (record != null)
            {
                var fields = new List<RecordFieldViewModel>();

                foreach (var field in record.FieldValues)
                {
                    fields.Add(new RecordFieldViewModel { FieldId = field.Key, FieldValue = field.Value });
                }

                foreach (var field in fields.OrderBy(field => field.FieldId))
                {
                    this.SelectedRecordFields.Add(field);
                }
            }
        }

        #endregion
    }
}