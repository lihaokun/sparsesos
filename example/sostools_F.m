function [prog,info,t] = sostools_F(n)
   fprintf("init..");
    tic; 
    syms x0 x1 x2 x3 x4 x5 x6 x7 x8 x9;
    vartable=[x0,x1,x2,x3,x4,x5,x6,x7,x8,x9];
    prog = sosprogram(vartable);
    p=eval(fileread(['F',int2str(n),'.txt']));
    t1 = toc;
    fprintf("done.(%.2fs)\n",t1);
    tic; 
    prog = sosineq(prog,p);
    solver_opt.solver = 'sdpt3';
    [prog,info] = sossolve(prog,solver_opt);
    t = toc;
    fprintf("Totel time=%.2fs\n",t);
end