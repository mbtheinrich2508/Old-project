public class Consumer implements Runnable
{
    private ParkingLot lot;

    public Consumer(ParkingLot lot)
    {
        this.lot = lot;
    }

    @Override
    public void run()
    {
        try
        {
            while (!Thread.currentThread().isInterrupted())
            {
                DummyCar car = lot.consumeCar();
                Thread.sleep(500); // sell a car in 500ms
            }
        }
        catch (InterruptedException e)
        {
            System.out.println("Consumer interrupted.");
        }
    }
}
