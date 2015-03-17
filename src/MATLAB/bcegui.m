% bcegui
% BAB
% 5/15/2012
function bcegui(varargin)
panelColor=get(0,'DefaultUIcontrolBackgroundColor');

set(0,'Units','characters');
screenSize=get(0,'ScreenSize');
fSize=[0 0 300 70];
if screenSize(4)<fSize(4)
    fSize(4)=screenSize(4)-15;
end
if screenSize(3)<fSize(3)
    fSize(3)=screenSize(3)-15;
end
fPos=[(screenSize(3:4)-fSize(3:4))/2,fSize(3:4)];

% Figure
figuretitle='Bayes Correlated Equilibrium Analysis Tool';
f=figure('Units','characters',...
    'Position',fPos,...
    'Name',figuretitle,...
    'Color',panelColor,...
    'NumberTitle','off',...
    'MenuBar','none',...
    'Toolbar','figure',...
    'Resize','on',...
    'ResizeFcn',@fResizeFcn);

%% Panels
botPanelHeight = 8;
botPanel = uipanel(f,...
    'Units','characters',...
    'Position',[0,0,fSize(3),botPanelHeight],...
    'BackgroundColor',panelColor,...
    'BorderType','none',...
    'ResizeFcn',@botPanelResizeFcn);

rightPanel = uipanel(f,...
    'Units','characters',...
    'Position',[fSize(3)/3,botPanelHeight,2*fSize(3)/3,fSize(4)-botPanelHeight],...
    'BorderType','none',...
    'BackgroundColor',panelColor);

leftTopPanel = uipanel(f,...
    'Units','characters',...
    'Position',[0,botPanelHeight+(fSize(4)-botPanelHeight)/2,fSize(3)/3,(fSize(4)-botPanelHeight)/2],...
    'BorderType','none',...
    'BackgroundColor',panelColor);
leftCenterPanel = uipanel(f,...
    'Units','characters',...
    'Position',[0,botPanelHeight,fSize(3)/3,(fSize(4)-botPanelHeight)/2],...
    'BorderType','none',...
    'BackgroundColor',panelColor);

%% Tabs
leftCenterTG = uitabgroup('Parent',leftCenterPanel,...
    'FlowDirection','Bottomup');
devTab = uitab('Parent',leftCenterTG,...
    'Title','Deviation payoffs');
stateDistrTab = uitab('Parent',leftCenterTG,...
    'Title','Distribution of state');
set(leftCenterTG,'SelectionChangeCallback',@leftCenterTGCallback);

rightTG = uitabgroup('Parent',rightPanel,...
    'FlowDirection','bottomup');
distrTab = uitab('Parent',rightTG,...
    'Title','Joint distribution of actions');
stateAjTab = uitab('Parent',rightTG,...
    'Title','Joint distribution of s and a_j');
% objectivesTab = uitab('Parent',rightTG,...
%     'Title','Objectives',...
%     'Units','characters',...
%     'ResizeFcn',@objectivesTabResizeFn);
% priorTab = uitab('Parent',rightTG,...
%     'Title','Prior',...
%     'Units','characters',...
%     'ResizeFcn',@priorTabResizeFn);
set(rightTG,'SelectionChangeCallback',@rightTGCallback);

%% Axes
frontierAx = axes('Parent',leftTopPanel); axis square;
axes('Parent',devTab);
devPayoffAx1 = subplot(2,1,1); 
devPayoffAx2 = subplot(2,1,2); 
axes('Parent',stateDistrTab);
stateDistrAx1 = subplot(2,1,1); 
stateDistrAx2 = subplot(2,1,2); 
distrAx = axes('Parent',distrTab); axis square;

axes('Parent',stateAjTab);
stateAjAx1 = subplot(2,1,1);
stateAjAx2 = subplot(2,1,2);

title(devPayoffAx1,'Player 1''s deviation payoff given a1 and t1');
xlabel(devPayoffAx1,'Deviation');
ylabel(devPayoffAx1,'Expected payoff');
title(devPayoffAx2,'Player 2''s deviation payoff given a2 and t2');
xlabel(devPayoffAx2,'Deviation');
ylabel(devPayoffAx2,'Expected payoff');
title(stateDistrAx1,'Conditional distribution of state given a1 and t1');
xlabel(stateDistrAx1,'State');
title(stateDistrAx2,'Conditional distribution of state given a1 and t1');
xlabel(stateDistrAx2,'State');
title(distrAx,'Joint distribution of actions given state');
xlabel(distrAx,'Player 2''s action');
ylabel(distrAx,'Player 1''s action');
zlabel(distrAx,'Probability');
title(frontierAx,'Set of values in BCE');
xlabel(frontierAx,'Player 1''s payoff');
ylabel(frontierAx,'Player 2''s payoff');
title(stateAjAx1,'Joint distribution of s and a2 given t1 and a1');
xlabel(stateAjAx1,'a2');
title(stateAjAx2,'Joint distribution of s and a1 given t2 and a2');
xlabel(stateAjAx2,'a1');

