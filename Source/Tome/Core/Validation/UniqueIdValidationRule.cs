// --------------------------------------------------------------------------------------------------------------------
// <copyright file="UniqueIdValidationRule.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Core.Validation
{
    using System.Collections.Generic;
    using System.Globalization;
    using System.Linq;
    using System.Windows.Controls;

    public class UniqueIdValidationRule : ValidationRule
    {
        #region Properties

        public IEnumerable<string> ExistingIds { get; set; }

        #endregion

        #region Public Methods and Operators

        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            var id = (string)value;
            if (this.ExistingIds.Contains(id))
            {
                return new ValidationResult(false, "Id already in use.");
            }

            return ValidationResult.ValidResult;
        }

        #endregion
    }
}