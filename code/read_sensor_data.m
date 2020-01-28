%print received data
n = 30;
fprintf('\n');
combined = zeros(1, 2);
flushinput(b);

for i=1:10000
    read = fread(b, 2*n);
    %for j=1:n
    %    fprintf('%4d ', j);
    %end
    %fprintf('\n');
    for j=1:n-1
        combined(1) = read(2*j-1,1);
        combined(2) = read(2*j,1);
        fprintf('%4d ', typecast(uint8(combined), 'uint16'));
    end
    combined(1) = read(2*n-1,1);
    combined(2) = read(2*n,1);
    fprintf('%4d\n', typecast(uint8(combined), 'uint16'));
end