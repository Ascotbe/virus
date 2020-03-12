import sys
from argparse import ArgumentParser, FileType

def process_bin(num, src_fp, dst_fp, dst_raw):
    shellcode = ''
    shellcode_size = 0
    shellcode_raw = b''
    try:
        while True:
            code = src_fp.read(1)
            if not code:
                break

            base10 = ord(code) ^ num
            base10_str = chr(base10)
            shellcode_raw += base10_str.encode()
            code_hex = hex(base10)
            code_hex = code_hex.replace('0x','')
            if(len(code_hex) == 1):
                code_hex = '0' + code_hex
            shellcode += '\\x' + code_hex
            shellcode_size += 1
        src_fp.close()
        dst_raw.write(shellcode_raw)
        dst_raw.close()
        dst_fp.write(shellcode)
        dst_fp.close()
        return shellcode_size
    except Exception as e:
        sys.stderr.writelines(str(e))

def main():
    parser = ArgumentParser(prog='Shellcode X', description='[XOR The Cobaltstrike PAYLOAD.BINs] \t > Author: rvn0xsy@gmail.com')
    parser.add_argument('-v','--version',nargs='?')
    parser.add_argument('-s','--src',help=u'source bin file',type=FileType('rb'), required=True)
    parser.add_argument('-d','--dst',help=u'destination shellcode file',type=FileType('w+'),required=True)
    parser.add_argument('-n','--num',help=u'Confused number',type=int, default=90)
    parser.add_argument('-r','--raw',help=u'output bin file', type=FileType('wb'), required=True)
    args = parser.parse_args()
    shellcode_size = process_bin(args.num, args.src, args.dst, args.raw)
    sys.stdout.writelines("[+]Shellcode Size : {} \n".format(shellcode_size))

if __name__ == "__main__":
    main()