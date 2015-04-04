// --------------------------------------------------------------------------------------------------------------------
// <copyright file="TomeProject.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Model.Project
{
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
    }
}