// --------------------------------------------------------------------------------------------------------------------
// <copyright file="EditRecordFieldValueWindow.xaml.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Records.Windows
{
    using System.Collections.Generic;
    using System.Windows;
    using System.Windows.Controls;
    using System.Windows.Media;

    using Tome.Fields.Controls;
    using Tome.Model.Data;
    using Tome.Model.Fields;
    using Tome.Records.ViewModels;
    using Tome.Util;

    /// <summary>
    ///   Interaction logic for EditRecordFieldValueWindow.xaml
    /// </summary>
    public partial class EditRecordFieldValueWindow : Window
    {
        #region Fields

        private IEnumerable<string> existingRecordIds;

        private RecordFieldViewModel recordFieldViewModel;

        #endregion

        #region Constructors and Destructors

        public EditRecordFieldValueWindow()
        {
            this.InitializeComponent();
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

        #region Public Methods and Operators

        public void SetExistingRecordIds(IEnumerable<string> existingRecordIds)
        {
            this.existingRecordIds = existingRecordIds;
        }

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
            // Build additional control data, if necessary.
            object data = null;

            switch (this.RecordFieldViewModel.FieldType)
            {
                case FieldType.Reference:
                    data = this.existingRecordIds;
                    break;
            }

            // Convert field value to correct type.
            this.RecordFieldViewModel.FieldValue = ConversionUtils.Convert(
                this.RecordFieldViewModel.FieldValue,
                this.RecordFieldViewModel.FieldType);

            // Create control.
            var control = ControlFactory.CreateControl(
                this.RecordFieldViewModel,
                "FieldValue",
                this.RecordFieldViewModel.FieldType,
                data);
            control.Style = (Style)this.FindResource("ErrorLabelMargin");
            Validation.SetErrorTemplate(control, (ControlTemplate)this.FindResource("ErrorLabel"));

            // Add control to window.
            this.DockPanelFieldValueUIElement.Children.Clear();
            this.DockPanelFieldValueUIElement.Children.Add(control);

            // Focus textbox, if any.
            var textBox = control as TextBox;

            if (textBox != null)
            {
                textBox.Focus();
                textBox.SelectAll();
            }
        }

        #endregion
    }
}