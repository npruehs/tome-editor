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
        #region Constructors and Destructors

        public TomeProject()
        {
        }

        public TomeProject(string name)
        {
            if (string.IsNullOrEmpty(name))
            {
                throw new ArgumentNullException(nameof(name));
            }

            this.Name = name;
            this.Description = string.Empty;
            this.FieldDefinitionFiles = new List<FieldDefinitionFile>
            {
                new FieldDefinitionFile
                {
                    Path = this.Name + FieldDefinitionFile.FileExtension,
                    FieldDefinitions = new List<FieldDefinition>()
                }
            };
            this.RecordFiles = new List<RecordFile>
            {
                new RecordFile { Path = this.Name + RecordFile.FileExtension, Records = new List<Record>() }
            };
        }

        #endregion

        #region Properties

        public string Description { get; set; }

        public List<FieldDefinitionFile> FieldDefinitionFiles { get; set; }

        public string Name { get; set; }

        public List<RecordFile> RecordFiles { get; set; }

        #endregion
    }
}