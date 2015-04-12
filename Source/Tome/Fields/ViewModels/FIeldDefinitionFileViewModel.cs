// --------------------------------------------------------------------------------------------------------------------
// <copyright file="FIeldDefinitionFileViewModel.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Fields.ViewModels
{
    using System.ComponentModel;
    using System.Runtime.CompilerServices;

    using Tome.Collections;
    using Tome.Model.Fields;

    public class FieldDefinitionFileViewModel : INotifyPropertyChanged
    {
        #region Fields

        private string path;

        #endregion

        #region Constructors and Destructors

        public FieldDefinitionFileViewModel(FieldDefinitionFile fieldDefinitionFile)
        {
            this.Path = fieldDefinitionFile.Path;
            this.FieldDefinitions =
                new ObservableWrappedCollection<FieldDefinition>(fieldDefinitionFile.FieldDefinitions);
        }

        #endregion

        #region Events

        public event PropertyChangedEventHandler PropertyChanged;

        #endregion

        #region Properties

        public ObservableWrappedCollection<FieldDefinition> FieldDefinitions { get; }

        public string Path
        {
            get
            {
                return this.path;
            }
            set
            {
                if (!Equals(this.path, value))
                {
                    this.path = value;
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