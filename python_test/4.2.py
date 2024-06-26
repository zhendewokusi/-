import random
from sympy import nextprime, mod_inverse
from functools import reduce

def generate_random_coefficients(secret, t, prime):
    coefficients = [secret] + [random.randint(1, prime - 1) for _ in range(t - 1)]
    return coefficients

def evaluate_polynomial(coefficients, x, prime):
    y = 0
    power_of_x = 1
    for coeff in coefficients:
        y = (y + coeff * power_of_x) % prime
        power_of_x = (power_of_x * x) % prime
    return y

def shamir_secret_sharing(secret, n, t):
    prime = nextprime(secret)  # 找到一个大于秘密值的质数作为有限域的素数
    coefficients = generate_random_coefficients(secret, t, prime)
    points = []
    for i in range(1, n + 1):
        y = evaluate_polynomial(coefficients, i, prime)
        points.append((i, y))
    return points, prime

def lagrange_interpolation(points, prime):
    def interpolate(x):
        total_sum = 0
        for i, (xi, yi) in enumerate(points):
            numerator = denominator = 1
            for j, (xj, yj) in enumerate(points):
                if i != j:
                    numerator = numerator * (x - xj) % prime
                    denominator = denominator * (xi - xj) % prime
            total_sum = (total_sum + yi * numerator * mod_inverse(denominator, prime)) % prime
        return total_sum
    return interpolate

# 测试示例
secret = "26224011Li Yantong"  # 学号+姓名作为秘密信息
secret_num = int.from_bytes(secret.encode(), 'big')  # 将字符串转换为整数

n = 5  # 总共的分片数
t = 3  # 恢复秘密所需的分片数

# Shamir秘密共享过程
points, prime = shamir_secret_sharing(secret_num, n, t)
print(f"生成的分片点：\n{points}")

# 恢复秘密信息
chosen_points = random.sample(points, t)  # 从分片点中随机选择t个点来恢复秘密
print(f"选择的分片点：\n{chosen_points}")

x_values, y_values = zip(*chosen_points)
interpolate = lagrange_interpolation(chosen_points, prime)
recovered_secret_num = interpolate(0)

# 将恢复的整数转换回字符串
recovered_secret = recovered_secret_num.to_bytes((recovered_secret_num.bit_length() + 7) // 8, 'big').decode()
print(f"\n恢复的秘密信息：\n{recovered_secret}")
