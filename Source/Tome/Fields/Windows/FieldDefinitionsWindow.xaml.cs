// --------------------------------------------------------------------------------------------------------------------
// <copyright file="FieldDefinitionsWindow.xaml.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Fields.Windows
{
    using System.Windows;

    using Tome.Fields.ViewModels;
    using Tome.Model.Project;

    /// <summary>
    ///   Interaction logic for FieldDefinitionsWindow.xaml
    /// </summary>
    public partial class FieldDefinitionsWindow : Window
    {
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

        public void SetFieldDefinitions(TomeProject project)
        {
            this.FieldDefinitionsViewModel.FieldDefinitions.Clear();

            foreach (var fieldDefinitionFile in project.FieldDefinitionFiles)
            {
                foreach (var fieldDefinition in fieldDefinitionFile.FieldDefinitions)
                {
                    this.FieldDefinitionsViewModel.FieldDefinitions.Add(fieldDefinition);
                }
            }
        }

        #endregion
    }
}