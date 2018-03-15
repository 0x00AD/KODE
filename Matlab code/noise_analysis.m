
% Make Markov chain, analyse the transfer probabilities for each pair
% Make the probabilities symmetric, effectively eliminating tilt errors
% Use the transfer probabilities to generate new data

% az is the acceleration
accuracy = 1024;
% (magicnumber is 1 g acceleration)
magicnumber = 1962;

Markov = zeros(accuracy, accuracy);

az(isnan(az)) = 0;

% The range, within which the values fall (from -range to range)
range = 3 * magicnumber;
%range = max(max(az), abs(min(az)));

% Normalize the values to scale between 0, accuracy
az_ = floor((az + range) * accuracy / (2 * range));
az_(az_ > 2 * range) = 2 * range;
az_(az_ < 0) = 0;

Markov(:, accuracy/2) = 1;

% Increment the values in data for each transition
for i = 1:length(az_)-1
    Markov(az_(i), az_(i+1)) = Markov(az_(i), az_(i+1)) + 1;
end

% Balance the data (WIP?)
Markov = Markov + fliplr(Markov);
% create the markov chan (needs the toolbox installed)
mc = dtmc(Markov);

% Plots the markov chain
% graphplot(mc)

% simulate the Markov chain
mcSimOut = simulate(mc, 3000);

% Normalize to acceleration
mcSimOut = (mcSimOut ./ accuracy .* (2*range) - range) ./ magicnumber;
% Remove gravity, convert to m/s^2
mcSimOut = (mcSimOut - 1) ./ 9.81;

% Create a 30-second time series
noiseTimeSeries = timeseries(mcSimOut, 0:0.01:30);

