// --------------------------------------------------------------------------------------------------------------------
// <copyright file="RecordFileViewModel.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Records.ViewModels
{
    using System.ComponentModel;
    using System.Runtime.CompilerServices;

    using Tome.Collections;
    using Tome.Model.Records;

    public class RecordFileViewModel
    {
        #region Fields

        private string path;

        #endregion

        #region Constructors and Destructors

        public RecordFileViewModel(RecordFile recordFile)
        {
            this.Path = recordFile.Path;
            this.Records = new ObservableWrappedCollection<Record>(recordFile.Records);
        }

        #endregion

        #region Events

        public event PropertyChangedEventHandler PropertyChanged;

        #endregion

        #region Properties

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

        public ObservableWrappedCollection<Record> Records { get; }

        #endregion

        #region Public Methods and Operators

        public override string ToString()
        {
            return this.Path != null ? System.IO.Path.GetFileName(this.Path) : string.Empty;
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