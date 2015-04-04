// --------------------------------------------------------------------------------------------------------------------
// <copyright file="MainWindow.xaml.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Core.Windows
{
    using System;
    using System.Windows;
    using System.Windows.Input;

    using Tome.Help.Windows;
    using Tome.Model.Project;
    using Tome.Project.Windows;

    public partial class MainWindow : Window
    {
        #region Fields

        private TomeProject currentProject;

        private NewProjectWindow newProjectWindow;

        #endregion

        #region Constructors and Destructors

        public MainWindow()
        {
            this.InitializeComponent();
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

        private void ExecutedClose(object target, ExecutedRoutedEventArgs e)
        {
            this.Close();
        }

        private void ExecutedHelp(object target, ExecutedRoutedEventArgs e)
        {
            // Create and show about window.
            new AboutWindow { Owner = this }.Show();
        }

        private void ExecutedNew(object target, ExecutedRoutedEventArgs e)
        {
            this.newProjectWindow = new NewProjectWindow { Owner = this };
            this.newProjectWindow.Closed += this.OnNewProjectWindowClosed;
            this.newProjectWindow.Show();
        }

        private void OnNewProjectWindowClosed(object sender, EventArgs e)
        {
            try
            {
                var newProject = this.newProjectWindow.TomeProject;
                newProject.InitProject();
                this.currentProject = newProject;
            }
            catch (InvalidOperationException exception)
            {
                this.ShowErrorMessage("Error creating project", exception.Message);
            }
        }

        private void ShowErrorMessage(string title, string error)
        {
            MessageBox.Show(error, title, MessageBoxButton.OK, MessageBoxImage.Error, MessageBoxResult.OK);
        }

        #endregion
    }
}