import sys
import pefile

pe  =   pefile.PE(sys.argv[1])
print pe.OPTIONAL_HEADER.AddressOfEntryPoint
