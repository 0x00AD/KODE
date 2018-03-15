function [out] = runsim( PIDN )
P = PIDN(1);
I = PIDN(2);
D = PIDN(3);
N = PIDN(4);
set_param('model2_sim/Head/Primary_motor/mainpid','P',num2str(P))
set_param('model2_sim/Head/Primary_motor/mainpid','I',num2str(I))
set_param('model2_sim/Head/Primary_motor/mainpid','D',num2str(D))
set_param('model2_sim/Head/Primary_motor/mainpid','N',num2str(N))

sim('model2_sim')

% Find and print the few biggest acceleration peaks
max_bed_a = sort(findpeaks(abs(bed_acc.data)));
%acc_peaks = max_bed_a(end-10:end)
max_plat_a = sort(findpeaks(abs(platform_acc.data)));
%acc_peaks2 = max_plat_a(end-4:end)
%acc_peaks2 = max_plat_a(end)
%max_plat_a = max(abs(platform_acc.data))
out = max_bed_a(end)/max_plat_a(end)
end

