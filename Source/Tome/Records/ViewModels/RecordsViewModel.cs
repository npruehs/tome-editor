// --------------------------------------------------------------------------------------------------------------------
// <copyright file="RecordsViewModel.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Records.ViewModels
{
    using System.Collections.Generic;

    using Tome.Collections;
    using Tome.Model.Records;

    public class RecordsViewModel
    {
        #region Constructors and Destructors

        public RecordsViewModel(List<RecordFile> recordFiles)
        {
            // Build record file view models.
            this.RecordFiles = new List<RecordFileViewModel>();

            foreach (var recordFile in recordFiles)
            {
                this.RecordFiles.Add(new RecordFileViewModel(recordFile));
            }

            // Build flattened list of field definitions.
            this.Records = new MultipleCollectionsView<Record>();

            foreach (var recordFile in this.RecordFiles)
            {
                this.Records.AddCollection(recordFile.Records);
            }
        }

        #endregion

        #region Properties

        public List<RecordFileViewModel> RecordFiles { get; }

        public MultipleCollectionsView<Record> Records { get; set; }

        #endregion
    }
}