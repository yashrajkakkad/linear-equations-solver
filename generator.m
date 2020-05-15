%Generate random systems of equations
%Submission by : Yashraj Kakkad
%                AU1841036

%define number of test cases here

t = 100;

inputID = fopen('input.txt', 'wt');
answerID = fopen('matlabanswer.txt', 'wt');
fprintf(inputID, "%d\n", t);
for tt = 1:t
    m = randi([1 10], 1, 1);
    n = randi([1 10], 1, 1);
    A = randi([1,20], m, n);
    b = randi([1,20], m, 1);
    x = A\b;
    N = null(A);
    fprintf(inputID, "%d ", m);
    fprintf(inputID, "%d\n", n);
    for ii = 1:size(A,1)
        fprintf(inputID,'%g ',A(ii,:));
        fprintf(inputID,'\n');
    end
    for ii = 1:size(b,1)
        fprintf(inputID, '%g ',b(ii, :));
        fprintf(inputID, '\n');
    end
    for ii = 1:size(x,1)
        fprintf(answerID,'%g ',x(ii,:));
        fprintf(answerID,'\n');
    end
    for ii = 1:size(n,1)
        fprintf(answerID,'%g ',n(ii,:));
        fprintf(answerID,'\n');
    end
end
fclose(inputID);
fclose(answerID);

