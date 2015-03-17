function [payoff,bbar,K]=azacispayoff(x,q)
% x is an nx2 array of probabilities, and x(i,1) is the probability that
% signal i is 1 if the player's opponent is v_l and x(i,2) is the prob that
% signal i is 1 if player's opponent is v_h

x=min(max(x,0),1);

n=length(x)/2;
y=x((n+1):end);
x=x(1:n);

% Put x in increasing likelihood ratio
[~,I]=sort(x./y);
x=x(flipud(I)); y=y(flipud(I));

% Find the b bars and K
bbar = zeros(n,1);
K = bbar;
bbar(1) = 1-q*x(1)/(q*x(1)+(1-q)*y(1)^2);
K(1)=q*x(1)/(q*x(1)+(1-q)*y(1));
for i=2:n
    bbar(i)=1 - (q*x(i)+(1-q)*y(i)*sum(y(1:(i-1))))/(q*x(i)+(1-q)*y(i)*sum(y(1:i)))*(1-bbar(i-1));
    K(i)=(q*x(i)+(1-q)*y(i)*sum(y(1:(i-1))))/(q*x(i)+(1-q)*y(i))*(1-bbar(i-1));
end



% Find the ex ante payoff
payoff = (1-q)*sum((q*x+(1-q)*y).*K);

end % azacispayoff