function Collatz = ex4_3()
    n = 499999;
    for k=1:n
        count = 1;
        temp = k;
        while k~=1,
            if (mod(k,2))==0
                k=k/2;
            else k=3*k+1;
            end
            count=count+1;
        end
        v(temp) = count;
    end
    find(v==max(v))
end