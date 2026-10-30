clear, clc;
%{
6.1
x = 0:0.01:1;
y1 = sin(2*pi*x);
y2 = cos(2*pi*x);

subplot(2, 2, 1)
plot(x, y1, 'red')
subplot(2, 2, 2)
plot(x, y2)


%6.2
A = load ('03_02magneticField.dat', '-ascii');
B = load('03_02magneticField_2.dat', '-ascii');
C = load('03_02magneticField_withPP.dat', '-ascii');
regr = C(:,1)\C(:,2)
plot(C(:,1),C(:,2),'r.', 'MarkerSize',20)

hold on
plot(A(:,1),A(:,2),'bo')
plot(B(:,1), B(:, 2), 'k*')

fplot(@(x) regr*x)
title('Characterisation of split coil system')
xlabel('current (A)')
ylabel('B field (mT)')
hold off

le = legend('with pole pieces', 'without pole pieces', 'with new powersupply', 'linear fit'); %labels
text(25, 5, 'r, z = 0')
axis([0 32 0 55]) % x:0->32, y:0->55
%}

figure(1)
[x y] = meshgrid(-2:0.1:2);
%z = x.^2 + y.^2;
fsurf(@(x, y) x.^2 + y.^2)
figure(2)
fmesh(@(x, y) x.^2 + y.^2)
figure(3)
fcontour(@(x, y) x.^2 + y.^2)
figure(4)
fsurf(@(x, y) 1./sqrt(x.^2+y.^2))
axis([-0.5 0.5 -0.5 0.5 -1 30])

%{
t=clock; 
fprintf('%02.0f:%02.0f:%02.0f\t%02.0f:%02.0f:%02.0f\n', t(4), t(5), t(6), t(3), t(2), t(1))

theta = 0: pi/40 : 2*pi;
polar(theta, sin(2*theta))
subplot(2, 2, 4)
t = 0:pi/20:10*pi;
comet3(exp(-0.02*t).*sin(t), exp(-0.02*t).*cos(t), t)
%}