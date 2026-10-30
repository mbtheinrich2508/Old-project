function ex6_5(number)
hold on
for k = 1:number
    if mod(k,2) == 0
        color ='b.';
    else
        color = 'g.';
    end
    plot(-[1:k], -k, color, 'MarkerSize', number*5)
    plot(-k, -[1:k], color, 'MarkerSize', number*5)
end

box on
axis([-number-2 1 -number-2 1])
axis square
h = gca; %Get current axes handle
h.XTick = [];
h.YTick = [];
hold off
clear, clc;
end