%% Tables
% panelPos = get(objectivesTab,'Position');
% objectivesTable = uitable('Parent',objectivesTab,...
%     'units','characters',...
%     'Position',[1 1 panelPos(3)-2 panelPos(4)-2],...
%     'visible','off');
% panelPos = get(priorTab,'Position');
% priorTable = uitable('Parent',priorTab,...
%     'units','characters',...
%     'Position',[1 1 panelPos(3)-2 panelPos(4)-2],...
%     'visible','off');

%% Bottom panel controls
sliderWidth = 150;
offsets=[0 50 100];
aText1 = uicontrol('Parent',botPanel,...
    'Style','text',...
    'String','a1:',...
    'Units','characters',...
    'HorizontalAlignment','right',...
    'FontSize',13,...
    'Position',[1,3.5,5,2.5]);
aText2 = uicontrol('Parent',botPanel,...
    'Style','text',...
    'String','a2:',...
    'Units','characters',...
    'HorizontalAlignment','right',...
    'FontSize',13,...
    'Position',[1,.5,5,2.5]);
aEdit1 = uicontrol('Parent',botPanel,...
    'Style','edit',...
    'Enable','off',...
    'Units','characters',...
    'HorizontalAlignment','right',...
    'FontSize',13,...
    'Position',[8,4,8,2]);
aEdit2 = uicontrol('Parent',botPanel,...
    'Style','edit',...
    'Enable','off',...
    'Units','characters',...
    'HorizontalAlignment','right',...
    'FontSize',13,...
    'Position',[8,1,8,2]);
aSlider1 = uicontrol('Parent',botPanel,...
    'Style','slider',...
    'Units','characters',...
    'BackgroundColor',panelColor,...
    'Position',[18 4 30 1.75],...
    'Enable','off',...
    'Callback',@sliderCallback);
aSlider2 = uicontrol('Parent',botPanel,...
    'Style','slider',...
    'Units','characters',...
    'BackgroundColor',panelColor,...
    'Position',[18 1 30 1.75],...
    'Enable','off',...
    'Callback',@sliderCallback);
aSliders = [aSlider1 aSlider2];
tText1 = uicontrol('Parent',botPanel,...
    'Style','text',...
    'String','t1:',...
    'Units','characters',...
    'HorizontalAlignment','right',...
    'FontSize',13,...
    'Position',[offsets(2)+1,3.5,5,2.5]);
tText2 = uicontrol('Parent',botPanel,...
    'Style','text',...
    'String','t2:',...
    'Units','characters',...
    'HorizontalAlignment','right',...
    'FontSize',13,...
    'Position',[offsets(2)+1,.5,5,2.5]);
tEdit1 = uicontrol('Parent',botPanel,...
    'Style','edit',...
    'Enable','off',...
    'Units','characters',...
    'HorizontalAlignment','right',...
    'FontSize',13,...
    'Position',[offsets(2)+8,4,8,2]);
tEdit2 = uicontrol('Parent',botPanel,...
    'Style','edit',...
    'Enable','off',...
    'Units','characters',...
    'HorizontalAlignment','right',...
    'FontSize',13,...
    'Position',[offsets(2)+8,1,8,2]);
tSlider1 = uicontrol('Parent',botPanel,...
    'Style','slider',...
    'Units','characters',...
    'BackgroundColor',panelColor,...
    'Position',[offsets(2)+18 4 30 1.75],...
    'Enable','off',...
    'Callback',@sliderCallback);
tSlider2 = uicontrol('Parent',botPanel,...
    'Style','slider',...
    'Units','characters',...
    'BackgroundColor',panelColor,...
    'Position',[offsets(2)+18 1 30 1.75],...
    'Enable','off',...
    'Callback',@sliderCallback);
