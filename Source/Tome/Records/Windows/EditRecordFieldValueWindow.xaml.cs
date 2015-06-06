// --------------------------------------------------------------------------------------------------------------------
// <copyright file="EditRecordFieldValueWindow.xaml.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Records.Windows
{
    using System.Windows;
    using System.Windows.Controls;

    using Tome.Records.ViewModels;
    using Tome.Util;

    /// <summary>
    ///   Interaction logic for EditRecordFieldValueWindow.xaml
    /// </summary>
    public partial class EditRecordFieldValueWindow : Window
    {
        #region Fields

        private RecordFieldViewModel recordFieldViewModel;

        #endregion

        #region Constructors and Destructors

        public EditRecordFieldValueWindow()
        {
            InitializeComponent();
        }

        #endregion

        #region Properties

        public RecordFieldViewModel RecordFieldViewModel
        {
            get
            {
                return this.recordFieldViewModel;
            }
            set
            {
                this.recordFieldViewModel = value;
                this.DataContext = this.recordFieldViewModel;

                this.UpdateValueControl();
            }
        }

        public bool Result { get; set; }

        #endregion

        #region Methods

        private void OnClickCancel(object sender, RoutedEventArgs e)
        {
            this.Result = false;
            this.Close();
        }

        private void OnClickOk(object sender, RoutedEventArgs e)
        {
            if (!ValidationUtils.IsValid(this))
            {
                return;
            }

            // Close window.
            this.Result = true;
            this.Close();
        }

        private void UpdateValueControl()
        {
            // Create control.
            var textBox = ControlUtils.CreateValueControl(
                this.RecordFieldViewModel,
                "FieldValue",
                this.RecordFieldViewModel.FieldType);
            textBox.Style = (Style)this.FindResource("ErrorLabelMargin");
            Validation.SetErrorTemplate(textBox, (ControlTemplate)this.FindResource("ErrorLabel"));

            // Add control to window.
            this.DockPanelFieldValueUIElement.Children.Clear();
            this.DockPanelFieldValueUIElement.Children.Add(textBox);
        }

        #endregion
    }
}