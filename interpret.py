class Struct:
    def __init__(self, VBLimH, VBLimL, CMD, LineLim, IBatLim, UPSV, AuxCMD, AuxVal):
        self.VBLimH = VBLimH
        self.VBLimL = VBLimL
        self.CMD = CMD
        self.LineLim = LineLim
        self.IBatLim = IBatLim
        self.UPSV = UPSV
        self.AuxCMD = AuxCMD
        self.AuxVal = AuxVal

def parse_file(file_path):
    structs = []
    with open(file_path, 'r') as file:
        bytes_list = file.readlines()

    # Remove all the newlines and whitespace
    bytes_list = [x.strip() for x in bytes_list]
    i = 0
    while i < len(bytes_list):
        # Read and clean up the bytes, removing newlines and whitespace and converting to integers
        byte = int(bytes_list[i], 10)

        # Check for the start of the struct
        if byte == 65:
            i += 1
            next_byte = int(bytes_list[i].strip(), 10)

            if next_byte == 122:
                # Start of struct, so read the next 8 bytes into the struct
                i += 1
                VBLimH = int(bytes_list[i].strip(), 10)
                i += 1
                VBLimL = int(bytes_list[i].strip(), 10)
                i += 1
                CMD = int(bytes_list[i].strip(), 10)
                i += 1
                LineLim = int(bytes_list[i].strip(), 10)
                i += 1
                IBatLim = int(bytes_list[i].strip(), 10)
                i += 1
                UPSV = int(bytes_list[i].strip(), 10)
                i += 1
                AuxCMD = int(bytes_list[i].strip(), 10)
                i += 1
                AuxVal = int(bytes_list[i].strip(), 10)

                # Create the struct
                struct = Struct(VBLimH, VBLimL, CMD, LineLim, IBatLim, UPSV, AuxCMD, AuxVal)

                # Verify the checksum
                i += 1
                checksum = int(bytes_list[i].strip(), 10)

                # Calculate the checksum by adding up all the bytes in the struct
                calculated_checksum = (65 ^ 122 ^ VBLimH ^ VBLimL ^ CMD ^ LineLim ^ IBatLim ^ UPSV ^ AuxCMD ^ AuxVal) & 0xFF

                # Verify the checksum
                if checksum != calculated_checksum:
                    print(f"Warning: Checksum mismatch for struct at byte {i - 9}. Expected {checksum}, but calculated {calculated_checksum}.")
                else:
                    structs.append(struct)

        # Go to the next byte
        i += 1

    return structs

file_path = "output.txt"
structs = parse_file(file_path)
for struct in structs:
    print(struct.__dict__)
