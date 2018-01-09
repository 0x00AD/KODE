
% Name the variables

m_patient = 80; % kg
m_bed = 20; % kg
m_payload = m_bed + m_patient; % kg

bed_pos_initial = 0;

max_offset = 0.2; % m

k_spring2 = -20000; % N/m
damp_spring2 = -1000; % whatever

m_curve = 5; % kg

k_spring1 = 800; % N/m
spring1_max_disp = 0.2; % m

linear_act_speed = 0.01; % m/s
linear_act_max = 1; % m
linear_act_delay = 0.1; % s

DC_max_force = 400; % N
DC_delay = 0.005; % s

DC_PID_P = 2;
DC_PID_I = 0;
DC_PID_D = 2;

sim('model1_sim')

figure
hold on
plot(bed_pos + 1)
plot(curve_pos + 0.5)
plot(baseline)
legend('bed pos', 'curve pos', 'platform', 'location', 'best')

figure
hold on
plot(DC_force)
plot(spring1_length)
plot(F_curve)
plot(F_spring2)
legend('DC force', 'spring1 length', 'curve force', 'spring 2 force', 'location', 'best')
