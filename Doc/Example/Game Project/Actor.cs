// --------------------------------------------------------------------------------------------------------------------
// <copyright file="Actor.cs" company="Tome">
//   Copyright (c) Tome. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------

namespace TomeExampleGame
{
    public class Actor
    {
        #region Properties

        /// <summary>
        ///   Maximum health of this actor. If reduced to zero, it dies.
        /// </summary>
        /*
           @tomeid Health
           @tomedn Health
           @tometype Int
           @tomedesc Maximum health of this actor. If reduced to zero, it dies.
           @tomedefault 100
         */
        public int Health { get; set; }

        /// <summary>
        ///   Whether this actor can take damage and be targeted, or not.
        /// </summary>
        /*
           @tomeid Invulnerable
           @tomedn Invulnerable
           @tometype Boolean
           @tomedesc Whether this actor can take damage and be targeted, or not.
           @tomedefault False
         */
        public bool Invulnerable { get; set; }

        /// <summary>
        ///   Speed of this actor, in m/s.
        /// </summary>
        /*
           @tomeid MovementSpeed
           @tomedn Movement Speed
           @tometype Float
           @tomedesc Speed of this actor, in m/s.
           @tomedefault 3.75
         */
        public float MovementSpeed { get; set; }

        /// <summary>
        ///   Name of this actor.
        /// </summary>
        /*
           @tomeid Name
           @tomedn Name
           @tometype String
           @tomedesc Name of this actor.
           @tomedefault
         */
        public string Name { get; set; }

        /// <summary>
        ///   Type of the projectile fired by this entity when attacking.
        /// </summary>
        /*
           @tomeid Projectile
           @tomedn Projectile
           @tometype Reference
           @tomedesc Type of the projectile fired by this entity when attacking.
           @tomedefault
         */
        public string Projectile { get; set; }

        #endregion
    }
}