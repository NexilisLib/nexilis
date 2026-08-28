namespace Nexilis.Util
{
    /// <summary>
    /// Lightweight 3D position used by the remote-client callbacks. Plain
    /// floats on purpose so the type can be used straight from any engine.
    /// </summary>
    public readonly struct Position
    {
        public Position(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public float X { get; }
        public float Y { get; }
        public float Z { get; }

        public override string ToString() => $"Position({X}, {Y}, {Z})";
    }
}

