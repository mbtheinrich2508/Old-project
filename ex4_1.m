function e = ex4_1(n)
    success = -1;, error = -1;, deviation = inf;, e_approx = -1;
    if n<1,
        success = 0
        error = 1
        return
    else
        error=0
        fac = [1:n];
        e=2;
        fac(1)=1;
        for k=2:n,
            fac(k)=fac(k-1)*k;
            e = e + 1/fac(k);
            deviation = 1/fac(k), e_approx = e
            if deviation < 1e-6,
                success = 1
                return
            end
        end

        if deviation > 1e-6,
            success = 0
        end
    end
end
   