import java.util.List;

public class CarDealership
{
    private CarPartFactory partFactory;

    public CarDealership(CarPartFactory partFactory)
    {
        this.partFactory = partFactory;
    }

    public Car order(Color carColor, Color seatColor)
    {
        return new Car(this.partFactory.buildEngine(),
                        List.of(this.partFactory.buildSeat(seatColor), this.partFactory.buildSeat(seatColor)),
                        carColor);
    }
}