tSliders = [tSlider1 tSlider2];
vText1 = uicontrol('Parent',botPanel,...
    'Style','text',...
    'String','v1:',...
    'Units','characters',...
    'HorizontalAlignment','right',...
    'FontSize',13,...
    'Position',[offsets(3)+1,3.5,5,2.5]);
vText2 = uicontrol('Parent',botPanel,...
    'Style','text',...
    'String','v2:',...
    'Units','characters',...
    'HorizontalAlignment','right',...
    'FontSize',13,...
    'Position',[offsets(3)+1,.5,5,2.5]);
vEdit1 = uicontrol('Parent',botPanel,...
    'Style','edit',...
    'Enable','off',...
    'Units','characters',...
    'HorizontalAlignment','right',...
    'FontSize',13,...
    'Position',[offsets(3)+8,4,8,2]);
vEdit2 = uicontrol('Parent',botPanel,...
    'Style','edit',...
    'Enable','off',...
    'Units','characters',...
    'HorizontalAlignment','right',...
    'FontSize',13,...
    'Position',[offsets(3)+8,1,8,2]);
vSlider1 = uicontrol('Parent',botPanel,...
    'Style','slider',...
    'Units','characters',...
    'BackgroundColor',panelColor,...
    'Position',[offsets(3)+18 4 30 1.75],...
    'Enable','off',...
    'Callback',@sliderCallback);
vSlider2 = uicontrol('Parent',botPanel,...
    'Style','slider',...
    'Units','characters',...
    'BackgroundColor',panelColor,...
    'Position',[offsets(3)+18 1 30 1.75],...
    'Enable','off',...
    'Callback',@sliderCallback);
vSliders = [vSlider1 vSlider2];
vObj = [vSliders vText1 vText2 vEdit1 vEdit2];
set(vObj,'Visible','off');

sText = uicontrol('Parent',botPanel,...
    'Style','text',...
    'String','s:',...
    'Units','characters',...
    'HorizontalAlignment','right',...
    'FontSize',13,...
    'Position',[offsets(3)+1,3.5,5,2.5]);
sEdit = uicontrol('Parent',botPanel,...
    'Style','edit',...
    'Enable','off',...
    'Units','characters',...
    'HorizontalAlignment','right',...
    'FontSize',13,...
    'Position',[offsets(3)+8,4,8,2]);
sSlider = uicontrol('Parent',botPanel,...
    'Style','slider',...
    'Units','characters',...
    'BackgroundColor',panelColor,...
    'Position',[offsets(3)+18 4 30 1.75],...
    'Enable','off',...
    'Callback',@sliderCallback);
sliders = [aSliders tSliders sSlider vSliders];
sObj = [sText sEdit sSlider];
set(sliders,'sliderstep',[1 1]); 

%% Variables
localData = struct('a1',0,'a2',0,... actions
    't1',0,'t2',0,... types
    's',0,... state
    'v1',0,'v2',0,... values
    'na',[],'nt',[],'ns',[],'nv',[],...
    'distr',[],... distribution for plotting
    'dev1',[],... deviation payoffs for p1
    'dev2',[],... deviation payoffs for p2
    'state1',[],... conditional distribution of state given a1, t1
    'state2',[],... conditional distribution of state given a2, t2
    'objectives',[],...
    'prior',[],...
    'currentEq',1,...
    'defaultdir','',...
    'bndryObjectives',[],...
    'bndryObjectiveIndices',[1 2]);

%% Menus
fileMenu = uimenu(f,'Label','File');
loadMenu = uimenu(fileMenu,'Label','Load...',...
    'Callback',@loadCallback); %#ok<*NASGU>

settingsMenu = uimenu(f,'Label','Settings');
setBndryObjective1Menu = uimenu(settingsMenu,'Label','Set boundary objectives 1',...
    'Callback',@bndryObjectiveMenuCallback);
setBndryObjective2Menu = uimenu(settingsMenu,'Label','Set boundary objective 2',...
    'Callback',@bndryObjectiveMenuCallback);
setBndryObjectiveMenus = [setBndryObjective1Menu setBndryObjective2Menu];

displayMenu = uimenu(f,'Label','Display');
normalizeMenu = uimenu(displayMenu,'Label','Normalize (s,aj)|(ai,ti)',...
    'Callback',@checkToggleCallback);
sqrtMenu = uimenu(displayMenu,'Label','Apply sqrt to joint distribution',...
    'Callback',@checkToggleCallback);
fixViewMenu = uimenu(displayMenu,'Label','Fix camera angle',...
    'checked','off',...
    'Callback',@checkToggleCallback);

