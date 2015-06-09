clear;

function retval = selection(block)
    retval = zeros(1,9);
    retval(1) = block(2,2);
    retval(2) = block(2,3);
    retval(3) = block(3,2);
    retval(4) = block(3,3);
    retval(5) = block(3,4);
    retval(6) = block(4,3);
    retval(7) = block(4,4);
    retval(8) = block(2,4);
    retval(9) = block(4,2);
endfunction

function retval = dfreqs0(block)
    retval = selection(block);
    mx = max(abs(retval));
    retval = round(20*retval./mx);
endfunction

function retval = cosine_similarity(a,b)
    dotp = dot(a,b);
    d1 = sqrt(sum(a.*a));
    d2 = sqrt(sum(b.*b));
    retval = dotp/(d1*d2);
endfunction

A = imread("A.png");
B = imread("B.png");

A = A(:,:,2)'
B = B(:,:,2)'

width = 2;
height = 3;

subplot(height,width,1)
imagesc(A)
title ("A");

subplot(height,width,2)
imagesc(B)
title ("B");

subplot(height,width,3)
dct_A = dct2(A)
imagesc(dct_A)
title ("DCT(A)");

subplot(height,width,4)
dct_B = dct2(B)
imagesc(dct_B)
title ("DCT(B)");

# Frequencies
subplot(height,width,5)
freqs_A = dfreqs0(dct_A)
plot(freqs_A)
title ("f A")

subplot(height,width,6)
freqs_B = dfreqs0(dct_B)
plot(freqs_B)
title ("f B")
