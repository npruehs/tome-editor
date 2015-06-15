// --------------------------------------------------------------------------------------------------------------------
// <copyright file="Vector3FControl.xaml.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace Tome.Fields.Controls
{
    using System.Windows;
    using System.Windows.Controls;

    using Tome.Model.Data;
    using Tome.Util;

    /// <summary>
    ///   Interaction logic for Vector3FControl.xaml
    /// </summary>
    public partial class Vector3FControl : UserControl
    {
        #region Constants

        public static readonly DependencyProperty VectorProperty = DependencyProperty.Register(
            "Vector",
            typeof(Vector3F),
            typeof(Vector3FControl),
            new FrameworkPropertyMetadata(Vector3F.Zero, OnVectorChanged, CoerceVector));

        public static readonly RoutedEvent VectorChangedEvent = EventManager.RegisterRoutedEvent(
            "VectorChanged",
            RoutingStrategy.Bubble,
            typeof(RoutedPropertyChangedEventHandler<Vector3F>),
            typeof(Vector3FControl));

        #endregion

        #region Constructors and Destructors

        public Vector3FControl()
        {
            this.InitializeComponent();

            this.DataContext = this;
        }

        #endregion

        #region Events

        public event RoutedPropertyChangedEventHandler<Vector3F> VectorChanged
        {
            add
            {
                this.AddHandler(VectorChangedEvent, value);
            }
            remove
            {
                this.RemoveHandler(VectorChangedEvent, value);
            }
        }

        #endregion

        #region Properties

        public Vector3F Vector
        {
            get
            {
                return (Vector3F)this.GetValue(VectorProperty);
            }
            set
            {
                this.SetValue(VectorProperty, value);
            }
        }

        #endregion

        #region Methods

        protected virtual void OnVectorChanged(Vector3F oldValue, Vector3F newValue)
        {
            var args = new RoutedPropertyChangedEventArgs<Vector3F>(oldValue, newValue, VectorChangedEvent);
            this.RaiseEvent(args);
        }

        private static object CoerceVector(DependencyObject element, object value)
        {
            return value;
        }

        private static void OnVectorChanged(DependencyObject obj, DependencyPropertyChangedEventArgs args)
        {
            var control = (Vector3FControl)obj;
            control.OnVectorChanged((Vector3F)args.OldValue, (Vector3F)args.NewValue);
        }

        private void OnXChanged(object sender, TextChangedEventArgs textChangedEventArgs)
        {
            this.Vector.X = ConversionUtils.ConvertOrDefault<float>(this.XTextBox.Text);
        }

        private void OnYChanged(object sender, TextChangedEventArgs textChangedEventArgs)
        {
            this.Vector.Y = ConversionUtils.ConvertOrDefault<float>(this.YTextBox.Text);
        }

        private void OnZChanged(object sender, TextChangedEventArgs textChangedEventArgs)
        {
            this.Vector.Z = ConversionUtils.ConvertOrDefault<float>(this.ZTextBox.Text);
        }

        #endregion
    }
}