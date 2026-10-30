function [] = plotCircle(radius, center)
x0 = center(1);
y0 = center(2);
phi = linspace(0, 2*pi, 500);
x = radius*cos(phi);
y = radius*sin(phi);
A = radius^2*pi;
fprintf('A = %.2f cm^2\n', A)

hold on
plot([-15 15],[0 0],'black') % x-axis
plot([0 0],[-15 15],'black') % y-axis
plot(x+x0, y+y0, 'red', 'LineWidth', 1.5)
plot(x0, y0, 'blue.', 'MarkerSize', 25)
axis([-15 15 -15 15])
axis square % Important: make the graph in ratio 1:1, so that the circle looks circular
grid on
xlabel('x(cm)')
ylabel('y(cm)')
hold off