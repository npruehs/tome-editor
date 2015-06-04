// --------------------------------------------------------------------------------------------------------------------
// <copyright file="MainWindow.xaml.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Core.Windows
{
    using System;
    using System.IO;
    using System.Linq;
    using System.Windows;
    using System.Windows.Input;

    using Microsoft.Win32;

    using Tome.Fields.Windows;
    using Tome.Help.Windows;
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
        }

        #endregion

        #region Properties

        public RecordsViewModel RecordsViewModel { get; private set; }

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
                return this.RecordsTreeView.SelectedItem != null;
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

            var record = (Record)this.RecordsTreeView.SelectedItem;

            // Show window.
            this.editRecordWindow = WindowUtils.ShowWindow(this.editRecordWindow, this, this.OnEditRecordWindowClosed);

            // Set edit mode.
            this.editedRecord = record;

            // Fill view model.
            var viewModel = this.editRecordWindow.RecordViewModel;
            viewModel.DisplayName = record.DisplayName;
            viewModel.Id = record.Id;
            viewModel.File = this.RecordsViewModel.RecordFiles.FirstOrDefault(file => file.Records.Contains(record));

            // Set available field definition files.
            this.editRecordWindow.SetRecordFiles(this.RecordsViewModel.RecordFiles);
            this.editRecordWindow.ComboBoxFile.IsEnabled = false;

            // Enforce unique field ids.
            this.editRecordWindow.SetExistingRecordIds(
                this.RecordsViewModel.Records.Select(existingRecord => existingRecord.Id)
                    .Where(recordId => !Equals(recordId, record.Id)));
        }

        private void ExecutedAddRecord(object target, ExecutedRoutedEventArgs e)
        {
            // Show window.
            this.editRecordWindow = WindowUtils.ShowWindow(this.editRecordWindow, this, this.OnEditRecordWindowClosed);

            // Set edit mode.
            this.editedRecord = null;

            // Fill view model.
            this.editRecordWindow.RecordViewModel.File = this.RecordsViewModel.RecordFiles[0];

            // Set available record definition files.
            this.editRecordWindow.SetRecordFiles(this.RecordsViewModel.RecordFiles);

            // Enforce unique record ids.
            this.editRecordWindow.SetExistingRecordIds(this.RecordsViewModel.Records.Select(record => record.Id));
        }

        private void ExecutedClose(object target, ExecutedRoutedEventArgs e)
        {
            this.Close();
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
                var newRecord = new Record { DisplayName = viewModel.DisplayName, Id = viewModel.Id };

                viewModel.File.Records.Add(newRecord);
            }
            else

            {
                // Edit existing record.
                record.DisplayName = viewModel.DisplayName;
                record.Id = viewModel.Id;
            }

            // Update view.
            this.RecordsTreeView.Items.Refresh();
            ControlUtils.ExpandAndSelectItem(this.RecordsTreeView, record);
        }

        private void OnFieldDefinitionsWindowClosed(object sender, EventArgs e)
        {
            this.Focus();
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

        #endregion
    }
}