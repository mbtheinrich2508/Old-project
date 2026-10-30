public class Cell
{
    private long value;

    public Cell(long value)
    {
        this.value = value;
    }

    public synchronized void setValue(long value)
    {
        this.value = value;
    }
    public synchronized long getValue()
    {
        return value;
    }

    public void swapValue(Cell other)
    {
        if (this == other) return;
        Cell a = this;
        Cell b = other;

        //This piece ensures resource ordering to prevent deadlock when locking multiple objects
        if (System.identityHashCode(a) > System.identityHashCode(b))
            // identityHashCode: Returns a unique integer identifier for an object, based on its memory address
        {
            a = other;
            b = this;
        }

        synchronized (a)
        {
            try
            {
                Thread.sleep(500); // Verlängert Sperrdauer zum Testen
            }
            catch (InterruptedException e)
            {
                Thread.currentThread().interrupt();
            }
            synchronized (b)
            {
                long temp = getValue();
                setValue(other.getValue());
                other.setValue(temp);
            }
        }
    }
}
