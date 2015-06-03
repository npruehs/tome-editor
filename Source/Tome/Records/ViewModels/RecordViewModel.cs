// --------------------------------------------------------------------------------------------------------------------
// <copyright file="RecordViewModel.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Records.ViewModels
{
    using System.ComponentModel;
    using System.Runtime.CompilerServices;

    public class RecordViewModel : INotifyPropertyChanged
    {
        #region Fields

        private string displayName;

        private RecordFileViewModel file;

        private string id;

        #endregion

        #region Events

        public event PropertyChangedEventHandler PropertyChanged;

        #endregion

        #region Properties

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

        public RecordFileViewModel File
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