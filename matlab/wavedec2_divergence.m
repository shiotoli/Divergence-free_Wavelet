function [d,s,basex,basey] = wavedec2_divergence(x,y,n,Ld0,Hd0,Ld1,Hd1)
% d{level}(pos{x,y},epsilon_0,epsilon_1,sizeof(x))
d = cell(n+1,1);
sx =  size(x);
s = zeros(n+2,length(sx));
if isempty(x) , return; end

s(end,:) = size(x);
for i=1:n
    [x,h1,v1,d1] = dwt2_df(x,Ld1,Hd1,Ld0,Hd0);
    [y,h2,v2,d2] = dwt2_df(y,Ld0,Hd0,Ld1,Hd1);
    d{i} = zeros(2,2,2,size(h1,1),size(h1,2));
    d{i}(1,1,2,:)= h1(:);
    d{i}(1,2,1,:)= v1(:);
    d{i}(1,2,2,:)= d1(:);
    d{i}(2,1,2,:)= h2(:);
    d{i}(2,2,1,:)= v2(:);
    d{i}(2,2,2,:)= d2(:);
    s(n+2-i,:) = size(x);
end
basex = x;
basey = y;
s(1,:) = size(x);



