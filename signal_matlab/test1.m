%第一章 绪论
%例1.3 信号的运算(难点)
%已知 x(t) = sin(2πt)u(t) ， y(t) = e−tu(t) 。试计算 t ∈ [0, 2] 区间的 z1(t) = 2x(t) ，
%z2(t) = x(t − 1) ，z3(t) = x(2t) ，z4(t) = x(t) + y(t) ，z5(t) = x(t)y(t) 。

t=0:0.01:2;
x = sin(2*pi*t);
y=exp(-t);
z1 = 2*x;
z4= x+y;
z5= x.*y;
plot(t,x,t,y,t,z1);
% subplot(3,1,1);
% plot(t,z1);
% subplot(3,1,2);
% plot(t,z4);
% subplot(3,1,3);
% plot(t,z5);
plot(t,x);
temp = zeros(size(x)); %定义一个临时变量temp,和x的长度相等的全0变量
z2=[temp(t<1),x(t<=1)];%把x(t)(0<t<1)拼接到的temp(0<t<1)后边，得到延时1的效果(前半部分为空数组的t:0-1，后半部分t:1-2为xt的t:0-1)
plot(t,z2);

t1=0:0.01:4;            %定义一个更长的x(t)
xL=sin(2*pi*t1);
z3=xL(1:2:length(xL));  %1是起始索引，2是步长，lenth(xL)是xL的最后一位索引    =>从xL第一个开始，隔一个取一个元素，直到最后一个lenth(xL)
plot(t1,xL,t,z3);       %压缩前和压缩后的波形



%例1.4 绘制信号波形，及各种css参数

figure;
hold on;            %保留上一次的曲线
plot(t,x);          % 以默认格式绘制 t-x
plot(t,z1,'r');     % 用红色实线绘制 t-z1
plot(t,z2,'k-o');   % 用黑色实线带圆圈标记绘制 t-z2
plot(t,z3,'g--');   %绿色虚线t-z3
xlabel('时间t(s)');
ylabel('信号');
legend('x原图形','z1:二倍x','z2:x时移','z3:x压缩');%图例
figure;
subplot(1,2,1);     %并列的两个子图:一行两列，选中第一个子图


% figure; % 生成新图框
% subplot(1,2,1); % 生成并列放置 2 个子图中的左边一个
% plot(t,z4,‘k’,t,x,‘b:’,t,y,‘r:’); % 用黑实线绘制 z4 ，蓝和红点线绘制 x 和 y
% title(‘z 4(t)’); % 填写标题
% subplot(1,2,2); % 生成并列放置 2 个子图中的右边一个
% plot(t,z5,‘k’,t,x,‘b:’,t,y,‘r:’);
% title(‘z 4(t)’);


%例1.5  ezplot是matlab一个功能强大的函数，可实现绘图功能。
syms t;
x=str2sym('1+2*sin(t)');
ezplot(x,[0,2*pi]);
xlim([0,10]);


