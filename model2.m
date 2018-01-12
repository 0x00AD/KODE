
%close all

% Name the variables

m_patient = 30; % kg
m_bed = 10; % kg
m_payload = m_bed + m_patient; % kg

max_offset = 0.1; % m

k_spring = 2000; % N/m
damp_spring = 100; % whatever

k_cutoffspring = 20000; % N/m

linear_act_speed = 0.1; % m/s
linear_act_max = 0.5; % m
linear_act_delay = 0.1; % s

DC_max_force = 200; % N
DC_delay = 0.005; % s

noise_power = 0.01;
noise_rate = 0.1;
noise_rate_limit = 10;
noise_seed = 113134;

noise_start_time = 3; % s
patient_sit_time = 1; % s
patient_sit_pace = 50; % kg/s

DC_PID_P = 0;
DC_PID_I = 0;
DC_PID_D = 1000;
DC_PID_N = 100;

sim('model2_sim')

max_bed_a = sort(findpeaks(abs(bed_acc.data)));
max_bed_a(end-10:end)
max_plat_a = max(abs(platform_acc.data))

figure
hold on
plot(bed_pos_abs + 0.2)
plot(platform_pos)
plot(lin_act_pos)
plot(pos_reset)
legend('bed pos', 'platform', 'lin act pos', 'RESET', 'location', 'best')
title('With actual position')

figure
hold on
plot(bed_pos_abs - platform_pos + 0.2)
plot(platform_pos)
plot(lin_act_pos)
plot(pos_reset)
legend('bed pos', 'platform', 'lin act pos', 'RESET', 'location', 'best')
title('With relative position')

figure
hold on
plot(DC_force)
plot(spring_force)
plot(-1 * payload_weight)
plot(bed_force)
plot(cutoff_force)
legend('DC force', 'spring force', 'bed + patient weight', 'total force to bed', 'cutoff force', 'location', 'best')


