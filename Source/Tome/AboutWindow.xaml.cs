// --------------------------------------------------------------------------------------------------------------------
// <copyright file="AboutWindow.xaml.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome
{
    using System.Diagnostics;
    using System.Windows;
    using System.Windows.Navigation;

    public partial class AboutWindow : Window
    {
        #region Constructors and Destructors

        public AboutWindow()
        {
            this.InitializeComponent();
        }

        #endregion

        #region Methods

        private void HyperlinkRequestNavigate(object sender, RequestNavigateEventArgs e)
        {
            Process.Start(new ProcessStartInfo(e.Uri.AbsoluteUri));
            e.Handled = true;
        }

        #endregion
    }
}