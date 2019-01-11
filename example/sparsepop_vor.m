function [t] = sparsepop_vor(n)
    tic; 
    param.relaxOrder = 3;
    param.SDPsolver = 'sdpt3';
    sparsePOP(['BXM_',num2str(n),'.gms'],param);
    t = toc;
end

