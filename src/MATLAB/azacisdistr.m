function [F,f,G,g]=azacisdistr(b,x,q)

[~,bbar]=azacispayoff([x(:,1);x(:,2)],q);

bbar=[0;bbar];



F=zeros(size(b)); f=F; G=F; g=f;
for i=1:(length(bbar)-1)
    F(b>=bbar(i))=sum(x(1:(i-1),2))+...
        x(i,2)*(q*x(i,1)+(1-q)*x(i,2)*sum(x(1:i-1,2)))/((1-q)*x(i,2)^2)...
        *(b(b>=bbar(i))-bbar(i))./(1-b(b>=bbar(i)));
    f(b>=bbar(i))=...
        x(i,2)*(q*x(i,1)+(1-q)*x(i,2)*sum(x(1:i-1,2)))/((1-q)*x(i,2)^2)...
        *(1-bbar(i))./(1-b(b>=bbar(i))).^2;
    G(b>=bbar(i))=sum(x(1:(i-1),1))+...
        x(i,1)*(q*x(i,1)+(1-q)*x(i,2)*sum(x(1:i-1,2)))/((1-q)*x(i,2)^2)...
        *(b(b>=bbar(i))-bbar(i))./(1-b(b>=bbar(i)));
    g(b>=bbar(i))=...
        x(i,1)*(q*x(i,1)+(1-q)*x(i,2)*sum(x(1:i-1,2)))/((1-q)*x(i,2)^2)...
        *(1-bbar(i))./(1-b(b>=bbar(i))).^2;
end
F(b>bbar(end))=0;
f(b>bbar(end))=0;
G(b>bbar(end))=0;
g(b>bbar(end))=0;

end % azacisdistr