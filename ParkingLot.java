import java.util.LinkedList;
import java.util.Queue;

public class ParkingLot
{
    private Queue<DummyCar> carQueue = new LinkedList<>();
    private int capacity;

    public ParkingLot(int capacity)
    {
        this.capacity = capacity;
    }

    //Setter: if it isnt full, producer keeps producing; if full, await
    public synchronized void produceCar(DummyCar car) throws InterruptedException
    {
        while (carQueue.size() == capacity)
        {
            wait(); //Wait until not full to produce
        }
        carQueue.add(car);
        System.out.println("Produced: " + car);
        notifyAll(); //Wake up waiting thread
    }

    //Getter: if it isnt empty, consumer consumes 1 car; if empty, await
    public synchronized DummyCar consumeCar() throws InterruptedException
    {
        while (carQueue.isEmpty())
        {
            wait(); //Wait until there is a car to get
        }
        DummyCar car = carQueue.poll(); //Take the head of Queue
        System.out.println("Consumed: " + car);
        notifyAll(); //Wake up waiting thread
        return car;
    }
}
