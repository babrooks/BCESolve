% Ben's m file for solving the relaxed program in the continuum case
function [V,B]=relaxed
V = 0:0.001:1;
% V = sqrt(V);
gridSize = numel(V);
B = zeros(numel(V));

% odeopts = odeset('reltol',1e-9,'abstol',1e-9);
odeopts = odeset;
for k=1:numel(V)-2
    [t,y]=ode45(@(vhat,b) odehelper(vhat,b,V(k),B(k,1:k),gridSize),...
        V(k:end),0,odeopts);
    B(k:end,k)=y;
end

rev = sum(sum(B(~isnan(B))))/sum(sum(B(~isnan(B))>0))
surplus = 2/3-rev

mesh(V,V,B);

ylabel('Winning value: vhat');
xlabel('Losing value: v');
zlabel('Winning bid: beta(vhat,v)');
% axis square

set(gcf,'units','inches');
fpos = get(gcf,'position');
set(gcf,'PaperSize',fpos(3:4),'PaperPosition',[0 0 fpos(3:4)]);

print(gcf,'-dpdf','winningbid.pdf');

end

function bp = odehelper(vhat,b,v,B,gridSize)
bp = (v-b)/sum(B<=b)*gridSize;
end