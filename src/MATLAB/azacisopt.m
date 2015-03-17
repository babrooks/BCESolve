function [payoff,xy]=azacisopt(n,q)
% Finds payoffs and signal probabilities for the Azacis-Vida 2010
% equilibria with n independent signals and probability q of being low
% valuation.

% Start with 1/3 2/3;
x0=[(1:(n-1))'/sum(1:(n-1));0;(1:n)'/sum(1:n)];

ub=ones(2*n,1);
ub(n)=0;
lb=zeros(2*n,1);

% [x,payoff]=fminsearch(@ (y) -azacisprobs(y,q),x0);
[x,payoff,exitflag,output]=fmincon(@ (y) -azacispayoff(y,q),x0,...
    [],[],kron(eye(2),ones(1,n)),[1;1],lb,ub,[],...
    optimset('algorithm','interior-point','tolfun',1e-14,'tolcon',1e-14,'maxiter',1e5,'maxfunevals',1e6,'display','notify'));

payoff = -payoff;

% Sort the output.
xy=[x(1:n) x(n+1:end)];
[~,I]=sort(xy(:,1)./xy(:,2));
xy=xy(flipud(I),:);

end % azacis
