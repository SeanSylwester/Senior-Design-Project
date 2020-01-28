%% create a bluetooth object
fprintf('\n\nConnecting...\n');
b = Bluetooth('ece34_S', 1);
fprintf('Connected! Opening...\n');
fopen(b);
fprintf('Opened!\n');
b.Timeout = 1;
%%
fclose(b)
fprintf('Closed!\n');
pause(1);
fopen(b);
fprintf('Opened!\n');
%%
fclose(b)
fprintf('Closed!\n');
