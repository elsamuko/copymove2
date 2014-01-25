
clear;

function retval = dct_beautified( v )
    retval = dct2(v)(1:16,1:16);
endfunction

load original.mat
load cloned.mat
load smiley.mat

width = 3;
height = 3;

subplot(height,width,1)
imagesc(original)
title ("Original");

subplot(height,width,2)
imagesc(cloned)
title ("Cloned");

subplot(height,width,3)
imagesc(smiley)
title ("Smiley");

subplot(height,width,4)
dct_original = dct_beautified(original)
image(abs(dct_original))
title ("DCT(Original)");

subplot(height,width,5)
dct_cloned = dct_beautified(cloned)
image(abs(dct_cloned))
title ("DCT(Cloned)");

subplot(height,width,6)
dct_smiley = dct_beautified(smiley)
image(abs(dct_smiley))
title ("DCT(Smiley)");

subplot(height,width,7)
difference = floor( dct_original - dct_cloned )
imagesc(abs(difference))
title ("Difference");

subplot(height,width,9)
difference2 = floor( dct_original - dct_smiley )
imagesc(abs(difference2))
title ("Difference 2");
