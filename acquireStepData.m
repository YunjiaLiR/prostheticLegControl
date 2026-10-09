function [t, y, u] = acquireStepData(port, durationSeconds)
%ACQUIRESTEPDATA Capture time, accelerometer tilt and commanded PWM.
% Example: [t,y,u] = acquireStepData("COM3", 10);
% Find the Arduino port with serialportlist("available").
% Start capture, then reset the Arduino to capture the complete one-shot test.
if nargin < 2
    durationSeconds = 10;
end
validateattributes(durationSeconds, {'numeric'}, {'scalar','real','finite','positive'});
sp = serialport(port, 115200);
configureTerminator(sp, "LF");
sp.Timeout = 0.5;
cleanup = onCleanup(@() delete(sp)); %#ok<NASGU>
flush(sp);
t = []; y = []; u = [];
fprintf('Capturing for %.1f s. Reset the Arduino now.\n', durationSeconds);
started = tic;
while toc(started) < durationSeconds
    if sp.NumBytesAvailable == 0
        pause(0.001);
        continue;
    end
    ln = readline(sp);
    if isempty(ln)
        continue;
    end
    fields = split(strtrim(ln), ',');
    if numel(fields) ~= 3
        continue; % Ignore sensor startup/status messages.
    end
    values = str2double(fields);
    if any(~isfinite(values))
        continue;
    end
    t(end+1) = values(1); %#ok<AGROW>
    y(end+1) = values(2); %#ok<AGROW>
    u(end+1) = values(3); %#ok<AGROW>
end
if isempty(t)
    warning('No valid samples received. Check the port and reset timing.');
    return;
end
save('step_data.mat', 't', 'y', 'u');
figure;
subplot(2,1,1);
plot(t,y); grid on;
ylabel('Accelerometer tilt (deg)'); xlabel('Time (s)');
subplot(2,1,2);
plot(t,u); grid on;
ylabel('Commanded PWM'); xlabel('Time (s)');
end
