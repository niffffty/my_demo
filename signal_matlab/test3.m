%傅里叶变换
%matlab自带fourier与ifourier函数实现傅里叶变换/逆变换

%例3.1
syms t;
F1=fourier(t*heaviside(t));     %F1=t*u(t)的傅里叶变换
F2=fourier(sin(t));             %F2=sin(t)的傅里叶变换

%例3.2
%用傅里叶变换求-1/2到1/2的矩形脉冲的频谱；再恢复到时域后，对比两次图像

%傅里叶变换的公式是连续的，matlab无法进行连续的运算，所以需要先将f(t)离散化(将每个点化为f(t1+n(&t)))
%得到F(w)后也是连续的，再离散化

%方法一：直接计算法

T=2;
N=200;
t=linspace(-T/2,T/2-T/N,N);     %为什么要减T/N?  => 为了达到等0.01的等间距，从-1到0.99分成200份，每份递增0.01
%或 t=-T/2:T/N:T/2-T/N;

% f=0*t;              %创造u(t)
f=zeros(size(t));
f(t>-1/2&t<1/2)=1;
% plot(t,f);

OMG=16*pi;          %创造空的F(w)
K=100;
omg=linspace(-OMG/2,OMG/2-OMG/K,K);

%核心：对F(w)赋值
%对f(t)上的200个点求和，最终得到F(w)上的一个数
% F=0*omg;
F=zeros(size(omg));
for k = 1:K
    for n=1:N            %两个循环：内循环是求F(w)中的某一个数(在时域上求和，0到n-1)，
        F(k)=F(k)+T/N*f(n)*exp(-1j*omg(k)*t(n));   %用到式-3.2：当n=1,F(1)=F(1)+T/N*f1*exp(j*omg(1)*t(1))；c从0到N累加后,得到一个F(w)
    end                                            %外循环把累加后的每一个值赋值到F(w)
end

% plot(omg,F);

%恢复F(w)到ft，从频域再恢复到时域
% fs=0*t;
fs=zeros(size(t));
for n = 1:N
    for k = 1:K
        fs(n)=fs(n)+OMG/2/pi/K * F(k)*exp(1j*omg(k)*t(n));   %式3.4
    end
end
% plot(t,f,t,fs);


%方法二：矢量计算法
%用内积实现式3.2中的累加和，即一个行矢量左乘一个列矢量
F=zeros(size(omg));
for k =1:K
%   F(k)=T/N*exp(-1j*omg(k)*t).'*f;     %指导书里这么写的
    F(k)=T/N*exp(-1j*omg(k)*t)*f.';     %实际应改成这个
end

%恢复到fs
fs=zeros(size(t));
for n=1:N
    fs(n)=OMG/2/pi/K*(exp(1j*omg*t(n))*F.');
end
plot(omg,F);
plot(t,fs);

%方法三：矩阵计算法
%公式简化为3.9与3.10，但是如何有效的构造矩阵W？   =>matlab提供了kron函数（张量积）

U=exp(-1j*kron(omg,t.'));
F=T/N*(U.*f);





surf(omg, t, real(F));  % 实部部分的波形
title('F(w, t)波形');
xlabel('频率 (w)');
ylabel('时间 (t)');
zlabel('幅度');

V=exp(1j*kron(t,omg.'));
fs=OMG/2/pi/K*V*F;






