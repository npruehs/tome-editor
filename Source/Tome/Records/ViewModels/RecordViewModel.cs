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

        private string id;

        #endregion

        #region Events

        public event PropertyChangedEventHandler PropertyChanged;

        #endregion

        #region Properties

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