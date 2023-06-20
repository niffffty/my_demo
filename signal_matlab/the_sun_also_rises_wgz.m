%5.4  用音符库任意表示一篇乐谱
Fs=44100;
Ts=1/Fs;
t=0:Ts:0.5;
K=30;          
k=0:K-1;         
f=220*2.^(k/12);
mas=sin(2*pi*kron(f',t));


lalow=mas(6,:);  %6.
cilow=mas(8,:);  %7.
do=mas(9,:);     %1
re=mas(11,:);    %2
mi=mas(13,:);    %3
fa=mas(14,:);    %4
sol=mas(16,:);   %5
la=mas(18,:);    %6
ci=mas(20,:);    %7
dohigh=mas(21,:);

%太阳照常升起-久石让
music_me1=[lalow mi mi mi fa mi mi mi fa sol la la sol sol mi mi mi];
music_me2=[music_me1 lalow re re re mi re re mi mi sol mi sol sol cilow do lalow lalow lalow lalow lalow lalow];
music_me3=[music_me2 do mi la sol la la la sol la sol la la sol sol mi mi mi mi mi re la sol mi re mi mi mi mi re la fa];
music_me4=[music_me3 mi re mi sol ci do lalow lalow lalow lalow];
sound(music_me4,Fs);

