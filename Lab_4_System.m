%% Lab4 System Identification Zumo Car 
clc
clear all
close all

t_int = 0.01; 
t_final = 10; 
t_range = 0:t_int:t_final-t_int; % (s) 


a = 500; 

figure(1);


f = 0.1;                     % hertz
x = a*sin(2*pi*f*t_range.^2);
% Plot the signal versus time:
figure;
plot(t_range,x);
xlabel('time (in seconds)');
title('Signal versus Time');
zoom xon;


%% 
q_in_ms = 2; 
top = {'cfc','Q',[q_in_ms],''};
bottom = {'cfc','Q',[-1],''};
stop = {'ccc','s','',''};
C    = cell(length(t_range), 1);
C(:) = {'chh'};
p    = cell(length(t_range), 1);
p(:) = {'p'};
x_cell = num2cell(int32(x));
out = [C,p, x_cell',x_cell'];
out = [top; out; bottom; stop];
writecell(out,'data.csv')