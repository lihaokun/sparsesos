function [t] = sparsepop_bxm(n)
    tic; 
    param.relaxOrder = 3;
    param.SDPsolver = 'sdpt3';
    sparsePOP(['BXM_',num2str(n),'.gms'],param);
    t = toc;
    fprintf("Totel time=%.2fs\n",t);
end

