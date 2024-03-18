#ifndef NEXILIS_CONFIG_HH
#define NEXILIS_CONFIG_HH

namespace nexilis
{

class Config
{
public:
    static bool getBigEndian()
    {
        return m_bigEndian;
    }

    static void setBigEndian()
    {
        m_bigEndian = true;
    }

    static void setLittleEndian()
    {
        m_bigEndian = false;
    }

private:
    static bool m_bigEndian;
};

} // namespace nexilis

#endif