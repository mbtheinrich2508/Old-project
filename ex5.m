%k = 1:100;
%p = k.^5 + k

%n = 70000;
%S = spdiags([0.5, 1, 0.2], [-1, 0, 1], n, n)
%A = full(S)

%k = 1:n;
%b = sqrt(k)'

%x = S\b

A = [6 8; 5 2];
b = [-8; 5];
norm(A, inf)
cond(A, inf)

xk = [0; 0];
D = diag(diag(A));
L = tril(A, -1);
U = triu(A, 1);

xk1 = (D+L)\(b-U*xk)
xk2 = (D+L)\(b-U*xk1)

C = -(D + L)\U;
is_diag_dom = norm(C) < 1

x_exact = A\b