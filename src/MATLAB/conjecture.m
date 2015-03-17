%% 
vstar = 0.6; vbar = 1;
p = 0.5;

V = 0:0.005:vstar-0.01;
P = ones(1,numel(V))/numel(V)*p;

X = zeros(numel(V));


probSum = 0;
for b1=1:(numel(V)-1)
    
    bound2 = (V(b1+1)-V(b1))/(vstar-V(b1))*P(b1);
    if probSum + bound2 > 1-p
        X(b1,b1)=1-p-probSum;
        probSum = 1-p;
        break;
    end

    % Did not exceed probSum
    X(b1,b1)=bound2; probSum = probSum + bound2;
    
    for b2=b1-1:-1:1
        % Calculate bound1 and bound2
        bound2 = (V(b1+1)-V(b2))/(vstar-V(b1+1))*P(b1)-sum(X(b2:b1,b2));
        
        bound1 = inf;
        for k=b2:(b1-1)
            tmpBnd = (vbar-V(b1))/(V(b1)-V(k))*sum(X(b1,k+1:b1))-sum(X(b1,b2+1:k));
            bound1 = min(bound1,tmpBnd);
        end % k

        if bound1 < bound2
            % Bidder 1's constraint is binding. Fill up the constraint and
            % then move to the next b1.
            bound1 = min(bound1,1-p-probSum);
            X(b1,b2)=bound1;
            probSum = probSum + bound1;
            break;
        end

        % fprintf(1,'%1.2f %1.2f\n',bound1,bound2);
        
        bound2 = min(bound2,1-p-probSum);
        X(b1,b2) = bound2;
        probSum = probSum + bound2;
    end % b2
    
    if probSum >= 1-p
        break;
    end

end % b1

X(end,end) = 1-p-probSum;

mesh(V,V,X);
Pi1 = (1-p)*vbar-dot(V,sum(X,2))

Pi2 = p*vstar-dot(V,P)
