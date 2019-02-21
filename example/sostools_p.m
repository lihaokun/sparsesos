function [prog,info,t] = sostools_p(n)

    fprintf("init..");
    tic; 
    syms  x y a b c d e f;
    vartable=[x,y,a,b,c,d,e,f];
    prog = sosprogram(vartable);
    p=eval(fileread(['P',int2str(n),'.txt']));
    t1 = toc;
    fprintf("done.(%.2fs)\n",t1);
    tic; 
    prog = sosineq(prog,p,'sparse');
    solver_opt.solver = 'sdpt3';
    [prog,info] = sossolve(prog,solver_opt);%
    t = toc;
    fprintf("Totel time=%.2fs\n",t);
end

