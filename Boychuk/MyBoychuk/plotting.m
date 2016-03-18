clear;
load x.dat
load psi.dat
load u.dat
load t.dat

plot(t,x-psi)
plot(t,x)
plot(t,u)
grid on;
xlabel('t'); 
ylabel('f(t)');
ylabel('x(t)');
ylabel('u(t)');