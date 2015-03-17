function bcemexbuild

if strcmp(computer,'PCWIN')
    boostlibdir = '-L./libs/w32';
    boostincldir = '-I..\..\..\boost\boost_1_49_0';
    compilerflags = 'COMPFLAGS=$COMPFLAGS /MT';
    boostlib = '-lboost_serialization-vc100-mt-s-1_49';
elseif strcmp(computer,'PCWIN64')
    boostlibdir = '-L./libs/w64';
    boostincldir = '-IE:/Program Files/boost/boost_1_54_0/';
    compilerflags = 'COMPFLAGS=$COMPFLAGS /MT';
    boostlib = '-lboost_serialization-vc100-mt-s-1_49';
    mex('-I../hpp','-I../cpp','-v',boostincldir,...
        '-I/usr/local/MATLAB/R2012a/extern/include',...
        boostlibdir, ...
        'bcemex.cpp','../cpp/bcedata.cpp',...
        './libs/w64/libboost_serialization-vc100-mt-s-1_54.lib',...
        compilerflags);
elseif strcmp(computer,'GLNXA64')
    boostlibdir = '-L/usr/local/lib/';
    boostincldir = '-I/usr/local/include/boost/';
    compilerflags = 'CFLAGS="-std=gnu++11 $CFLAGS"';
    LDFLAGS = 'LDFLAGS="-std=gnu++11 $LDFLAGS"';
	defines = 'DEFINES="-std=gnu++11 $DEFINES"';
	CXXFLAGS='CXXFLAGS="\$CXXFLAGS -std=gnu++11"';
    % boostlib = '/tmp/boost_1_51_0/stage/lib/libboost_serialization.a';
    boostlib = [];
    mex(CXXFLAGS,'-I../hpp','-I../cpp','-v',boostincldir,...
        '-I/usr/local/MATLAB/R2012a/extern/include',...
        boostlibdir, ...
        'bcemex.cpp','../cpp/bcedata.cpp',...
		'/usr/local/lib/libboost_serialization.a', ...
        LDFLAGS);
end


end % bcemexbuild