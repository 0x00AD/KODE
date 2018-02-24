
ax_ = ax ./ 1962;
ay_ = ay ./ 1962;
az_ = az ./ 1962;

offset = 1;
ax_ = ax_(offset:end);
ay_ = ay_(offset:end);
az_ = az_(offset:end);
ax_(isnan(ax_))=0;
ay_(isnan(ay_))=0;
az_(isnan(az_))=0;

azt = [];%timeseries(az_(1:5000), zeros(5000, 1) + 0.01)
count = 200;
for i = 1:count:5000
    avgx = sum(ax_(i:i+count))/count;
    avgy = sum(ay_(i:i+count))/count;
    avgz = sum(az_(i:i+count))/count;
    length = sqrt(avgx.^2+avgy.^2+avgz.^2);
    length
    azt = [azt; az_(i:i+count) - avgz*0.94/length];
end
azt = sqrt(ax_.^2+ay_.^2+az_.^2)-1;
azt(isnan(azt))=0;
azt2 = timeseries(azt(1: 5000), (1:5000')./100);

total = sqrt(ax_.^2 + ay_.^2 + az_.^2);
total(isnan(total))=0;
total2 = timeseries(total(1: 5000), (1:5000')./100);

%totalv = integrate(total)
figure
plot(total)
figure

plot(azt)