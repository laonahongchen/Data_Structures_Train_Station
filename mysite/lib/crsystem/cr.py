import ctypes

def main():
    lib = ctypes.cdll.LoadLibrary('./libcr.so')

    dataInput = ctypes.create_string_buffer("中国 海军".encode('UTF-8'), 1000)
    dataOutput = ctypes.create_string_buffer(1000)

    inputPointer = (ctypes.c_char_p)(ctypes.addressof(dataInput))
    outputPointer = (ctypes.c_char_p)(ctypes.addressof(dataOutput))

    lib.systemCheck(inputPointer, outputPointer)
    info = dataOutput.value.decode('UTF-8').split()
    print(info)

if __name__ == '__main__':
    main()