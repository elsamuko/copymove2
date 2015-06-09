width = 4;
height = 4;

function retval = mat16x16(x,y,val)
    retval = zeros(256,256);
    retval(x,y) = 1;
    retval = idct2(retval);
endfunction


for y = 1:4
    for x = 1:4
        pos = 4*(y-1)+x;
        subplot(height,width,pos)
        imagesc(mat16x16(x,y,1))
    endfor
endfor

