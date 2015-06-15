// --------------------------------------------------------------------------------------------------------------------
// <copyright file="Vector3FConverter.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Model.Data
{
    using System;
    using System.ComponentModel;
    using System.Globalization;

    public class Vector3FConverter : TypeConverter
    {
        #region Public Methods and Operators

        public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType)
        {
            if (sourceType == typeof(Vector3F))
            {
                return true;
            }

            return base.CanConvertFrom(context, sourceType);
        }

        public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
        {
            if (destinationType == typeof(string))
            {
                return true;
            }

            return base.CanConvertTo(context, destinationType);
        }

        public override object ConvertFrom(ITypeDescriptorContext context, CultureInfo culture, object value)
        {
            var s = value as string;

            if (s != null)
            {
                var v = s.Replace(" ", "").Replace("(", "").Replace(")", "").Split(',');
                return new Vector3F(float.Parse(v[0]), float.Parse(v[1]), float.Parse(v[2]));
            }

            return base.ConvertFrom(context, culture, value);
        }

        public override object ConvertTo(
            ITypeDescriptorContext context,
            CultureInfo culture,
            object value,
            Type destinationType)
        {
            if (destinationType == typeof(string))
            {
                var v = (Vector3F)value;
                return $"{v.X}, {v.Y}, {v.Z}";
            }
            return base.ConvertTo(context, culture, value, destinationType);
        }

        public override bool IsValid(ITypeDescriptorContext context, object value)
        {
            var s = value as string;

            if (s != null)
            {
                var v = s.Replace(" ", "").Replace("(", "").Replace(")", "").Split(',');
                float temp;
                return v.Length == 3 && float.TryParse(v[0], out temp) && float.TryParse(v[1], out temp)
                       && float.TryParse(v[2], out temp);
            }

            return base.IsValid(context, value);
        }

        #endregion
    }
}