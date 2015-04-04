// --------------------------------------------------------------------------------------------------------------------
// <copyright file="TomeProject.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Model.Project
{
    using System;
    using System.Collections.Generic;

    using Tome.Model.Fields;
    using Tome.Model.Records;

    public class TomeProject
    {
        #region Properties

        public string Description { get; set; }

        public List<FieldDefinitionFile> FieldDefinitionFiles { get; set; }

        public string Name { get; set; }

        public List<RecordFile> RecordFiles { get; set; }

        #endregion

        #region Public Methods and Operators

        public void InitProject()
        {
            if (string.IsNullOrEmpty(this.Name))
            {
                throw new InvalidOperationException("Project name must not be empty.");
            }

            this.Description = string.Empty;
            this.FieldDefinitionFiles = new List<FieldDefinitionFile>
            {
                new FieldDefinitionFile { Path = this.Name + FieldDefinitionFile.FileExtension }
            };
            this.RecordFiles = new List<RecordFile> { new RecordFile { Path = this.Name + RecordFile.FileExtension } };
        }

        #endregion
    }
}