public class DummyCar
{
    private final String model;

    public DummyCar(String model)
    {
        this.model = model;
    }

    @Override
    public String toString()
    {
        return "DummyCar{model='" + model + "'}";
    }
}
