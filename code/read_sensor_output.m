%print received data
n = 5;
fprintf('\n');
flushinput(b);

for i=1:10000
    dummy = fread(b,1);
    read = fread(b, n-1);
    fprintf('%5d: ', i);
    for j=1:n-2
        fprintf('%4d ', mod(read(j), 16));
        fprintf('%4d ', floor(read(j) / 16));
    end
    fprintf('%4d ', mod(read(n-1), 16));
    fprintf('%4d\n ', floor(read(n-1) / 16));
end