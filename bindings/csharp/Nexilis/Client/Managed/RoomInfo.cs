/* Copyright (C) 2026 Valtteri Viirret
   This file is part of the Nexilis Project.

   This file is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This file is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this file.  If not, see <https://gnu.org>. */

using System;

namespace Nexilis.Client
{
    /// <summary>
    /// A plain, immutable snapshot of the metadata of a room the client knows
    /// about. Unlike <see cref="Room"/> it holds no native memory, so it stays
    /// valid after the collection it was read from has been released. This is
    /// what you want for a room browser / server list.
    /// </summary>
    public sealed class RoomInfo : IEquatable<RoomInfo>
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="RoomInfo"/> class.
        /// </summary>
        /// <param name="id">The unique id of the room.</param>
        /// <param name="name">The name of the room.</param>
        /// <param name="context">The 2D/3D context of the room.</param>
        /// <param name="maxSize">The maximum amount of clients in the room.</param>
        /// <param name="clientCount">The amount of clients currently in the room.</param>
        /// <param name="creatorId">The id of the client that created the room.</param>
        public RoomInfo(ulong id, string name, RoomContext context, uint maxSize, ulong clientCount, ulong creatorId = 0)
        {
            Id = id;
            Name = name ?? string.Empty;
            Context = context;
            MaxSize = maxSize;
            ClientCount = clientCount;
            CreatorId = creatorId;
        }

        /// <summary>The unique id of the room. Pass this to a join request.</summary>
        public ulong Id { get; }

        /// <summary>The name of the room, never null.</summary>
        public string Name { get; }

        /// <summary>Whether the room is a 2D or a 3D room.</summary>
        public RoomContext Context { get; }

        /// <summary>The maximum amount of clients the room accepts.</summary>
        public uint MaxSize { get; }

        /// <summary>The amount of clients currently in the room.</summary>
        public ulong ClientCount { get; }

        /// <summary>The id of the client that created the room, zero if unknown.</summary>
        public ulong CreatorId { get; }

        /// <summary>True when the room cannot accept more clients.</summary>
        public bool IsFull => MaxSize > 0 && ClientCount >= MaxSize;

        /// <summary>True when the room still accepts clients.</summary>
        public bool HasFreeSpace => !IsFull;

        /// <summary>True when nobody has joined the room yet.</summary>
        public bool IsEmpty => ClientCount == 0;

        /// <summary>
        /// Reads the metadata of a live <see cref="Room"/>. Every value is
        /// copied out of the native room, so the snapshot outlives the room
        /// wrapper it was taken from.
        /// </summary>
        /// <param name="room">The room to read.</param>
        /// <exception cref="ArgumentNullException">Thrown if room is null.</exception>
        public static RoomInfo FromRoom(Room room)
        {
            if (room == null)
            {
                throw new ArgumentNullException(nameof(room));
            }

            return new RoomInfo(room.GetId(), room.GetName(), room.GetContext(), room.GetMaxSize(), room.GetClientAmount(), room.GetCreatorId());
        }

        public bool Equals(RoomInfo? other)
        {
            if (other is null)
            {
                return false;
            }
            if (ReferenceEquals(this, other))
            {
                return true;
            }
            return Id == other.Id
                && string.Equals(Name, other.Name, StringComparison.Ordinal)
                && Context == other.Context
                && MaxSize == other.MaxSize
                && ClientCount == other.ClientCount
                && CreatorId == other.CreatorId;
        }

        public override bool Equals(object? obj) => Equals(obj as RoomInfo);

        public override int GetHashCode()
        {
            unchecked
            {
                int hash = Id.GetHashCode();
                hash = (hash * 397) ^ Name.GetHashCode();
                hash = (hash * 397) ^ (int)Context;
                hash = (hash * 397) ^ (int)MaxSize;
                hash = (hash * 397) ^ ClientCount.GetHashCode();
                hash = (hash * 397) ^ CreatorId.GetHashCode();
                return hash;
            }
        }

        public override string ToString() => $"RoomInfo(id={Id}, name={Name}, context={Context}, clients={ClientCount}/{MaxSize})";
    }
}
