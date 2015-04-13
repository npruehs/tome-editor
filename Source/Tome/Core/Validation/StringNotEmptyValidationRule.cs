// --------------------------------------------------------------------------------------------------------------------
// <copyright file="StringNotEmptyValidationRule.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Core.Validation
{
    using System.Globalization;
    using System.Windows.Controls;

    public class StringNotEmptyValidationRule : ValidationRule
    {
        #region Public Methods and Operators

        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            var s = value as string;
            if (s == null || string.IsNullOrEmpty(s))
            {
                return new ValidationResult(false, "Must not be empty.");
            }

            return ValidationResult.ValidResult;
        }

        #endregion
    }
}