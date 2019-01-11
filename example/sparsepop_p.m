function [t] = sparsepop_p(n)
    tic; 
    param.relaxOrder = 3;
    param.SDPsolver = 'sdpt3';
    sparsePOP(['P_',num2str(n),'.gms'],param);
    t = toc;
end

