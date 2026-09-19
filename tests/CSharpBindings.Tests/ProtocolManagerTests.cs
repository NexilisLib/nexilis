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

using Xunit;
using Nexilis;

namespace Nexilis.Tests
{
    public class ProtocolManagerTests
    {
        [Fact]
        public void CreateAndDestroy()
        {
            using var manager = new ProtocolManager();
            Assert.NotEqual(System.IntPtr.Zero, manager.ProtocolManagerPtr);
        }

        [Fact]
        public void ProtocolDataCreateDestroy()
        {
            using var data = new ProtocolManager.ProtocolData(ProtocolType.BOOST_TCP_SERVER);

            Assert.Equal(ProtocolType.BOOST_TCP_SERVER, data.GetProtocolType());
            Assert.NotEqual(0ul, data.GetId());
        }

        [Fact]
        public void ProtocolDataTypeMatches()
        {
            using var data = new ProtocolManager.ProtocolData(ProtocolType.BOOST_UDP_CLIENT);

            Assert.Equal(ProtocolType.BOOST_UDP_CLIENT, data.GetProtocolType());
        }

        [Fact]
        public void ProtocolDataIdUniqueness()
        {
            using var a = new ProtocolManager.ProtocolData(ProtocolType.BOOST_TCP_SERVER);
            using var b = new ProtocolManager.ProtocolData(ProtocolType.BOOST_TCP_SERVER);

            Assert.NotEqual(a.GetId(), b.GetId());
        }
    }
}
