import java.util.concurrent.ExecutionException;

public class Main
{
    public static void main(String[] args) throws Exception {
        /*
        Parking Lot holds many cars, it may be full or empty:
        + If full (that depends on capacity), producer stops and consumer can buy normally.
        + If empty, consumer stops and waits for new cars, while producer will produce a new car( new car takes 3000ms = 3s).
        + Classes Producer and Consumer implement Runnable for void run()
         */

//        ParkingLot lot = new ParkingLot(2);
//
//        Thread tp1 = new Thread(new Producer(lot, "FX10"));
//        Thread tp2 = new Thread(new Producer(lot, "FX20"));
//
//        Thread tc1 = new Thread(new Consumer(lot));
//        Thread tc2 = new Thread(new Consumer(lot));
//
//        //All Thread: start -> interrupt -> join
//        tp1.start();    tp2.start();
//        tc1.start();    tc2.start();
//        // 4 parallel Threads
//
//        Thread.sleep(2000); // 2s
//
//        tp1.interrupt();    tp2.interrupt();
//        tc1.interrupt();    tc2.interrupt();
//
//        tp1.join();    tp2.join();
//        tc1.join();    tc2.join();

        //A3
        long start = System.currentTimeMillis();
        int sum = Fibonacci.sumFibo(44);
        long duration = System.currentTimeMillis() - start;

        System.out.println("Sum = " + sum + "\nDuration: " + duration/1000);

    }
}