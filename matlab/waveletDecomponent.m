u = load('uelocity.txt');
v = load('velocity.txt');
lscdf = liftwave('cdf3.1');
displs(lscdf);
[LoD,HiD,LoR,HiR] = ls2filt(lscdf);
bswfun(LoD,-HiD,LoR,-HiR,'plot');
[C,S] = wavedec2(u,1,'haar');