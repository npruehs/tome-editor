// --------------------------------------------------------------------------------------------------------------------
// <copyright file="CanConvertToTypeValidationRule.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Core.Validation
{
    using System;
    using System.ComponentModel;
    using System.Globalization;
    using System.Windows.Controls;

    using Tome.Util;

    public class CanConvertToTypeValidationRule : ValidationRule
    {
        #region Constructors and Destructors

        public CanConvertToTypeValidationRule()
        {
        }

        public CanConvertToTypeValidationRule(Type type)
        {
            this.Type = type;
        }

        #endregion

        #region Properties

        public Type Type { get; set; }

        #endregion

        #region Public Methods and Operators

        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            return ConversionUtils.CanConvertTo(value, this.Type)
                ? ValidationResult.ValidResult
                : new ValidationResult(false, string.Format("Can't convert to type {0}.", this.Type.Name));
        }

        #endregion
    }
}