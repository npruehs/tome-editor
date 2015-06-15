// --------------------------------------------------------------------------------------------------------------------
// <copyright file="FieldDefinitionsViewModel.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Fields.ViewModels
{
    using System.Collections.Generic;

    using Tome.Collections;
    using Tome.Model.Fields;

    public class FieldDefinitionsViewModel
    {
        #region Constructors and Destructors

        public FieldDefinitionsViewModel(List<FieldDefinitionFile> fieldDefinitionFiles)
        {
            // Build field definition file view models.
            this.FieldDefinitionFiles = new List<FieldDefinitionFileViewModel>();

            foreach (var fieldDefinitionFile in fieldDefinitionFiles)
            {
                this.FieldDefinitionFiles.Add(new FieldDefinitionFileViewModel(fieldDefinitionFile));
            }

            // Build flattened list of field definitions.
            this.FieldDefinitions = new MultipleCollectionsView<FieldDefinition>();

            foreach (var fieldDefinitionFile in this.FieldDefinitionFiles)
            {
                this.FieldDefinitions.AddCollection(fieldDefinitionFile.FieldDefinitions);
            }
        }

        #endregion

        #region Properties

        public List<FieldDefinitionFileViewModel> FieldDefinitionFiles { get; }

        public MultipleCollectionsView<FieldDefinition> FieldDefinitions { get; set; }

        #endregion
    }
}