import ctypes

def main():
    lib = ctypes.cdll.LoadLibrary('./libcr.so')
    string_buffer = ctypes.create_string_buffer(1000)
    pointer = (ctypes.c_char_p)(ctypes.addressof(string_buffer))
    lib.searchTicket(pointer)
    info = string_buffer.value.decode('UTF-8').split()
    name = info[0::2]
    value = info[1::2]
    ab = {}
    for i in range(0, len(name)):
        print(name[i], value[i])

if __name__ == '__main__':
    main()
