public class VolkswagenCarPartFactory implements CarPartFactory
{
    public Engine buildEngine()
    {
        return new VolkswagenEngine(6, 2, 10);
    }
    public Seat buildSeat(Color seatColor)
    {
        return new VolkswagenSeat(seatColor);
    }
}
