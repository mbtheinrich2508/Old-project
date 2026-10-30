public class Main
{
    public static void main(String[] args)
    {
        CarDealership cd = new CarDealership(UniquelyFancyCarPartFactory.getInstance());
        Car myCar = cd.order(Color.BLUE, Color.GREY);

        System.out.println("Pistons: "+ myCar.getEngine().getNumberOfPistons()
                +"\nSize: "+ myCar.getEngine().getSize()
                +"\nFuel Consumption: "+ myCar.getEngine().getFuelConsumption());

        for (int i = 0; i < myCar.getSeats().size(); i++)
        {
            System.out.println("Color of Seats " + i + ": " + myCar.getSeats().get(i).getColor()); // .get(i) for List imple
        }
        System.out.println("Color of Car: " + myCar.getColor());
        myCar.fuelUp();
        if (myCar.testDrive())
            System.out.println("Ja, Sie können das Auto prüfen");
        else System.out.println("Nein, leider kein Ausprobieren!");
    }
}