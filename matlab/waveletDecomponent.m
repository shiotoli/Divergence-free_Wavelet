u = load('uelocity.txt');
v = load('velocity.txt');
[Ld,Hd,Lr,Hr] = wfilters('bio2.2','d');
bswfun(LoD,-HiD,LoR,-HiR,'plot');
% [C,S] = wavedec2(u,1,'bio2.2');