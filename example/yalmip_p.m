function [sol,v,Q] = yalmip_p(n)
    fprintf("init..");
    tic; 
    sdpvar  x y a b c d e f;
    p=eval(fileread(['P',int2str(n),'.txt']));
    t1 = toc;
    fprintf("done.(%.2fs)\n",t1);
    tic;
    f=sos(p);
    ops = sdpsettings('solver','sdpt3');
    [sol,v,Q]=solvesos(f);%,0,ops);%
    t=toc;
    fprintf("Totel time=%.2fs\n",t);
end