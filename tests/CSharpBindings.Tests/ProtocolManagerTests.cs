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
