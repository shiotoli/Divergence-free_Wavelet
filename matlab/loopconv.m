function y = loopconv( a,b,startid)
%LOOPCONV Summary of this function goes here
%   Detailed explanation goes here
y = zeros(1,size(a,2));
i1 = mod((5-startid-1), 4)+1;
i2 = mod((6-startid-1),4)+1;
i3 = mod((7-startid-1),4)+1;
i4 = mod((8-startid-1),4)+1;
for i = 1:size(a,2)
    j1=i;
    j2=mod(i+1-1,size(a,2))+1;
    j3=mod(i+2-1,size(a,2))+1;
    j4=mod(i+3-1,size(a,2))+1;
    y(j1)=a(j1)*b(i1)+a(j2)*b(i4)+a(j3)*b(i3)+a(j4)*b(i2);
%     y(2)=a(2)*b(i1)+a(3)*b(i4)+a(4)*b(i3)+a(1)*b(i2);
%     y(3)=a(3)*b(i1)+a(4)*b(i4)+a(1)*b(i3)+a(2)*b(i2);
%     y(4)=a(4)*b(i1)+a(1)*b(i4)+a(2)*b(i3)+a(3)*b(i2);
end
end

