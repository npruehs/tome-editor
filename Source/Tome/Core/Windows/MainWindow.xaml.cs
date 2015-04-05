// --------------------------------------------------------------------------------------------------------------------
// <copyright file="MainWindow.xaml.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Core.Windows
{
    using System;
    using System.Collections.Generic;
    using System.IO;
    using System.Windows;
    using System.Windows.Input;

    using Microsoft.Win32;

    using Tome.Help.Windows;
    using Tome.Model.Fields;
    using Tome.Model.Project;
    using Tome.Model.Records;
    using Tome.Project.Windows;

    public partial class MainWindow : Window
    {
        #region Fields

        private AboutWindow aboutWindow;

        private TomeProjectFile currentProject;

        private NewProjectWindow newProjectWindow;

        #endregion

        #region Constructors and Destructors

        public MainWindow()
        {
            this.InitializeComponent();
        }

        #endregion

        #region Properties

        private string TomeProjectFileFilter
        {
            get
            {
                return string.Format("Tome Project Files (*{0})|*{0}", TomeProjectFile.FileExtension);
            }
        }

        #endregion

        #region Methods

        private void CanExecuteClose(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
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
            e.CanExecute = this.currentProject != null;
        }

        private void ExecutedClose(object target, ExecutedRoutedEventArgs e)
        {
            this.Close();
        }

        private void ExecutedHelp(object target, ExecutedRoutedEventArgs e)
        {
            this.aboutWindow = this.ShowWindow(this.aboutWindow);
        }

        private void ExecutedNew(object target, ExecutedRoutedEventArgs e)
        {
            this.newProjectWindow = this.ShowWindow(this.newProjectWindow, this.OnNewProjectWindowClosed);
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
        }

        private void ExecutedSave(object target, ExecutedRoutedEventArgs e)
        {
            // Write project files.
            var serializer = new TomeProjectFileSerializer();
            serializer.Serialize(this.currentProject);
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

                // TODO(np): Remove - Add dummy data.
                newProject.FieldDefinitionFiles[0].FieldDefinitions.Add(
                    new FieldDefinition
                    {
                        DefaultValue = 33.0f,
                        Description = "Increase of speed per second",
                        DisplayName = "Acceleration",
                        FieldType = FieldType.None,
                        Id = "acceleration"
                    });
                newProject.RecordFiles[0].Records.Add(
                    new Record
                    {
                        Id = "Fighter",
                        FieldValues = new Dictionary<string, object> { { "acceleration", "46.0f" } }
                    });

                // Write project files.
                var serializer = new TomeProjectFileSerializer();
                serializer.Serialize(newProjectFile);

                // Set current project.
                this.currentProject = newProjectFile;
            }
            catch (ArgumentNullException exception)
            {
                this.ShowErrorMessage("Error creating project", exception.Message);
            }
        }

        private void ShowErrorMessage(string title, string error)
        {
            MessageBox.Show(error, title, MessageBoxButton.OK, MessageBoxImage.Error, MessageBoxResult.OK);
        }

        private T ShowWindow<T>(T currentWindow) where T : Window, new()
        {
            return this.ShowWindow(currentWindow, null);
        }

        private T ShowWindow<T>(T currentWindow, EventHandler onClosed) where T : Window, new()
        {
            if (currentWindow == null || !currentWindow.IsLoaded)
            {
                currentWindow = new T { Owner = this, ShowInTaskbar = false };

                if (onClosed != null)
                {
                    currentWindow.Closed += onClosed;
                }
            }

            currentWindow.Show();
            currentWindow.Focus();
            return currentWindow;
        }

        #endregion
    }
}