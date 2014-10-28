u = load('uelocity.txt');
v = load('velocity.txt');
[Ld0,Hd0] = wfilters('rbio2.2','d');
Hd0 = - Hd0;
[Ld1,Hd1] = wfilters('rbio3.1','d');
Hd1 = - Hd1;

% bswfun(Ld0,Hd0,Ld1,Hd1,'plot');
% dwt2(u,'rbio2.2');
level = 8;
[C,S,baseu,basev] = wavedec2_divergence(u,v,8,Ld0,Hd0,Ld1,Hd1);
for i = 1:level
    d = zeros(6,size(C{i},4),size(C{i},4));
    d0=C{i}(2,2,1);
    d1=C{i}(1,1,2);
    d2=C{i}(1,2,2)/2.0-C{i}(2,2,2)/2.0;
    d3=C{i}(1,2,1)+C{i}(2,2,1)/4.0-circshift(C{i}(2,2,1),[0 1])/4.0;
    d4=C{i}(2,2,1)+C{i}(1,1,2)/4.0-circshift(C{i}(1,1,2),[1 0])/4.0;
    d5=C{i}(1,2,2)/2+C{i}(2,2,2)/2;
    path = sprintf('../mathematica/u_%d_HL.txt',i);
    save(path,'d0', '-ascii', '-double');
    path = sprintf('../mathematica/u_%d_LH.txt',i);
    save(path,'d1', '-ascii', '-double');
    path = sprintf('../mathematica/u_%d_HH.txt',i);
    save(path,'d2', '-ascii', '-double');
    path = sprintf('../mathematica/v_%d_HL.txt',i);
    save(path,'d3', '-ascii', '-double');
    path = sprintf('../mathematica/v_%d_LH.txt',i);
    save(path,'d4', '-ascii', '-double');
    path = sprintf('../mathematica/v_%d_HH.txt',i);
    save(path,'d5', '-ascii', '-double');
end
% plot(C);
% bswfun(LoD,-HiD,LoR,-HiR,'plot');
% [C,S] = wavedec2(u,1,'bio2.2');
save('../mathematica/baseu.txt','baseu', '-ascii', '-double');
save('../mathematica/basev.txt','basev', '-ascii', '-double');