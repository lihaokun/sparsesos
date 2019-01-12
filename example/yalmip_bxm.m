function [sol,v,Q] = yalmip_bxm(n)
        fprintf("init..");
    tic; 
    if (n==1)
        sdpvar x0 x1 x2 x3 x4;
    end
    if (n==2)
        sdpvar x0 x1 x2 x3 x4 x5 x6 x7;
    end
    if (n==3)
        sdpvar x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 x10;
    end
    if (n==4)
        sdpvar x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 x10 x11 x12 x13;
    end
    if (n==5)
        sdpvar x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 x10 x11 x12 x13 x14 x15 x16;
    end
    if (n==10)
        sdpvar x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 x10 x11 x12 x13 x14 x15 x16...
            x17 x18 x19 x20 x21 x22 x23 x24 x25 x26 x27 x28 x29 x30 x31;
    end
    p=eval(fileread(['BXM_',int2str(n),'.txt']));
    t1 = toc;
    fprintf("done.(%.2fs)\n",t1);
    tic;
    f=sos(p);
    ops = sdpsettings('solver','sdpt3');
    [sol,v,Q]=solvesos(f,0,ops);%);%
    t=toc;
    fprintf("Totel time=%.2fs\n",t);
end
