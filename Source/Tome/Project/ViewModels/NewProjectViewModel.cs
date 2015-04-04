// --------------------------------------------------------------------------------------------------------------------
// <copyright file="NewProjectViewModel.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Project.ViewModels
{
    using System.ComponentModel;
    using System.Runtime.CompilerServices;

    public class NewProjectViewModel : INotifyPropertyChanged
    {
        #region Fields

        private string name;

        private string path;

        #endregion

        #region Events

        public event PropertyChangedEventHandler PropertyChanged;

        #endregion

        #region Properties

        public string Name
        {
            get
            {
                return this.name;
            }
            set
            {
                if (!Equals(this.name, value))
                {
                    this.name = value;
                    this.OnPropertyChanged();
                }
            }
        }

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