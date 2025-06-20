#ifndef NEXILIS_CONFIG_HH
#define NEXILIS_CONFIG_HH

namespace nexilis::server
{

class Config
{
public:
    static bool getBigEndian()
    {
        return m_bigEndian;
    }

    static void setBigEndian(bool bigEndian)
    {
        m_bigEndian = bigEndian;
    }

    static bool isSystemBigEndian();

private:
    static bool m_bigEndian;
};

} // namespace nexilis::server

#endif
