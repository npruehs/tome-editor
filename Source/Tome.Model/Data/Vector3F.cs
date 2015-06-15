// --------------------------------------------------------------------------------------------------------------------
// <copyright file="Vector3F.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Model.Data
{
    using System.ComponentModel;

    [TypeConverter(typeof(Vector3FConverter))]
    public class Vector3F
    {
        #region Constants

        public static readonly Vector3F Zero = new Vector3F();

        #endregion

        #region Constructors and Destructors

        public Vector3F()
        {
        }

        public Vector3F(float x, float y, float z)
        {
            this.X = x;
            this.Y = y;
            this.Z = z;
        }

        #endregion

        #region Properties

        public float X { get; set; }

        public float Y { get; set; }

        public float Z { get; set; }

        #endregion

        #region Public Methods and Operators

        public override string ToString()
        {
            return $"{this.X}, {this.Y}, {this.Z}";
        }

        #endregion
    }
}