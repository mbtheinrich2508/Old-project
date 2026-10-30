A = rand(8,8);
for i = 1:numel(A)
    if A(i) < 0.5
        A(i) = 0;
    end
end

%2nd:
i = 1;
while i<numel(A)
    if A(i)>0.5
        A(i)=1;
    end
    i=i+1;
end

%MultiMoodle
a=5; b=7; c=1;
sum=0;
while a*c<2500
    sum=sum+a*c;
    c=c+1;
end
c=1;
while b*c<2500
    sum=sum+b*c;
    c=c+1;
end


function [area, perimeter] = rectangleProperties(l, w)
    area = l * w;
    perimeter = 2 * (l + w);
end