%% Helper functions
    function plotData(varargin)
        for arginCounter = 1:nargin
            switch varargin{arginCounter}
                case 'All'
                    plotData('Distribution',...
                        'Deviation1','Deviation2',...
                        'State1','State2',...
                        'Frontier');
                    
                case 'Player 1'
                    plotData('Deviation1','State1','sa2|a1t1');
                    
                case 'Player 2'
                    plotData('Deviation2','State2','sa1|a2t2');
                    
                case 'Distribution'
                    if strcmp(get(distrTab,'visible'),'on')
                        [~,localData.distr] = ...
                            bcemex('GetConditionalMarginal',...
                            localData.s,...
                            [],[],...
                            [],[],...
                            false,true(2,1),false(2,1));
                        na = bcemex('GetNumActions');
                        
                        [az,el] = view(distrAx);
                        if strcmp(get(sqrtMenu,'checked'),'on')
                            mesh(distrAx,real(sqrt(reshape(localData.distr,na(1),na(2)))));
                        else
                            mesh(distrAx,reshape(localData.distr,na(1),na(2)));
                        end
                        
                        if strcmp(get(fixViewMenu,'checked'),'on')
                            view(distrAx,az,el);
                        end
                        
                        title(distrAx,'Joint distribution of actions given state');
                        xlabel(distrAx,'Player 2''s action');
                        ylabel(distrAx,'Player 1''s action');
                        zlabel(distrAx,'Probability');
                    end
                    
                case 'Deviation1'
                    if strcmp(get(devTab,'visible'),'on')
                        [localData.dev1,prob] = bcemex('GetDeviationObjectives',...
                            0,localData.t1,localData.a1);
                        h = bar(devPayoffAx1,localData.dev1(:,1));
                        
                        v=ones(1,size(localData.dev1,1));
                        indifferent=abs(max(localData.dev1(:,1))-localData.dev1(:,1))<1E-3;
                        if any(indifferent)
                            v(indifferent)=100;
                        end
                        v(localData.a1+1)=32;
                        set(get(h,'children'),'CDataMapping','direct',...
                            'CData',v,...
                            'edgecolor','none');
                        
						ylim = [min(localData.dev1(:,1)),max(localData.dev1(:,1))];
						if (ylim(2)-ylim(1))>0
							ylim = ylim + [0 1]*(ylim(2)-ylim(1))*0.0;
						else
							ylim = [0,1];
						end
						set(devPayoffAx1,'ylim',ylim,'xlim',[0 size(localData.dev1,1)+1]);
						
                        title(devPayoffAx1,sprintf('Player 1''s deviation payoff given a1 and t1, p=%1.4f, payoff=%1.4f',prob,localData.dev1(localData.a1+1,1)));
                        xlabel(devPayoffAx1,'Deviation');
                        ylabel(devPayoffAx1,'Expected payoff');
                    end
                    
                case 'Deviation2'
                    if strcmp(get(devTab,'visible'),'on')
                        [localData.dev2,prob]= bcemex('GetDeviationObjectives',...
                            1,localData.t2,localData.a2);
                        h = bar(devPayoffAx2,localData.dev2(:,2));
                        
                        v=ones(1,size(localData.dev2,1));
                        indifferent=abs(max(localData.dev2(:,2))-localData.dev2(:,2))<1E-3;
                        if any(indifferent)
                            v(indifferent)=100;
                        end
                        v(localData.a2+1)=32;
                        set(get(h,'children'),'CDataMapping','direct',...
                            'CData',v,...
                            'edgecolor','none');
                        
						ylim = [min(localData.dev2(:,2)),max(localData.dev2(:,2))];
						if (ylim(2)-ylim(1))>0
							ylim = ylim + [0 1]*(ylim(2)-ylim(1))*0.0;
						else
							ylim = [0,1];
						end
						set(devPayoffAx2,'ylim',ylim,'xlim',[0 size(localData.dev2,1)+1]);

						title(devPayoffAx2,sprintf('Player 2''s deviation payoff given a1 and t1, prob=%1.4f, payoff=%1.4f',prob,localData.dev2(localData.a2+1,2)));
                        xlabel(devPayoffAx2,'Deviation action');
                        ylabel(devPayoffAx2,'Expected payoff');
                    end
                    
                case 'State1'
                    if strcmp(get(stateDistrTab,'visible'),'on')
                        [~,localData.state1] = bcemex('GetConditionalMarginal',...
                            [],localData.a1,[],localData.t1,[],true,false(2,1),false(2,1));
                        bar(stateDistrAx1,localData.state1);
                        title(stateDistrAx1,'Conditional distribution of state given a1 and t1');
                        xlabel(stateDistrAx1,'State');
                        if ~isempty(localData.nv)
                            indices = (0:(localData.ns-1))';
                            v2Indices = mod(indices,localData.nv(2));
                            v1Indices = (indices-v2Indices)/localData.nv(2);
                            labels = cellstr(strcat('(v1=',num2str(v1Indices),',v2=',num2str(v2Indices),')'));
                            set(stateDistrAx1,'XTickLabel',labels);
                        end
                    end
                    
                case 'State2'
                    if strcmp(get(stateDistrTab,'visible'),'on')
                        [~,localData.state2] = bcemex('GetConditionalMarginal',...
                            [],[],localData.a2,[],localData.t2,true,false(2,1),false(2,1));
                        bar(stateDistrAx2,localData.state2);
                        title(stateDistrAx2,'Conditional distribution of state given a1 and t1');
                        xlabel(stateDistrAx2,'State');
                        if ~isempty(localData.nv)
                            indices = (0:(localData.ns-1))';
                            v2Indices = mod(indices,localData.nv(2));
                            v1Indices = (indices-v2Indices)/localData.nv(2);
                            labels = cellstr(strcat('(v1=',num2str(v1Indices),',v2=',num2str(v2Indices),')'));
                            set(stateDistrAx2,'XTickLabel',labels);
                        end
                    end
                    
                case 'sa1|a2t2'
                    if strcmp(get(stateAjTab,'visible'),'on')
                        [~,condDistr] = bcemex('GetConditionalMarginal',...
                            [],[],localData.a2,[],localData.t2,...
                            true,[true false],[false false]);
                        
                        condDistr = reshape(condDistr,localData.ns,[])';

                        if strcmp(get(normalizeMenu,'Checked'),'on')
                            condSum = sum(condDistr,2);
                            condDistr(condSum>0,:)=bsxfun(@times,condDistr(condSum>0,:),1./condSum(condSum>0));
                        end
                        
                        xvals = (0:(localData.na(1)-1))';
                        keep=any(condDistr>0,2);
                        xvals(~keep,:)=[];
                        condDistr = condDistr(keep,:);
                        bar(stateAjAx2,xvals,condDistr);
                        
                        if isempty(localData.nv)
                            legendEntries = cellstr(strcat('s=',num2str((0:(localData.ns-1))')));
                        else
                            indices = (0:(localData.ns-1))';
                            v2Indices = mod(indices,localData.nv(2));
                            v1Indices = (indices-v2Indices)/localData.nv(2);
                            legendEntries = cellstr(strcat('v1=',num2str(v1Indices),',v2=',num2str(v2Indices)));
                        end
%                         set(stateAjAx2,'XTickLabel',cellstr(strcat('a1=',num2str(0:(localData.na(1)-1)))));
                        legend(stateAjAx2,legendEntries{:});
                        
                        title(stateAjAx2,'Joint distribution of s and a1 given t2 and a2');
                        xlabel(stateAjAx2,'a1');
                    end
                    
                case 'sa2|a1t1'
                    if strcmp(get(stateAjTab,'visible'),'on')
                        [~,condDistr] = bcemex('GetConditionalMarginal',...
                            [],localData.a1,[],localData.t1,[],...
                            true,[false true],[false false]);
                        condDistr = reshape(condDistr,localData.ns,[])';

                        if strcmp(get(normalizeMenu,'Checked'),'on')
                            condSum = sum(condDistr,2);
                            condDistr(condSum>0,:)=bsxfun(@times,condDistr(condSum>0,:),1./condSum(condSum>0));
                        end
                        
                        xvals = (0:(localData.na(2)-1))';
                        keep=any(condDistr>0,2);
                        xvals(~keep,:)=[];
                        condDistr = condDistr(keep,:);
                        bar(stateAjAx1,xvals,condDistr);

                        if isempty(localData.nv)
                            legendEntries = cellstr(strcat('s=',num2str((0:(localData.ns-1))')));
                        else
                            indices = (0:(localData.ns-1))';
                            v2Indices = mod(indices,localData.nv(2));
                            v1Indices = (indices-v2Indices)/localData.nv(2);
                            legendEntries = cellstr(strcat('v1=',num2str(v1Indices),',v2=',num2str(v2Indices)));
                        end
%                         set(stateAjAx1,'XTickLabel',cellstr(strcat('a2=',num2str(0:(localData.na(2)-1)))));
                        legend(stateAjAx1,legendEntries{:});
                        
                        title(stateAjAx1,'Joint distribution of s and a2 given t1 and a1');
                        xlabel(stateAjAx1,'a2');
                    end
                    
                case 'Frontier'
					point = [localData.bndryObjectives(localData.currentEq,localData.bndryObjectiveIndices(1)),...
                        localData.bndryObjectives(localData.currentEq,localData.bndryObjectiveIndices(2))];
                    h = plot(frontierAx,...
                        localData.bndryObjectives([1:end 1],localData.bndryObjectiveIndices(1)),...
                        localData.bndryObjectives([1:end 1],localData.bndryObjectiveIndices(2)),'b.-',...
                        point(1),point(2),'r.',...
                        point(1),point(2),'rx',...
                        point(1),point(2),'ro');
                    
                    maxes = max(localData.bndryObjectives(:,[localData.bndryObjectiveIndices]),[],1);
                    mins = min(localData.bndryObjectives(:,[localData.bndryObjectiveIndices]),[],1);
                    maxes = maxes+0.05*(maxes-mins);
                    mins = mins-0.05*(maxes-mins);
                    
                    set(h([end-1:end]),'markersize',11);
                    set(h(1),'markersize',5);
                    
                    axis(frontierAx,'equal');
                    if all(maxes>mins)
                        set(frontierAx,'xlim',[mins(1) maxes(1)],...
                            'ylim',[mins(2) maxes(2)]);
                    end
                    
                    title(frontierAx,sprintf('Set of values in BCE: (%1.3f,%1.3f)',point(1),point(2)));
                    
                    xlabel(frontierAx,'Player 1''s payoff');
                    ylabel(frontierAx,'Player 2''s payoff');
                    
                    set([frontierAx; get(frontierAx,'children')],'ButtonDownFcn',@frontierClickCallback);
            end % switch
        end % for
    end % plotData

    function fillTables(varargin)
        for arginCounter = 1:nargin
            switch varargin{arginCounter}
                case 'All'
                  % fillTables('Prior');
                case 'Prior'
                    localData.prior = bcemex('GetPrior');
                    
                    indices = (0:(numel(localData.prior)-1))';
                    t2Indices = mod(indices,localData.nt(2));
                    indices = indices - t2Indices;
                    indices = indices / localData.nt(2);
                    t1Indices = mod(indices,localData.nt(1));
                    indices = indices - t1Indices;
                    indices = indices / localData.nt(1);
                    if isempty(localData.nv)
                        sIndices = indices;
                        text = strcat('(s=',int2str(sIndices),...
                            ',t1=',int2str(t1Indices),...
                            ',t2=',int2str(t2Indices),')');
                    else
                        v2Indices = mod(indices,localData.nv(2));
                        indices = indices - v2Indices;
                        indices = indices / localData.nv(2);
                        v1Indices = indices;
                        text = strcat('(v1=',int2str(v1Indices),...
                            ',v2=',int2str(v2Indices),...
                            ',t1=',int2str(t1Indices),...
                            ',t2=',int2str(t2Indices),')');
                    end
                    % set(priorTable,'Data',[cellstr(text) num2cell(localData.prior)]);
                    
				case 'Objectives'
                    localData.objectives = bcemex('GetObjectives');
                        
                    indices = (0:(size(localData.objectives,1)-1))';
                    a2Indices = mod(indices,localData.na(2));
                    indices = indices - a2Indices;
                    indices = indices / localData.na(2);
                    a1Indices = mod(indices,localData.na(1));
                    indices = indices - a1Indices;
                    indices = indices / localData.na(1);
                    if isempty(localData.nv)
                        sIndices = indices;
                        text = strcat('(s=',int2str(sIndices),...
                            ',t1=',int2str(t1Indices),...
                            ',t2=',int2str(t2Indices),')');
                    else
                        v2Indices = mod(indices,localData.nv(2));
                        indices = indices - v2Indices;
                        indices = indices / localData.nv(2);
                        v1Indices = indices;
                        text = strcat('(v1=',int2str(v1Indices),...
                            ',v2=',int2str(v2Indices),...
                            ',t1=',int2str(t1Indices),...
                            ',t2=',int2str(t2Indices),')');
                    end
                    
            end % switch
        end % for
    end % fillTables

%% Callbacks
    function loadCallback(src,event)
        [filename,pathname]=uigetfile([localData.defaultdir '*.bce'],'Pick a file');
        if filename
            % If filename is 0, the user hit cancel. 
            try
                fname = filename;
                filename = [pathname filename];
                bcemex('Load',filename);
                localData.bndryObjectives = bcemex('GetExpectedObjectives');
                
                localData.na = bcemex('GetNumActions');
                localData.nt = bcemex('GetNumTypes');
                localData.ns = bcemex('GetNumStates');
                localData.nv = bcemex('GetNumValues');
                
                localData.a1 = 0; localData.a2 = 0;
                localData.t1 = 0; localData.t2 = 0;
                localData.s = 0;
                localData.v1 = 0; localData.v2 = 0;
                
                set(aEdit1,'String',num2str(localData.a1));
                set(aEdit2,'String',num2str(localData.a2));
                set(tEdit1,'String',num2str(localData.t1));
                set(tEdit2,'String',num2str(localData.t2));
                set(sEdit,'String',num2str(localData.s));
                if ~isempty(localData.nv)
                    set(vEdit1,'String',num2str(localData.v1));
                    set(vEdit2,'String',num2str(localData.v2));
                end
                
                localData.currentEq = 1;
                bcemex('SetCurrentEquilibrium',0);
                
                if localData.na(1)>1
                    set(aSlider1,'Enable','on',...
                                 'Max',localData.na(1)-1,...
                                 'min',0,'value',0,...
                                 'sliderstep',[1 1]/(localData.na(1)-1));
                else
                    set(aSlider1,'Enable','off');
                end % if
                if localData.na(2)>1
                    set(aSlider2,'Enable','on',...
                                 'Max',localData.na(2)-1,...
                                 'min',0,'value',0,...
                                 'sliderstep',[1 1]/(localData.na(2)-1));
                else
                    set(aSlider2,'Enable','off');
                end
                if localData.nt(1)>1
                    set(tSlider1,'enable','on',...
                                 'Max',localData.nt(1)-1,...
                                 'min',0,'value',0,...
                                 'sliderstep',[1 1]/(localData.nt(1)-1));
                else
                    set(tSlider1,'enable','off');
                end
                if localData.nt(2)>1
                    set(tSlider2,'enable','on',...
                                 'Max',localData.nt(2)-1,...
                                 'min',0,'value',0,...
                                 'sliderstep',[1 1]/(localData.nt(2)-1));
                else
                    set(tSlider2,'enable','off');
                end
                if isempty(localData.nv)
                    set(sObj,'visible','on');
                    set(vObj,'visible','off');
                    if localData.ns>1
                        set(sSlider,'enable','on',...
                                    'Max',localData.ns-1,...
                                    'min',0,'value',0,...
                                    'sliderstep',[1 1]/(localData.ns-1));
                    else
                        set(sSlider,'enable','off');
                    end
                else
                    set(sObj,'visible','off');
                    set(vObj,'visible','on');
                    if localData.nv(1)>1
                        set(vSlider1,'enable','on',...
                                     'Max',localData.nv(1)-1,...
                                     'min',0,'value',0,...
                                     'sliderstep',[1 1]/(localData.nv(1)-1));
                    else
                        set(vSlider1,'enable','off');
                    end
                    if localData.nv(2)>1
                        set(vSlider2,'enable','on',...
                                     'Max',localData.nv(2)-1,...
                                     'min',0,'value',0,...
                                     'sliderstep',[1 1]/(localData.nv(2)-1));
                    else
                        set(vSlider2,'enable','off');
                    end
                end
                
                localData.defaultdir = pathname;
                set(f,'name',[figuretitle ': ' fname]);

                plotData('All');
                % fillTables('All');
            catch ME
                errordlg(sprintf('Not able to open %s%s. Following error message retrieved:\n\n%s',...
                    pathname,filename,...
                    ME.getReport),...
                    'Unable to open');
            end
        end
    end % loadCallback

    function sliderCallback(src,event)
        % Update localData
        if src==aSlider1
            localData.a1 = round(get(src,'value'));
            set(aEdit1,'String',num2str(localData.a1));
            plotData('Player 1');
        elseif src==aSlider2
            localData.a2 = round(get(src,'value'));
            set(aEdit2,'String',num2str(localData.a2));
            plotData('Player 2');
        elseif src==tSlider1
            localData.t1 = round(get(src,'value'));
            set(tEdit1,'String',num2str(localData.t1));
            plotData('Player 1');
        elseif src==tSlider2
            localData.t2 = round(get(src,'value'));
            set(tEdit2,'String',num2str(localData.t2));
            plotData('Player 2');
        elseif src==sSlider
            localData.s = round(get(src,'value'));
            set(sEdit,'String',num2str(localData.s));
            plotData('Distribution');
        elseif src==vSlider1
            localData.v1 = round(get(src,'value'));
            localData.s = localData.v1*localData.nv(2)+localData.v2;
            set(vEdit1,'String',num2str(localData.v1));
            set(sEdit,'String',num2str(localData.s));
            plotData('Distribution');
        elseif src==vSlider2
            localData.v2 = round(get(src,'value'));
            localData.s = localData.v1*localData.nv(2)+localData.v2;
            set(vEdit2,'String',num2str(localData.v2));
            set(sEdit,'String',num2str(localData.s));
            plotData('Distribution');
        end % if
    end % sliderCallback

    function frontierClickCallback(src,event)
        currentPnt = get(frontierAx,'currentpoint');
        currentPnt = currentPnt(1,1:2);
        
        [~,I]=min(sum(bsxfun(@(x,y) x-y,localData.bndryObjectives(:,[localData.bndryObjectiveIndices]),currentPnt).^2,2));
        localData.currentEq = I;

        bcemex('SetCurrentEquilibrium',I-1);
        
        plotData('Frontier','Distribution','Deviation1','Deviation2');
    end % frontierClickCallback

    function leftCenterTGCallback(src,event)
        if strcmp(get(devTab,'visible'),'on')
            plotData('Deviation1','Deviation2');
        elseif strcmp(get(stateDistrTab,'visible'),'on')
            plotData('State1','State2');
        end
    end % leftCenterTGCallback

    function rightTGCallback(src,event)
        if strcmp(get(stateAjTab,'visible'),'on')
            plotData('sa1|a2t2','sa2|a1t1');
        elseif strcmp(get(distrTab,'visible'),'on')
            plotData('Distribution');
        end
    end % rightTGCallback

    function bndryObjectiveMenuCallback(src,event)
        newIndex = listdlg('ListString',cellstr(num2str((1:size(localData.bndryObjectives,2))')),...
            'SelectionMode','Single');
        
        if ~isempty(newIndex)
            localData.bndryObjectiveIndices(src==setBndryObjectiveMenus) = newIndex;
            
            plotData('Frontier');
        end % if
    end % bndryObjectiveMenuCallback

    function checkToggleCallback(src,event)
        if strcmp(get(src,'Checked'),'on')
            set(src,'Checked','off');
        else
            set(src,'Checked','on');
        end            
        if src==sqrtMenu
            plotData('Distribution');
        end
    end % checkToggleCallback

%% Resize functions
    function fResizeFcn(src,event)
        set([f botPanel leftTopPanel leftCenterPanel rightPanel] ,'Units','characters');
        fSize = get(f,'Position');
        set(botPanel,'Position',[0,0,fSize(3),botPanelHeight]);
        set(rightPanel,'Position',[fSize(3)/3,botPanelHeight,2*fSize(3)/3,fSize(4)-botPanelHeight]);
        set(leftTopPanel,'Position',[0,botPanelHeight+(fSize(4)-botPanelHeight)/2,fSize(3)/3,(fSize(4)-botPanelHeight)/2]);
        set(leftCenterPanel,'Position',[0,botPanelHeight,fSize(3)/3,(fSize(4)-botPanelHeight)/2]);
    end % fResizeFn

    function botPanelResizeFcn(src,event)
        
    end % botPanelResizeFn

%     function objectivesTabResizeFn(src,event)
%         set(objectivesTab,'Units','characters');
%         panelPos = get(objectivesTab,'Position');
%         set(objectivesTable,...
%             'units','characters',...
%             'Position',[1 1 panelPos(3)-4 panelPos(4)-2]);
%     end % objectivesTabResizeFn
% 
%     function priorTabResizeFn(src,event)
%         set(priorTab,'Units','characters');
%         panelPos = get(priorTab,'Position');
%         set(priorTable,...
%             'units','characters',...
%             'Position',[1 1 panelPos(3)-4 panelPos(4)-2]);
%     end % priorTabResizeFn

end % bcegui