function [prog,info,t] = sostools_bxm(n)
%SOSTOOLS_BXM Summary of this function goes here
%   Detailed explanation goes here
    fprintf("init..");
    tic; 
    if (n==1)
        syms x0 x1 x2 x3 x4;
        vartable = [x0,x1,x2,x3,x4];
        prog = sosprogram(vartable);
    end
    if (n==2)
        syms x0 x1 x2 x3 x4 x5 x6 x7;
        vartable = [x0,x1,x2,x3,x4,x5,x6,x7];
        prog = sosprogram(vartable);
    end
    if (n==3)
        syms x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 x10;
        vartable = [x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10];
        prog = sosprogram(vartable);
    end
    if (n==4)
        syms x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 x10 x11 x12 x13;
        vartable = [x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13];
        prog = sosprogram(vartable);
    end
    if (n==5)
        syms x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 x10 x11 x12 x13 x14 x15 x16;
        vartable = [x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16];
        prog = sosprogram(vartable);
    end
    p=eval(fileread(['BXM_',int2str(n),'.txt']));
    t1 = toc;
    fprintf("done.(%.2fs)\n",t1);
    tic; 
    prog = sosineq(prog,p);
    solver_opt.solver = 'sdpt3';
    [prog,info] = sossolve(prog,solver_opt);
    t = toc;
    fprintf("Totel time=%.2fs\n",t);
end

