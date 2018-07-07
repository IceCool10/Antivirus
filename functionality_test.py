import subprocess
import os
import re

if __name__ == "__main__":
    first_method_wannacry   =   "\033[1;31mINFECTED FILE\033[0m: ( WannaCry )"
    second_method_wannacry  =   r"Signature : \x8b\x44\x24\x08\x8b\x4c\x24\x10"+"\n"\
r"\x0b\xc8\x8b\x4c\x24\x0c\x75\x09"+"\n"\
r"\x8b\x44\x24\x04\xf7\xe1\xc2\x10"+"\n"\
r"\x00\x53\xf7\xe1\x8b\xd8\x8b\x44"+"\n"\
r"\x24\x08\xf7\x64\x24\x14\x03\xd8"+"\n"\
r"\x8b\x44\x24\x08\xf7\xe1\x03\xd3"+"\n"\
r"\x5b\xc2\x10\x00"
    output_wannacry      =   subprocess.Popen(["./antiv","scan","viruses/wanncry.exe"], stdin=subprocess.PIPE,stderr=subprocess.PIPE,stdout=subprocess.PIPE).communicate()[0]
    print "WannaCry Test: "
    print ""
    print "First method Test:"
    if first_method_wannacry in output_wannacry:
        print "OK!"
    else:
        print "NOT OK!"
    print "Second method Test:"
    if second_method_wannacry in output_wannacry:
        print "OK!"
    else:
        print "NOT OK!"

    print ""
    print ""
    print "Cerber with UPX test:"
    print ""
    output_cerber_upxed     =   subprocess.Popen(["./antiv","scan","viruses/cerber_upxed.exe"], stdin=subprocess.PIPE,stderr=subprocess.PIPE,stdout=subprocess.PIPE).communicate()[0]
    upx_found               =   "UPX found"
    first_method_cerber     =   "\033[1;31mINFECTED FILE\033[0m: ( Cerber )"
    second_method_cerber    =   "\033[1;32mClean file!\033[0m\n"    

    print "UPX Detection Test: "
    if upx_found in output_cerber_upxed:
        print "OK!"
    print "First method Test: "
    if first_method_cerber in output_cerber_upxed:
        print "OK!"
    print "Second method Test: "
    if second_method_cerber in output_cerber_upxed:
        print "OK!"
