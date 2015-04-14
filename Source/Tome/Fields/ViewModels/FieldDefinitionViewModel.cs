// --------------------------------------------------------------------------------------------------------------------
// <copyright file="FieldDefinitionViewModel.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Fields.ViewModels
{
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Linq;
    using System.Runtime.CompilerServices;

    using Tome.Model.Fields;

    public class FieldDefinitionViewModel : INotifyPropertyChanged, IDataErrorInfo
    {
        #region Fields

        private object defaultValue;

        private string description;

        private string displayName;

        private IEnumerable<string> existingFieldIds;

        private FieldType fieldType;

        private FieldDefinitionFileViewModel file;

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

        public string Error { get; }

        public IEnumerable<string> ExistingFieldIds
        {
            get
            {
                return this.existingFieldIds;
            }
            set
            {
                if (!Equals(this.existingFieldIds, value))
                {
                    this.existingFieldIds = value;
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

        public FieldDefinitionFileViewModel File
        {
            get
            {
                return this.file;
            }
            set
            {
                if (!Equals(this.file, value))
                {
                    this.file = value;
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

        public string this[string columnName]
        {
            get
            {
                if (columnName == "Id")
                {
                    if (this.ExistingFieldIds != null && this.ExistingFieldIds.Contains(this.Id))
                    {
                        return "Field id already in use.";
                    }
                }

                return null;
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