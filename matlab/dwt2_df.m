function [a,h,v,d] = dwt2_df(x,Ld0,Hd0,Ld1,Hd1)
%DWT2 Single-level discrete 2-D wavelet transform.
%   DWT2 performs a single-level 2-D wavelet decomposition
%   with respect to either a particular wavelet ('wname',
%   see WFILTERS for more information) or particular wavelet filters
%   (Lo_D and Hi_D) you specify.
%
%   [CA,CH,CV,CD] = DWT2(X,'wname') computes the approximation
%   coefficients matrix CA and details coefficients matrices 
%   CH, CV, CD, obtained by a wavelet decomposition of the 
%   input matrix X.
%   'wname' is a string containing the wavelet name.
%
%   [CA,CH,CV,CD] = DWT2(X,Lo_D,Hi_D) computes the 2-D wavelet
%   decomposition as above given these filters as input:
%   Lo_D is the decomposition low-pass filter.
%   Hi_D is the decomposition high-pass filter.
%   Lo_D and Hi_D must be the same length.
%
%   Let SX = size(X) and LF = the length of filters; then
%   size(CA) = size(CH) = size(CV) = size(CD) = SA where
%   SA = CEIL(SX/2), if the DWT extension mode is set to
%   periodization. SA = FLOOR((SX+LF-1)/2) for the other
%   extension modes. For the different DWT extension modes, 
%   see DWTMODE.
%
%   [CA,CH,CV,CD] = DWT2(...,'mode',MODE) computes the wavelet 
%   decomposition with the extension mode MODE you specify.
%   MODE is a string containing the extension mode.
%
%   NOTE: When X represents an indexed image, then X as well 
%   as the output arrays CA, CH, CV, CD are m-by-n matrices.
%   When X represents a truecolor image, then they become 
%   m-by-n-by-3 arrays. These arrays consist of three m-by-n matrices
%  (representing the red, green, and blue color planes) concatenated 
%   along the third dimension.
%   For more information on image formats, see the reference pages 
%   of IMAGE and IMFINFO functions.
%
%   Example:
%     x = magic(4);
%     [ca,ch,cv,cd] = dwt2(x,'db1','mode','sym')
%
%   See also DWTMODE, IDWT2, WAVEDEC2, WAVEINFO.

%   M. Misiti, Y. Misiti, G. Oppenheim, J.M. Poggi 12-Mar-96.
%   Last Revision: 06-Feb-2011.
%   Copyright 1995-2011 The MathWorks, Inc.

% Check arguments.
% nbIn = nargin;
% error(nargchk(2,7,nbIn,'struct'))
% if ischar(varargin{1})
%     [Lo_D,Hi_D] = wfilters(varargin{1},'d'); next = 2;
% else
%     Lo_D = varargin{1}; Hi_D = varargin{2};  next = 3;
% end

% Check arguments for Extension and Shift.
DWT_Attribute = getappdata(0,'DWT_Attribute');
if isempty(DWT_Attribute) , DWT_Attribute = dwtmode('get'); end
dwtEXTM = DWT_Attribute.extMode; % Default: Extension.
shift   = DWT_Attribute.shift2D; % Default: Shift.
% for k = next:2:nbIn-1
%     switch varargin{k}
%       case 'mode'  , dwtEXTM = varargin{k+1};
%       case 'shift' , shift   = mod(varargin{k+1},2);
%     end
% end

% Compute sizes.
lf0 = length(Ld0);
lf1 = length(Ld1);
sx = size(x);
% Extend, Decompose &  Extract coefficients.
first = 2-shift;
flagPer = isequal(dwtEXTM,'per');
if flagPer
    sizeEXT0 = lf0-1; last = sx+[lf1-1,lf0-1];
     sizeEXT1 = lf1-1; 
else
    sizeEXT0 = lf0/2; last = 2*ceil(sx/2);
    sizeEXT1 = lf1/2; 
end
x = double(x);
y = wextend('addcol',dwtEXTM,x,sizeEXT0);
z = conv2(y,Ld0(:)','valid');
a = convdown(z,Ld1,dwtEXTM,sizeEXT1,first,last);
h = convdown(z,Hd1,dwtEXTM,sizeEXT1,first,last);
z = conv2(y,Hd0(:)','valid');
v = convdown(z,Ld1,dwtEXTM,sizeEXT1,first,last);
d = convdown(z,Hd1,dwtEXTM,sizeEXT1,first,last);

%-------------------------------------------------------%
% Internal Function(s)
%-------------------------------------------------------%
function y = convdown(x,F,dwtEXTM,lenEXT,first,last)
y = x(:,first(2):2:last(2));
y = wextend('addrow',dwtEXTM,y,lenEXT);
y = conv2(y',F(:)','valid');
y = y';
y = y(first(1):2:last(1),:);
%-------------------------------------------------------%
