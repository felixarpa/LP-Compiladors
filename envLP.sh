setenv PATH /opt/pccts/bin:$PATH
if ( $?CPATH ) then
    setenv CPATH /opt/pccts/h:$CPATH
else
    setenv CPATH /opt/pccts/h
endif
