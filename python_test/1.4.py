
def lfsr(initial_state, poly, length):
    state = initial_state.copy()
    n = len(state)
    output_sequence = []

    for _ in range(length):
        output_sequence.append(state[-1])
        feedback_bit = state[n - 1]
        
        for i in range(n - 1, 0, -1):
            if poly[i] == 1:
                feedback_bit ^= state[i - 1]
            state[i] = state[i - 1]
        
        state[0] = feedback_bit

    return output_sequence
# 初始状态
initial_state = [1, 0, 0, 1, 0, 1, 1]
# 多项式 p(x) = 1 + x + x^7 对应的系数
poly = [1, 0, 0, 0, 0, 0, 1, 1]
# 产生1024个比特的序列来确定周期
output_sequence = lfsr(initial_state, poly, 1024)

# 寻找周期
for cycle_length in range(1, 1024):
    if output_sequence[:cycle_length] == output_sequence[cycle_length:2*cycle_length]:
        print("序列的周期是:", cycle_length)
        break
# 定义将字符串转换为ASCII二进制表示的函数
def str_to_ascii_bin(string):
    return ''.join(format(ord(char), '08b') for char in string)

def bin_to_str(binary):
    chars = [binary[i:i+8] for i in range(0, len(binary), 8)]
    return ''.join(chr(int(char, 2)) for char in chars)

def xor_encrypt_decrypt(data, key):
    return ''.join(str(int(a) ^ int(b)) for a, b in zip(data, key))

#设置姓名和学号
name = "Zhao Fangyuan"
student_id = "26224012"

# 转换为二进制字符串
name_bin = str_to_ascii_bin(name)
student_id_bin = str_to_ascii_bin(student_id)

# 截取对应长度的LFSR序列作为密钥
key_name = ''.join(map(str, output_sequence[:len(name_bin)]))
key_student_id = ''.join(map(str, output_sequence[:len(student_id_bin)]))

# 加密
encrypted_name_bin = xor_encrypt_decrypt(name_bin, key_name)
encrypted_student_id_bin = xor_encrypt_decrypt(student_id_bin, key_student_id)

# 解密
decrypted_name_bin = xor_encrypt_decrypt(encrypted_name_bin, key_name)
decrypted_student_id_bin = xor_encrypt_decrypt(encrypted_student_id_bin, key_student_id)

# 转换回字符串
decrypted_name = bin_to_str(decrypted_name_bin)
decrypted_student_id = bin_to_str(decrypted_student_id_bin)

print("Original Name:", name)
print("Encrypted Name (binary):", encrypted_name_bin)
print("Decrypted Name:", decrypted_name)

print("Original Student ID:", student_id)
print("Encrypted Student ID (binary):", encrypted_student_id_bin)
print("Decrypted Student ID:", decrypted_student_id)
