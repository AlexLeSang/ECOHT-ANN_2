function f = b1()
clear,clc,close all 
format compact
% MATLAB neural network back propagation code
% by AliReza KashaniPour & Phil Brierley
% first version 29 March 2006
% 2's edition 14 agust 2007
%
% This code implements the basic backpropagation of
% error learning algorithm. The network has tanh hidden  
% neurons and a linear output neuron.
% and applied for predicting y=sin(2piX1)*sin(2piX2)
%
% adjust the learning rate with the slider
%
% feel free to improve! and notic us
% AliReza.kashaniPour@yahoo.com
% Special thank from Phil
%
%--------------------------------------------------------
%% Initializing
hidden_neurons = 30;
epochs = 700;

% ------- load in the data -------

X1 = linspace(0,0.5,10); %input
X2 = linspace(0,0.5,10); %input

[xx yy] = meshgrid(X1, X2);

Y_train = sin(xx(:)).*sin(yy(:)); %satisfy output

train_inp = [xx(:) yy(:)];   %setting input
train_out = [Y_train]; %seting Out put

% check same number of patterns in each
if size(train_inp,1) ~= size(train_out,1)
    disp('ERROR: data mismatch')
   return 
end    
fake_rand = [ 0.63358600,    0.83480100,    0.23745400,    0.52321100,    0.85529400,    0.49205700,    0.78515400,    0.06630500,    0.10606000,    0.87117500,    0.26862500,    0.56327400,    0.82896800,    0.93081200,    0.28282200,    0.10661300,    0.76146600,    0.00686100,    0.13602700,    0.70438700,    0.23325000,    0.90437700,    0.85558400,    0.88981400,    0.74716000,    0.92519400,    0.20502000,    0.47298900,    0.19015200,    0.30558800,    0.05330500,    0.82373800,    0.14038900,    0.29075900,    0.34694900,    0.99568300,    0.78281600,    0.13210300,    0.06198800,    0.88887600,    0.00327800,    0.33061300,    0.45215000,    0.83224500,    0.26142500,    0.73497200,    0.93885800,    0.02289000,    0.74183400,    0.07488400,    0.72727700,    0.97508300,    0.97926200,    0.58286100,    0.86489700,    0.72642100,    0.50805500,    0.06991800,    0.19941000,    0.69820700,    0.37550600,    0.25271500,    0.52194500,    0.51589500,    0.54347400,    0.86889400,    0.51157800,    0.32629100,    0.00099600,    0.57356600,    0.21516600,    0.00427400,    0.90417900,    0.66731600,    0.83651900,    0.16560300,    0.40228800,    0.77537700,    0.18849400,    0.14412200,    0.85026100,    0.91577100,    0.11920500,    0.82952300,    0.49863200,    0.98410300,    0.55594400,    0.00668700,    0.05402000,    0.75535400,    0.70489400,    0.42952600,    0.00807000,    0.22683900,    0.94542100,    0.55154400,    0.09573300,    0.45699900,    0.87783500,    0.09672900 ];
%standardise the data to mean=0 and standard deviation=1
%inputs
mu_inp = mean(train_inp);
sigma_inp = std(train_inp);
train_inp = (train_inp(:,:) - mu_inp(:,1)) / sigma_inp(:,1);

%outputs
train_out = train_out';
mu_out = mean(train_out);
sigma_out = std(train_out);
train_out = (train_out(:,:) - mu_out(:,1)) / sigma_out(:,1);
train_out = train_out';

%read how many patterns
patterns = size(train_inp,1);

%add a bias as an input
bias = ones(patterns,1);
train_inp = [train_inp bias];

%read how many inputs
inputs = size(train_inp,2);

%---------- data loaded ------------

%--------- add some control buttons ---------
%add button for early stopping
%hstop = uicontrol('Style','PushButton','String','Stop', 'Position', [5 5 70 20],'callback','earlystop = 1;'); 
earlystop = 0;

%add button for resetting weights
%hreset = uicontrol('Style','PushButton','String','Reset Wts', 'Position', get(hstop,'position')+[75 0 0 0],'callback','reset = 1;'); 
reset = 0;

%add slider to adjust the learning rate
hlr = 0.5;%uicontrol('Style','slider','value',.1,'Min',.01,'Max',1,'SliderStep',[0.01 0.1],'Position', get(hreset,'position')+[75 0 100 0]);

