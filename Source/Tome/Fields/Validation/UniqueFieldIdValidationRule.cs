// --------------------------------------------------------------------------------------------------------------------
// <copyright file="UniqueFieldIdValidationRule.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Fields.Validation
{
    using System.Collections.Generic;
    using System.Globalization;
    using System.Linq;
    using System.Windows.Controls;

    public class UniqueFieldIdValidationRule : ValidationRule
    {
        #region Properties

        public IEnumerable<string> ExistingFieldIds { get; set; }

        #endregion

        #region Public Methods and Operators

        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            var fieldId = (string)value;
            if (this.ExistingFieldIds.Contains(fieldId))
            {
                return new ValidationResult(false, "Field id already in use.");
            }

            return ValidationResult.ValidResult;
        }

        #endregion
    }
}