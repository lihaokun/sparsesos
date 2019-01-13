function [sol,v,Q] = yalmip_F(n)
    fprintf("init..");
    tic; 
    sdpvar x0 x1 x2 x3 x4 x5 x6 x7 x8 x9;
    p=eval(fileread(['F',int2str(n),'.txt']));
    t1 = toc;
    fprintf("done.(%.2fs)\n",t1);
    tic;
    f=sos(p);
    ops = sdpsettings('solver','sdpt3');
    [sol,v,Q]=solvesos(f);%,0,ops);%
    t=toc;
    fprintf("Totel time=%.2fs\n",t);
end