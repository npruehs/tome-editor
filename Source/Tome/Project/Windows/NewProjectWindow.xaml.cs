// --------------------------------------------------------------------------------------------------------------------
// <copyright file="NewProjectWindow.xaml.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Project.Windows
{
    using System;
    using System.Windows;
    using System.Windows.Forms;

    using Tome.Project.ViewModels;

    public partial class NewProjectWindow : Window
    {
        #region Constructors and Destructors

        public NewProjectWindow()
        {
            this.InitializeComponent();

            this.NewProjectViewModel = new NewProjectViewModel
            {
                Path = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments)
            };

            this.DataContext = this.NewProjectViewModel;
        }

        #endregion

        #region Properties

        public NewProjectViewModel NewProjectViewModel { get; }

        public bool Result { get; private set; }

        #endregion

        #region Methods

        private void OnClickBrowseLocation(object sender, RoutedEventArgs e)
        {
            var folderBrowserDialog = new FolderBrowserDialog
            {
                SelectedPath = this.NewProjectViewModel.Path,
                ShowNewFolderButton = true
            };

            var result = folderBrowserDialog.ShowDialog();

            if (result == System.Windows.Forms.DialogResult.OK)
            {
                this.NewProjectViewModel.Path = folderBrowserDialog.SelectedPath;
            }
        }

        private void OnClickCancel(object sender, RoutedEventArgs e)
        {
            this.Result = false;
            this.Close();
        }

        private void OnClickCreateProject(object sender, RoutedEventArgs e)
        {
            this.Result = true;
            this.Close();
        }

        private void OnLoaded(object sender, EventArgs e)
        {
            this.NameTextBox.Focus();
        }

        #endregion
    }
}