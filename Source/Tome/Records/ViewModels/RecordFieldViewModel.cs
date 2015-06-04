// --------------------------------------------------------------------------------------------------------------------
// <copyright file="RecordFieldViewModel.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Records.ViewModels
{
    using Tome.Model.Fields;

    public class RecordFieldViewModel
    {
        #region Properties

        public bool Enabled { get; set; }

        public string FieldId { get; set; }

        public object FieldValue { get; set; }

        #endregion
    }
}