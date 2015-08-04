%% BAB 5-20-2014

%%
% Calculate winning bid distributions for 


%% Solve relaxed program
H = @(b,u,v) min(v,u./(v-b));

up = @(u,v) quadl(@(b) H(b,u,v),0,v*0.999);

V = linspace(0,1,100);
U = zeros(size(V));
for k=1:numel(V)
	high = 1; low = 0;
	while high-low>1e-8
		mid = (high + low)/2;
		
		x = up(mid,V(k));
		if x>mid
			low = mid;
		else
			high = mid;
		end
		
		
	end
	
	U(k)= high;
end

sum(U)*(V(2)-V(1))


%% Calculate winning bid distributions, compare to relaxed program
% bcemex('Load','Y:\work\code\bcesolve\devel\examples\fpaknown_nv=35_nb=35_highbid=0.5.bce');
if exist('f2','var')
    figure(f2);
else
    f2=figure;
end

nV = bcemex('GetNumTypes'); nV = nV(1);
nB = bcemex('GetNumActions'); nB = nB(1);

gbar = zeros(nV,nB);
% 
% for v = 1:nV
%     [prob1,distr1] = bcemex('GetConditionalMarginal',[],[],[],v-1,v-1,false,[true true],false(1,2));
% %     [prob1,distr1] = bcemex('GetConditionalMarginal',(v-1)^2-1,[],[],[],[],false,[true true],false(1,2));
%     distr1 = reshape(distr1,[],nB);
%     distr1 = sum(triu(distr1),2)-diag(distr1)/2;
%     [prob2,distr2] = bcemex('GetConditionalMarginal',[],[],[],v-1,0:(v-2),false,[true false],false(1,2));
%     gbar(v,:) = (prob1*distr1+prob2*distr2)'/(prob1/2+prob2);
% end
% plot((1:nB)',cumsum(gbar(1:nV,1:nB),2)')
% legendNames = arrayfun(@(x) {['v_{' num2str(x) '}']},(2:nV)');
% % legend(legendNames{:});
% set(gca,'ylim',[-0.1,1.1]);

% h = @(x) cumsum(x')';
% surf(1:nB,2:nV,h(gbar(2:nV,1:nB)))

hold on
gbar = zeros(nV,nB);

for v = 1:nV
%     [prob1,distr1] = bcemex('GetConditionalMarginal',[],[],[],v-1,v-1,false,[true true],false(1,2));
    [prob1,distr1] = bcemex('GetConditionalMarginal',(v-1)+nV*(v-1),[],[],[],[],false,[true true],false(1,2));
    distr1 = reshape(distr1,[],nB);
    distr1 = sum(triu(distr1),2)-diag(distr1)/2;
    [prob2,distr2] = bcemex('GetConditionalMarginal',v-1+nV*(0:v-2),[],[],[],[],false,[true false],false(1,2));
    gbar(v,:) = (prob1*distr1+prob2*distr2)'/(prob1/2+prob2);
end
% h = @(x) cumsum(x')';
h = @(x) x;
mesh(1:nB,2:nV,h(gbar(2:nV,1:nB))+0.001)

hold off

%% 

