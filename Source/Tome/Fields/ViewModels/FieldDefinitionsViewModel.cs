// --------------------------------------------------------------------------------------------------------------------
// <copyright file="FieldDefinitionsViewModel.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Fields.ViewModels
{
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using System.ComponentModel;
    using System.Runtime.CompilerServices;
    using System.Windows.Data;

    using Tome.Collections;
    using Tome.Model.Fields;

    public class FieldDefinitionsViewModel : INotifyPropertyChanged
    {
        #region Fields

        private MultipleCollectionsView<FieldDefinition> fieldDefinitions;

        #endregion

        #region Constructors and Destructors

        public FieldDefinitionsViewModel(List<ObservableWrappedCollection<FieldDefinition>> fieldDefinitionFiles)
        {
            // Build flattened list of field definitions.
            this.fieldDefinitions = new MultipleCollectionsView<FieldDefinition>();

            foreach (var fieldDefinitionFile in fieldDefinitionFiles)
            {
                this.fieldDefinitions.AddCollection(fieldDefinitionFile);
            }
        }

        #endregion

        #region Events

        public event PropertyChangedEventHandler PropertyChanged;

        #endregion

        #region Properties

        public MultipleCollectionsView<FieldDefinition> FieldDefinitions
        {
            get
            {
                return this.fieldDefinitions;
            }
            set
            {
                if (!Equals(this.fieldDefinitions, value))
                {
                    this.fieldDefinitions = value;
                    this.OnPropertyChanged();
                }
            }
        }

        #endregion

        #region Methods

        private void OnPropertyChanged([CallerMemberName] string propertyName = "")
        {
            if (this.PropertyChanged != null)
            {
                this.PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }
        }

        #endregion
    }
}