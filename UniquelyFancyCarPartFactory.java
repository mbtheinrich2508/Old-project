public class UniquelyFancyCarPartFactory implements CarPartFactory
{
    private static UniquelyFancyCarPartFactory instance = null;

    private UniquelyFancyCarPartFactory() {}

    public static synchronized UniquelyFancyCarPartFactory getInstance()
    {
        if (instance == null)
            instance = new UniquelyFancyCarPartFactory();
        return instance;
    }

    public Engine buildEngine()
    {
        return new VolkswagenEngine(6, 2, 10);
    }
    public Seat buildSeat(Color seatColor)
    {
        return new VolkswagenSeat(seatColor);
    }
}
