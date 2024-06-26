import random

# 生成随机的置换表
def generate_substitution_table(length):
    alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"  
    substitution_table = list(alphabet)
    random.shuffle(substitution_table)
    return dict(zip(alphabet, substitution_table))

# 求逆置换
def inverse_substitution_table(substitution_table):
    return {v: k for k, v in substitution_table.items()}

# 加密
def encrypt_message(message, substitution_table):
    encrypted_message = ""
    for char in message:
        encrypted_message += substitution_table.get(char, char)
    return encrypted_message

# 解密
def decrypt_message(encrypted_message, inverse_substitution_table):
    decrypted_message = ""
    for char in encrypted_message:
        decrypted_message += inverse_substitution_table.get(char, char)
    return decrypted_message

substitution_table = generate_substitution_table(26)
print("随机生成的置换表：", substitution_table)

inverse_table = inverse_substitution_table(substitution_table)
print("逆置换表：", inverse_table)

student_id = "26224011"
name = "Li Yantong"
message = student_id + name

encrypted_message = encrypt_message(message, substitution_table)
print("加密后消息：", encrypted_message)

decrypted_message = decrypt_message(encrypted_message, inverse_table)
print("解密后消息：", decrypted_message)
