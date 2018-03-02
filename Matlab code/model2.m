
%close all

% Name the variables

m_patient = 40; % kg
m_bed = 8; % kg
m_payload = m_bed + m_patient; % kg

feet_delay = 0.1; % s
max_offset = 0.1; % m

k_spring = 2500; % N/m
damp_spring = 1; % whatever

k_cutoffspring = 20000; % N/m

linear_act_speed = 0.05; % m/s
linear_act_max = 0.5; % m
linear_act_delay = 0.1; % s

DC_max_force = 800; % N
DC_delay = 0.005; % s

noise_power = 0.01;
noise_rate = 0.1;
noise_rate_limit = 10;
noise_seed = 113134;

noise_start_time = 3; % s
patient_sit_time = 0.5; % s
patient_sit_pace = 50; % kg/s


P = 0;
I = 0;
D = 840;
N = 104;

reduction = runsim([P, I, D, N])

%fminsearch(@runsim, [P; I; D; N])

% figure
% plot(bed_angle)
% title('Bed angle')

% figure
% hold on
% plot(bed_pos_abs + 0.2)
% plot(platform_pos)
% plot(lin_act_pos)
% plot(pos_reset)
% legend('Head pos', 'platform', 'lin act pos', 'RESET', 'location', 'best')
% title('Head, actual position')
% 
% % figure
% % hold on
% % plot(bed_pos_abs2 + 0.2)
% % plot(platform_pos2)
% % plot(lin_act_pos2)
% % plot(pos_reset2)
% % legend('Feet pos', 'platform', 'lin act pos', 'RESET', 'location', 'best')
% % title('Feet, actual position')
% 
% figure
% hold on
% plot(bed_pos_abs - platform_pos + 0.2)
% plot(platform_pos)
% plot(lin_act_pos)
% plot(pos_reset)
% legend('bed pos', 'platform', 'lin act pos', 'RESET', 'location', 'best')
% title('Head, relative position')
% 
% % figure
% % hold on
% % plot(bed_pos_abs2 - platform_pos2 + 0.2)
% % plot(platform_pos2)
% % plot(lin_act_pos2)
% % plot(pos_reset2)
% % legend('bed pos', 'platform', 'lin act pos', 'RESET', 'location', 'best')
% % title('Feet, relative position')
% 
% figure
% hold on
% plot(DC_force)
% plot(spring_force)
% plot(-1 * payload_weight)
% plot(bed_force)
% plot(cutoff_force)
% legend('DC force', 'spring force', 'bed + patient weight', 'total force to bed', 'cutoff force', 'location', 'best')


