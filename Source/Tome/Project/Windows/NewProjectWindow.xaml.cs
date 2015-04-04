// --------------------------------------------------------------------------------------------------------------------
// <copyright file="NewProjectWindow.xaml.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Project.Windows
{
    using System;
    using System.Windows;

    using Tome.Model.Project;

    public partial class NewProjectWindow : Window
    {
        #region Constructors and Destructors

        public NewProjectWindow()
        {
            this.InitializeComponent();

            this.TomeProject = new TomeProject();
            this.DataContext = this.TomeProject;
        }

        #endregion

        #region Properties

        public TomeProject TomeProject { get; }

        #endregion

        #region Methods

        private void OnClickCreateProject(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void OnLoaded(object sender, EventArgs e)
        {
            this.ProjectTitleTextBox.Focus();
        }

        #endregion
    }
}