
% Name the variables

m_patient = 30; % kg
m_bed = 10; % kg
m_payload = m_bed + m_patient; % kg

bed_pos_initial = 0;

max_offset = 0.1; % m

k_spring2 = 20000; % N/m
damp_spring2 = 100; % whatever

m_curve = 5; % kg

k_spring1 = 8000; % N/m
damp_spring1 = 1000; % whatever
spring1_max_disp = 0.2; % m

linear_act_speed = 0.1; % m/s
linear_act_max = 0.25; % m
linear_act_delay = 0.1; % s

DC_max_force = 400; % N
DC_delay = 0.005; % s

noise_power = 0.003;
noise_rate = 0.1;

noise_seed = 113314;
noise_start_time = 3; % s
patient_sit_time = 1; % s

DC_PID_P = 0;
DC_PID_I = 0;
DC_PID_D = 0;

sim('model1_sim')

figure
hold on
plot(baseline + bed_pos + 0.2)
plot(baseline + curve_pos + 0.1)
plot(baseline)
plot(DC_force/1000)
plot(lin_act_pos)
legend('bed pos', 'curve pos', 'platform', 'DC Force', 'lin act pos', 'location', 'best')
title('With actual position')

figure
hold on
plot(bed_pos + 0.2)
plot(curve_pos + 0.1)
plot(baseline)
plot(DC_force/1000)
plot(lin_act_pos)
legend('bed pos', 'curve pos', 'platform', 'DC Force', 'lin act pos', 'location', 'best')
title('With relative position')

figure
hold on
plot(DC_force)
plot(spring1_force)
plot(F_curve)
plot(F_spring2)
legend('DC force', 'spring 1 force', 'curve force', 'spring 2 force', 'location', 'best')
