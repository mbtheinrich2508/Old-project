function root = ex4_5(f, a, b)
    top = a;
    ftop = f(a);
    bottom = b;
    fbottom = f(b);
    eps = 1.0e-6;
    % Find the 1 root 4 equation fx
    while (bottom - top) > eps
        x = (top + bottom)/2;
        fx = f(x);
    
        if (sign(fx) == sign(ftop))
            top = x;
            ftop = fx;
        else
            bottom = x;
            fbottom = fx; % a bit useless here
        end
    end
    x
end