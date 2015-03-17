% Code for replacement rule auction equilibria
% BAB 9-19-2012
function [g,fval,exitflag,output,lambda] = replacementrule(gridSize)

f = ones(1,gridSize)/gridSize;
% f = rand(1,gridSize); f=f/sum(f);

% Both bidders draw independent bids from the distribution g. If the bidder
% with the lower valuation draws the higher bid, then both bids are
% replaced with the valuation of the lower bidder, or only the bidder with
% the higher valuation's bid is so replaced. 

g0 = tril(ones(gridSize)); 
% g0 = tril(rand(gridSize));
g0 = bsxfun(@times,g0,1./sum(g0,2));
g0 = reshape(g0',[],1);

% Need conditional probabilities to sum to 1. 
Aeq = kron(eye(gridSize),ones(1,gridSize));
beq = ones(gridSize,1);
LB = zeros(gridSize*gridSize,1);
UB = reshape(triu(ones(gridSize)),[],1);

[LB g0 UB];

% g0 = UB;

opt = optimset('Display','iter');

[g,fval,exitflag,output,lambda] = ...
	fmincon(@(g) revenue(g,f),...
	g0, ...
	[],[],...
	Aeq,beq,...
	LB,UB,...
	@(g) nonlcon(g,f),...
	opt);

end % replacementrule

function r = revenue(g,f)
% Revenue
gridSize = sqrt(numel(g));
r=0;
for v=1:gridSize
	for b=1:v
		j=J(b,v,g,f);
		r = r + b*j(b);
	end
end
r = 2 * r / gridSize;

end % obj

function [c,ceq] = nonlcon(g,f)
% We include gains of deviation for each (b,bi,vi). Most be non-positive.
n = sqrt(numel(g));
gridSize = n;
c = zeros((n-1)*n*(n+1)/3,1);
ceq = [];

filledupto = 0;
for v=1:gridSize
	for b=1:v
		j = J(b,v,g,f);
		c(filledupto+1:filledupto+v-1) = ...
			((v-[1:b-1 b+1:v]).*j([1:b-1 b+1:v]))' - (v-b)*j(b);
		filledupto = filledupto + (v-1);
	end
end

% assert(filledupto == (n-1)*n*(n+1)/3);
end % nonlcon

% Total probability of winning with a bid of b, given a recommendation of
% bi and valuation vi. Vector of probabilities for each bid.
function prob = J(bi,vi,g,f)
gridSize = numel(f);

g = reshape(g,gridSize,[])';
G = cumsum(g,2);

% G(b-Delta|vi)
G_lt = zeros(gridSize,gridSize); 
G_lt(:,2:end) = G(:,1:end-1);

prob = zeros(1,vi);

% Probability of winning against lower valuation types.
prob(1:bi) = f(vi)*g(vi,bi).*sum(bsxfun(@times,f(1:vi-1)',G(1:(vi-1),1:bi)),1);
if (vi>bi)
	% Rule 1. Only replace loser's tentative bid.
% 	prob(1:bi) = prob(1:bi) + f(vi)*f(bi)*cumsum(g(bi,1:bi).*G_lt(vi,1:bi),2);

	% Rule 2. Replace both tentative bids. Only practical impact is that
	% now the conditional CDF of the other player's bid is point mass on
	% bi. Technically, there is also mass on vi when told to bid vi, but we
	% can ignore this scenario, since it would only be optimal to bid vi if
	% there is 0 chance of facing any other bid less than vi, which
	% property does not depend on the actual mass on vi.
	prob(bi) = prob(bi) + f(vi)*f(bi)*sum(g(bi,1:bi).*G_lt(vi,1:bi));
end

% Probability of winning against higher valuation types.
prob(bi+1:vi) = prob(bi)...
	+ f(vi)*g(vi,bi)*sum(bsxfun(@times,f(vi+1:end)',...
	bsxfun(@minus,G_lt(vi+1:end,bi+1:vi),G_lt(vi+1:end,bi))),1);

% Prob of winning against same valuation
prob = prob + f(vi)^2*g(vi,bi).*(g(vi,1:vi)/2 + G_lt(vi,1:vi)); 

end % J