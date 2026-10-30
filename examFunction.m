function [] = examFunction()
x=[1:9];
y=0;
for k=x(2):x(length(x)-1);
    y=y+ k;
end
end
