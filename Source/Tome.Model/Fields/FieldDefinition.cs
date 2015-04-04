// --------------------------------------------------------------------------------------------------------------------
// <copyright file="FieldDefinition.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Model.Fields
{
    public class FieldDefinition
    {
        #region Properties

        public object DefaultValue { get; set; }

        public string Description { get; set; }

        public string DisplayName { get; set; }

        public FieldType FieldType { get; set; }

        public string Id { get; set; }

        #endregion
    }
}