// --------------------------------------------------------------------------------------------------------------------
// <copyright file="FieldDefinitionsViewModel.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Fields.ViewModels
{
    using System.Collections.ObjectModel;
    using System.ComponentModel;
    using System.Runtime.CompilerServices;

    using Tome.Model.Fields;

    public class FieldDefinitionsViewModel : INotifyPropertyChanged
    {
        #region Fields

        private ObservableCollection<FieldDefinition> fieldDefinitions;

        #endregion

        #region Constructors and Destructors

        public FieldDefinitionsViewModel()
        {
            this.fieldDefinitions = new ObservableCollection<FieldDefinition>();
        }

        #endregion

        #region Events

        public event PropertyChangedEventHandler PropertyChanged;

        #endregion

        #region Properties

        public ObservableCollection<FieldDefinition> FieldDefinitions
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