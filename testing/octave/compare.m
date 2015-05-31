
clear;

function retval = dct_beautified(v)
    sz = max(size(v));
    retval = dct2(v(:,:,2)-128)(1:sz,1:sz);
endfunction

function retval = cap_big_one(v)
    if(abs(v) > 99)
        retval = 0;
    else    
        retval = v;
    endif
endfunction

function retval = cap_big(v)
    retval = arrayfun(@cap_big_one, v);
endfunction

# function retval = selection(block)
#     retval = zeros(1,11);
#     # retval(1) = block(1,1);
#     retval(2) = block(1,2);
#     retval(3) = block(2,1);
#     retval(4) = block(3,1);
#     retval(5) = block(2,2);
#     retval(6) = block(1,3);
#     retval(7) = block(1,4);
#     retval(8) = block(2,3);
#     retval(9) = block(3,2);
#     retval(10) = block(4,1);
#     retval(11) = block(3,3);
# endfunction

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
#     retval = abs(retval);
    retval /= max(abs(retval));
endfunction

function retval = cosine_similarity(a,b)
    dotp = dot(a,b);
    d1 = sqrt(sum(a.*a));
    d2 = sqrt(sum(b.*b));
    retval = dotp/(d1*d2);
endfunction

load original.mat;
load cloned.mat;
load smiley.mat;

width = 3;
height = 4;

subplot(height,width,1)
imagesc(original(:,:,2))
title ("Original");

subplot(height,width,2)
imagesc(cloned(:,:,2))
title ("Cloned");

subplot(height,width,3)
imagesc(smiley(:,:,2))
title ("Smiley");

subplot(height,width,4)
dct_original = dct_beautified(original)
imagesc(dct_original)
title ("DCT(Original)");

subplot(height,width,5)
dct_cloned = dct_beautified(cloned)
imagesc(dct_cloned)
title ("DCT(Cloned)");

subplot(height,width,6)
dct_smiley = dct_beautified(smiley);
image(dct_smiley)
title ("DCT(Smiley)");

# Frequencies
subplot(height,width,7)
freqs_original = dfreqs0(dct_original)
plot(freqs_original)
title ("f Original")

subplot(height,width,8)
freqs_cloned = dfreqs0(dct_cloned)
plot(freqs_cloned)
title ("f Cloned")

subplot(height,width,9)
freqs_smiley = dfreqs0(dct_smiley)
plot(freqs_smiley)
title ("f Smiley")

similar = cosine_similarity(freqs_original(1:8),freqs_cloned(1:8))
unsimilar = cosine_similarity(freqs_original(1:8),freqs_smiley(1:8))


# Differences
subplot(height,width,10)
difference = floor(dct_original - dct_cloned) + 0.01;
imagesc(difference)
title ("Difference");

subplot(height,width,12)
difference2 = floor(dct_original - dct_smiley);
imagesc(difference2)
title ("Difference2");

subplot(height,width,11)
difference3 = cap_big(floor(difference2 ./ difference));
imagesc(difference3)
title ("Difference2/Difference");
