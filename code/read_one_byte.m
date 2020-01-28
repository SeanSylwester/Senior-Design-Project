fprintf('\n');
combined = zeros(1, 2);
flushinput(b);

for i=1:10000
    read = fread(b, 2);
    combined(1) = read(1,1);
    combined(2) = read(2,1);
    fprintf('%4d\n', typecast(uint8(combined), 'uint16'));
end