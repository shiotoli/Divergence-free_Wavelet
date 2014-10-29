function [ div ] = divergence( u,v )
%DIVERGENCE Summary of this function goes here
%   Detailed explanation goes here
div = zeros(size(u,1),size(u,2));
for i = 2:size(u,1)-1
    for j = 2:size(v,1)-1
        div(i,j)=(u(i,j)-u(i,j-1)+u(i-1,j)-u(i-1,j-1)+v(i,j)-v(i-1,j)+v(i,j-1)-v(i-1,j-1))/2;
    end
end
end

