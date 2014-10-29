a = zeros(8,8);
for i = 1:8
    for j = 1:8
        a(i,j)=i*(j);
    end
end
[C S] = wavedec2(a,2,'rbio2.2');
LL=reshape(C(1:25),5,5);
LH=reshape(C(26:50),5,5);
HL=reshape(C(51:75),5,5);
HH=reshape(C(76:100),5,5);
[LH_c,HL_c,HH_c] = detcoef2('all',C,S,2); 
% C(109:144)= C(109:144)*10;
X = waverec2(C,S,'rbio2.2');

lscdf = liftwave('rbio2.2');
displs(lscdf);
[LoD,HiD,LoR,HiR] = ls2filt(lscdf);
bswfun(LoD,HiD,LoR,HiR,'plot');