import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;

public class Fibonacci
{
    public static int fibo(int n)
    {
        if (n < 2) return 1;
        return fibo(n-1) + fibo(n-2);
    }

    public static int sumFibo(int limit) throws Exception
    {
        ExecutorService exec = Executors.newCachedThreadPool();
        List<Future<Integer>> futures = new ArrayList<>();

        for (int i = 0; i < limit; i++)
        {
            final int j = i;
            Callable<Integer> ans = () ->
            {
                return fibo(j); // To use lambda, must be final, so I created j
            };
            var future = exec.submit(ans);
            futures.add(future);
        }

        int sum = 0;
        for (var future : futures)
        {
            sum += future.get();
        }

        exec.shutdown();
        return sum;
    }
}
