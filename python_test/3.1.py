import hashlib

def generate_mac(message, key):
    # 使用哈希算法生成消息认证码
    hmac = hashlib.sha256(key.encode() + message.encode()).hexdigest()
    return hmac

def verify_mac(message, key, mac):
    # 验证消息的完整性和真实性
    generated_mac = generate_mac(message, key)
    if generated_mac == mac:
        return True
    else:
        return False

# 用户输入学号和姓名
student_id = input("请输入学号：")
student_name = input("请输入姓名：")

# 构造消息
message = f"学号：{student_id}，姓名：{student_name}"
key = "secret_key"

# 生成消息认证码
mac = generate_mac(message, key)
print("生成的消息认证码：", mac)

# 验证消息的完整性和真实性
is_valid = verify_mac(message, key, mac)
if is_valid:
    print("消息认证码验证通过，消息的完整性和真实性得到保证。")
else:
    print("消息认证码验证失败，消息可能被篡改或不完整。")