% ---------- set weights -----------------
%set initial random weights
who = [-0.0621970,    -0.1586820,    0.01842900,    -0.1575190,    -0.0466730,    0.02448400,    -0.0466390,    -0.1026640,    -0.0037470,    -0.0299300,    0.16246800,    -0.0128280,    0.05209600,    0.02834100,    -0.0627840,    -0.0836810,    -0.1187150,    0.08084200,    -0.1435840,    -0.3172370,    -0.1272740,    -0.1021570,    -0.2515290,    -0.0449630,    -0.1394620,    -0.0513170,    -0.0684680,    -0.1179450,    -0.0573740,    0.03582500 ];
wih = [ -0.0621970,    -0.1586820,    0.01842900,    -0.1575190,    -0.0466730,    0.02448400,    -0.0466390,    -0.1026640,    -0.0037470,    -0.0299300,    0.16246800,    -0.0128280,    0.05209600,    0.02834100,    -0.0627840,    -0.0836810,    -0.1187150,    0.08084200,    -0.1435840,    -0.3172370,    -0.1272740,    -0.1021570,    -0.2515290,    -0.0449630,    -0.1394620,    -0.0513170,    -0.0684680,    -0.1179450,    -0.0573740,    0.03582500 ;
-0.0123090,    -0.1249270,    0.09023100,    0.03059000,    -0.0835230,    0.01725700,    0.07115200,    -0.0453520,    -0.0785110,    0.06508700,    -0.1814280,    -0.0631080,    -0.0970960,    -0.0427730,    0.11732700,    -0.0510240,    -0.0905610,    -0.0815260,    -0.1106960,    -0.2096330,    -0.1488120,    0.02971400,    0.06047000,    -0.1825570,    -0.1471750,    0.10695700,    -0.0341260,    -0.0661900,    0.09181700,    -0.2493640;
0.00679600,    -0.0514150,    0.00156000,    -0.1697540,    0.06167500,    0.00573300,    -0.1935720,    -0.0614030,    0.04410100,    -0.0718840,    -0.0917900,    -0.1548330,    -0.0634850,    -0.1600940,    0.02461100,    -0.0582440,    -0.0404150,    -0.0566740,    -0.0481160,    0.06417400,    -0.0657850,    0.08276900,    0.06021600,    -0.0049430,    -0.0367390,    -0.0440990,    -0.0373080,    -0.2352440,    0.04906000,    -0.0726160
];
%weight_input_hidden = (randn(inputs,hidden_neurons) - 0.5)/10;
%weight_hidden_output = (randn(1,hidden_neurons) - 0.5)/10;

  weight_input_hidden = (wih - 0.5)/10;
  weight_hidden_output = (who - 0.5)/10;

%% Learining
epochs = 400;
%do a number of epochs
for iter = 1:epochs 
    
    %get the learning rate from the slider
    alr = hlr;
    blr = alr / 10;
    
    
    %loop through the patterns, selecting randomly
    for j = 1:patterns
        
        %select a random pattern
        patnum = round((fake_rand(j) * patterns) + 0.5);
        if patnum > patterns
            patnum = patterns;
        elseif patnum < 1
            patnum = 1;    
        end
	%set the current pattern
        this_pat = train_inp(patnum,:);
        act = train_out(patnum,1);
        
        %calculate the current error for this pattern
        
        xx = this_pat*weight_input_hidden;
        hval = (tanh(xx))';
        pred = hval'*weight_hidden_output';
        
        error = pred - act;

        % adjust weight hidden - output
        delta_HO = error.*blr .*hval;
        weight_hidden_output = weight_hidden_output - delta_HO';

        % adjust the weights input - hidden
       % a = alr.*error.*weight_hidden_output';
     %   m1 = (1-(hval.^2));
      %  m2 = a * m1;
       % m3 = m2 * this_pat;
        delta_IH= alr.*error.*weight_hidden_output'.*(1-(hval.^2))*this_pat;
        weight_input_hidden = weight_input_hidden - delta_IH';
        
    end
    % -- another epoch finished
    
    %plot overall network error at end of each epoch
    p1 = tanh(train_inp*weight_input_hidden)';
    pred = weight_hidden_output*p1;

    error = pred' - train_out;
    err(iter) =  (sum(error.^2))^0.5;
       
       figure(1)
    plot(err)
    %reset weights if requested
%      if reset
%          weight_input_hidden = (randn(inputs,hidden_neurons) - 0.5)/10;
%          weight_hidden_output = (randn(1,hidden_neurons) - 0.5)/10;
%          fprintf('weights reaset after %d epochs\n',iter);
%          reset = 0;
%      end
%      
%      %stop if requested
%      if earlystop
%          fprintf('stopped at epoch: %d\n',iter); 
%          break 
%      end 
%  
%      %stop if error is small
%      if err(iter) < 0.001
%          fprintf('converged at epoch: %d\n',iter);
%          break 
%      end
       
end
    weight_input_hidden
    weight_hidden_output
err
%% Testing
X3 = linspace(0.2,0.8,10)';
X4 = linspace(0.2,0.8,10)';

[xx1 yy1] = meshgrid(X3, X4);

train_test = [xx1(:) yy1(:)];

mu_test = mean(train_test);
sigma_test = std(train_test);
train_test = (train_test(:,:) - mu_test(:,1)) / sigma_test(:,1);
train_test = [train_test bias];

pred = weight_hidden_output*tanh(train_test*weight_input_hidden)';

%a = (pred' - Y_train)

% Finish
% fprintf('state after %d epochs\n',iter);
  a = (train_out* sigma_out(:,1)) + mu_out(:,1);
  b = (pred'* sigma_out(:,1)) + mu_out(:,1);
  act_pred_err = [a b b-a]  %display actual,predicted & error
  figure
mesh(X1,X2,vec2mat(a,10))
figure
mesh(X1,X2,vec2mat(b,10))
%  
%  figure
%  plot(X3,act_pred_err((1:(size(train_inp,1)/2)),2));
%  plot3(X3,X4,act_pred_err((1:(size(train_inp,1)/2)),2),'color','red','linewidth',2)
%  grid on
%  title(' Approximatly result (using Neural Networks');
%  figure
%  plot3(X1,X2,Y_train, 'color','green','linewidth',2)
%  plot(X1,Y_train)
%  grid on,title(' Sin(2*pi.*X1)*Sin(2*pi.*X2) Y = Orginal result');
end
