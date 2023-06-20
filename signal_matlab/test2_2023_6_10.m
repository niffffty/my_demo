%第一章第五节

%阶跃信号
%使用heaviside(x)函数，返回一个阶跃函数，参数x为定义域，heaviside函数把x区间大于0的数变为1，将x等于0的值赋为NaN(非数值)
%heaviside源码：
% Y = zeros(size(X)); % 定义和 X 结构相同的全零变量
% Y(X > 0) = 1; % 定义对应于 X 大于 0 的 Y 为 1
% Y(X == 0) = NaN; % X 等于零时 Y 非数值

t=-1:0.001:1;
ut=heaviside(t);
plot(t,ut);


%冲激信号(dirac信号)
%源码：和阶跃信号相同，创建一个与x相同结构的变量，将t等于0的值赋为Inf(无穷大)
t=-1:0.001:1;
tt=dirac(t);
plot(t,tt);


%第六节，其他matlab命令

%一.矩阵(三种表达方式)

%1.创建一个等比行向量，用reshape转化成矩阵
x = linspace(0,9,9);
B = reshape(x,3,3);

%2.用zeros函数或ones函数创建矩阵
C=zeros(3,3);%3*3全零矩阵
D=ones(3,3); %3*3全1矩阵

%3.单位矩阵，eye()
G=eye(3);    %3*3的单位矩阵
elmat






%第二章
%第一节：创建LTI系统模型(线性时不变)
%描述一个系统（用传递函数）
a=[1 2 3 4];
b=1;
sys=tf(b,a);
sys;            %输出为一个传递函数，matlab以传递函数的形式描述LTI系统(线性时不变系统)



%描述函数（状态方程+输出方程）
A=[-2,0,-1;0,-3,3;2,-2,0];%定义四个矩阵
B=[1,0;0,-3;0,0];
C=[0,1,0];
D=[0,1];
sys=ss(A,B,C,D);
sys





%第二节：微分方程式
%微分方程的解：齐次解+特解

%齐次解=系统特征方程的根，用roots函数计算
%例2.3
p=[1 7 16 12];  %定义多项式
a=roots(p);     %定义多项式p=0的根a
a;


%特解=系统函数在给定激励信号作用下的输出，用lsim函数进行仿真
%例2.4
a=[1 2 3];
b=[1 1];
sys=tf(b,a);            %描述系统
t=0:0.1:10;           %仿真时间：0到10秒(间隔0.1s)

e1=t.^2;                %定义激励信号e1
r1=lsim(sys,e1,t);      %用e1激励系统sys，输出为r1

e2=exp(t);              %定义激励信号e2
r2=lsim(sys,e2,t);      %用e2激励系统sys，输出为r2

subplot(2,2,1);         %绘制e1,e2的输入、输出r1,r2波形
plot(t,e1);
ylabel('输入:e1');
subplot(2,2,2);
plot(t,r1);
ylabel('输出:r1');
subplot(2,2,3);         
plot(t,e2);
ylabel('输入:e2');
subplot(2,2,4);
plot(t,r2);
ylabel('输出:r2');





%信号输入系统，输出信号
a=[1 2 3];
b=[1 1];
% sys=tf(b,a);
t=0:0.01:10;
e1t=t.^2;       %输入信号
e2t=exp(t);

sys=tf(b,a);    %系统
r1t=lsim(sys,e1t,t);
subplot(1,2,1);
plot(t,e1t,t,e2t);
subplot(1,2,2);
plot(t,r1t);
% r2t=;

%冲击响应与阶跃响应






%卷积
T=  0.01;
t=  -1:T:4;
e=  (t>-1/2&t<1);        %矩形脉冲
h=  (t>0&t<2).*t/2;      %三角脉冲
r=T*conv(e,h);           %卷积后的波形

% fg_2_12_plot();

subplot(3,1,1);         %输出波形
plot(t,e);
ylim([-1 2]);
ylabel('矩形脉冲');
subplot(3,1,2);
plot(t,h);
ylim([-1 2]);
ylabel('三角脉冲');
subplot(3,1,3);

%总结：
% 由于卷积运算会导致输出信号的长度增加，所以在计算离散卷积r 后，
% 向量 r 的长度会超过向量 t 的长度
% r = r(1:length(t)); %截取r中从1到t长度的值

t1=-2:T:8;
xlim([-1 4]);
plot(t1,r);
ylabel('卷积后');

 

