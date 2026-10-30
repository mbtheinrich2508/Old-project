public class Producer implements Runnable
{
    private ParkingLot lot;
    private String model;

    public Producer(ParkingLot lot, String model)
    {
        this.lot = lot;
        this.model = model;
    }

    @Override
    public void run()
    {
        try
        {
            while (!Thread.currentThread().isInterrupted()) //Check if it is not interrupted
            {
                Thread.sleep(1000); //produce a car in 2000ms
                lot.produceCar(new DummyCar(model));
            }
        }
        catch (InterruptedException e)
        {
            System.out.println("Producer " + model + " interrupted.");
        }
    }
}
