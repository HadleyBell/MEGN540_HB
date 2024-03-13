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

x_l_all = [];
x_r_all = []; 
pwm_value = [100,200,300,400,500]; 
% forward reverse 
for i = 1:5
    x_l = zeros(1,17);
    x_r = zeros(1,17); 
    x_l(1,2) = pwm_value(i);
    x_r(1,2) = pwm_value(i);
    x_l(1,3) = 0;
    x_r(1,3) = 0;
    x_l(1,4) = -pwm_value(i);
    x_r(1,4) = -pwm_value(i);
    x_l(1,5) = 0;
    x_r(1,5) = 0;
    x_l(1,6) = pwm_value(i);
    x_r(1,6) = pwm_value(i);
    x_l(1,7) = -pwm_value(i);
    x_r(1,7) = -pwm_value(i);
    x_l(1,8) = 0;
    x_r(1,8) = 0;
    x_l(1,9) = -pwm_value(i);
    x_r(1,9) = -pwm_value(i);
    x_l(1,10) = pwm_value(i);
    x_r(1,10) = pwm_value(i);
    x_l(1,11) = -pwm_value(i);
    x_r(1,11) = -pwm_value(i);
    x_l(1,12) = 0;
    x_r(1,12) = 0;
    % left right 
    x_l(1,13) = pwm_value(i);
    x_r(1,13) = -pwm_value(i);
    x_l(1,14) = 0;
    x_r(1,14) = 0;
    x_l(1,15) = -pwm_value(i);
    x_r(1,15) = pwm_value(i);
    x_l(1,16) = -pwm_value(i);
    x_r(1,16) = 0;
    x_l(1,17) = 0;
    x_r(1,17) = 0;

    x_l_all = [x_l_all, x_l];
    x_r_all = [x_r_all, x_r];

end 

x_all = [x_l_all',x_r_all']

% forward step 
% stop 
% forward step 
% reverse step 
% stop 
% reverse step
% forward step 
% stop 
% turn left 
% stop 
% turn right 
% turn left 
% stop 
% turn left 
% turn right 
% turn left 
% stop 

% forward 1/4 step 
% forward 2/4 step 
% forward 3/4 step 
% forward 4/4 step
% forward 3/4 step 
% forward 2/4 step 
% forward 1/4 step 
% reverse 1/4 step 
% reverse 2/4 step 
% reverse 3/4 step 
% reverse 4/4 step
% reverse 3/4 step 
% reverse 2/4 step 
% reverse 1/4 step 

%% 
q_in_ms = 2; 
top = {'cfc','Q',[q_in_ms],''};
bottom = {'cfc','Q',[-1],''};
stop = {'ccc','s','',''};
C    = cell(length(x_all), 1);
C(:) = {'chh'};
p    = cell(length(x_all), 1);
p(:) = {'p'};
% x_cell = num2cell(int32(x));
x_cell = num2cell(x_all); 
out = [C,p, x_cell(:,1),x_cell(:,2)];
out = [top; out; bottom; stop];
writecell(out,'data_3.csv')



%% read in test resutls 
input = readmatrix('Sim_Results_Test_1.csv');
input = input(1:end-1,:);

time = input(:,2);
time(isnan(time)) = []; 
left_pwm = input(:,3);
left_pwm(isnan(left_pwm)) = [];
right_pwm = input(:,4);
right_pwm(isnan(right_pwm)) = [];
left_encoder = input(:,5); 
left_encoder(isnan(left_encoder)) = [];
right_encoder = input(:,6); 
right_encoder(isnan(right_encoder)) = [];
dt = 0.002;

close all
figure(1);
subplot(2,1,1)
hold on 
plot(input(:,2),input(:,3))
plot(input(:,2),input(:,4))
legend("Left","Right")
subplot(2,1,2)
hold on 
plot(input(:,2),input(:,5))
plot(input(:,2),input(:,6))
legend("Left","Right")

figure(2);
subplot(2,1,1)
hold on 
plot(time,left_pwm)
plot(time,right_pwm)
legend("Left","Right")
subplot(2,1,2)
hold on 
plot(time,left_encoder)
plot(time,right_encoder)
legend("Left","Right")


%% 
% systemIdentification
% first 
% left 
G_L_1 = 240.1; 
B_L_1 = 80.0;

sys_1 = tf(G_L_1,[1,B_L_1,0]); 

time = linspace(0,length(input)*dt,length(left_pwm));

sim_out = lsim(sys_1, left_pwm, time);

figure
plot(time,sim_out)
hold on
plot(time,left_encoder);
legend("Plant Output","Measured Output")
xlabel("Time (s)");
ylabel("Output (^{\circ})");
title("Measured Output and Plant Output vs. Time")
