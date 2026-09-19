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
using Xunit;
using Nexilis.Client;

namespace Nexilis.Tests
{
    public class ClientSessionTests : IDisposable
    {
        private readonly ClientConfig _config;
        private readonly ClientAPI _clientApi;

        public ClientSessionTests()
        {
            TestEnvironment.Initialize();
            _config = new ClientConfig();
            _clientApi = new ClientAPI(_config);
        }

        public void Dispose()
        {
            _clientApi.Dispose();
            _config.Dispose();
        }

        [Fact]
        public void CreateAndDestroy()
        {
            using var session = new ClientSession(42, _clientApi.ClientApiPtr, true);
        }

        [Fact]
        public void GetId()
        {
            using var session = new ClientSession(42, _clientApi.ClientApiPtr, true);

            Assert.Equal(42ul, session.GetId());
        }

        [Fact]
        public void GetIdDifferentValues()
        {
            using var s1 = new ClientSession(1, _clientApi.ClientApiPtr, true);
            using var s2 = new ClientSession(999, _clientApi.ClientApiPtr, true);

            Assert.Equal(1ul, s1.GetId());
            Assert.Equal(999ul, s2.GetId());
        }

        [Fact]
        public void SetAndGetPosition3D()
        {
            using var session = new ClientSession(1, _clientApi.ClientApiPtr, true);
            session.SetPosition3D(1.0f, 2.0f, 3.0f);

            using var pos = session.GetPosition3D();
            Assert.Equal(1.0f, pos.X);
            Assert.Equal(2.0f, pos.Y);
            Assert.Equal(3.0f, pos.Z);
        }

        [Fact]
        public void SetUsername()
        {
            using var session = new ClientSession(1, _clientApi.ClientApiPtr, true);
            // No getter for username via the API, but this should not throw.
            session.SetUsername("test_player");
        }

        [Fact]
        public void NullClientApiHandleThrows()
        {
            Assert.Throws<ArgumentNullException>(() => new ClientSession(1, IntPtr.Zero, true));
        }
    }
}
