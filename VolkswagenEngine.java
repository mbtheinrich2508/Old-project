public class VolkswagenEngine implements Engine
{
    private int numberOfPistons;
    private int size;
    private int fuelConsumption;

    public VolkswagenEngine(int numberOfPistons, int size, int fuelConsumption)
    {
        this.numberOfPistons = numberOfPistons;
        this.size = size;
        this.fuelConsumption = fuelConsumption;
    }

    public int getNumberOfPistons()
    {
        return numberOfPistons;
    }
    public int getSize()
    {
        return size;
    }
    public int getFuelConsumption()
    {
        return fuelConsumption;
    }
}
