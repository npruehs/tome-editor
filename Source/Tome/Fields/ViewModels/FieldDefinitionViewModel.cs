// --------------------------------------------------------------------------------------------------------------------
// <copyright file="FieldDefinitionViewModel.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Fields.ViewModels
{
    using System.ComponentModel;
    using System.Runtime.CompilerServices;

    using Tome.Model.Fields;

    public class FieldDefinitionViewModel : INotifyPropertyChanged
    {
        #region Fields

        private object defaultValue;

        private string description;

        private string displayName;

        private FieldType fieldType;

        private string id;

        #endregion

        #region Events

        public event PropertyChangedEventHandler PropertyChanged;

        #endregion

        #region Properties

        public object DefaultValue
        {
            get
            {
                return this.defaultValue;
            }
            set
            {
                if (!Equals(this.defaultValue, value))
                {
                    this.defaultValue = value;
                    this.OnPropertyChanged();
                }
            }
        }

        public string Description
        {
            get
            {
                return this.description;
            }
            set
            {
                if (!Equals(this.description, value))
                {
                    this.description = value;
                    this.OnPropertyChanged();
                }
            }
        }

        public string DisplayName
        {
            get
            {
                return this.displayName;
            }
            set
            {
                if (!Equals(this.displayName, value))
                {
                    this.displayName = value;
                    this.OnPropertyChanged();
                }
            }
        }

        public FieldType FieldType
        {
            get
            {
                return this.fieldType;
            }
            set
            {
                if (!Equals(this.fieldType, value))
                {
                    this.fieldType = value;
                    this.OnPropertyChanged();
                }
            }
        }

        public string Id
        {
            get
            {
                return this.id;
            }
            set
            {
                if (!Equals(this.id, value))
                {
                    this.id = value;
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