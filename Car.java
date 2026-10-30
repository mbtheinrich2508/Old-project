import java.util.List;

public class Car
{
    private Engine engine;
    private List<Seat> seats;
    private Color color;

    public Car(Engine engine, List<Seat> seats, Color color)
    {
        this.engine = engine;
        this.seats = seats;
        this.color = color;
    }

    public Engine getEngine()
    {
        return this.engine;
    }
    public List<Seat> getSeats()
    {
        return this.seats;
    }
    public Color getColor()
    {
        return this.color;
    }
    public void fuelUp()
    {
        System.out.println("Fuel up!");
    }
    public boolean testDrive()
    {
        return true;
        //Ja, Sie können das Auto prüfen
    }
}
